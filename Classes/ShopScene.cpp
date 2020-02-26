//
//  ShopScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 07/03/2019.
//

#include "ShopScene.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/Data/Shop/ShopDisplayItem.h>
#include <AzoomeeCommon/Data/Shop/ShopDataDownloadHandler.h>
#include "ModalMessages.h"
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include "PopupMessageBox.h"
#include "RewardManager.h"
#include <AzoomeeCommon/Device.h>

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool ShopScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	bool isIphoneX = TZ::Device::getInstance()->isDeviceIphoneX();
	
	_bgColour = LayerColor::create(Color4B(3, 36, 78,60));
	this->addChild(_bgColour);
	
	_wires = Sprite::create("res/shop/wires.png");
	_wires->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_wires->setScale(MAX(visibleSize.width, visibleSize.height) / _wires->getContentSize().width);
	this->addChild(_wires);
	
	_gradient = Sprite::create("res/shop/gradient_haze.png");
	_gradient->setScaleX(visibleSize.width / _gradient->getContentSize().width);
	_gradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_gradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	this->addChild(_gradient);
	
	_shopCarousel = ShopCarousel::create();
	_shopCarousel->setItemSelectedCallback([this](const ShopDisplayItemRef& item){
		_purchasePopup->setItemData(item);
        ModalMessages::getInstance()->startLoading();
        _getInvContext = GetInvContext::PRE_PURCHSE;
        ChildManager::getInstance()->updateInventory();
	});
	this->addChild(_shopCarousel);
	
	_purchasePopup = ShopItemPurchasePopup::create();
	_purchasePopup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_purchasePopup->setPosition(visibleSize / 2);
	_purchasePopup->setPurchaseCallback([this](const ShopDisplayItemRef& item, bool purchased){
		if(purchased)
		{
			ModalMessages::getInstance()->startLoading();
			AnalyticsSingleton::getInstance()->shopItemPurchased(item);
			HttpRequestCreator* request = API::BuyReward(item->getPurchaseUrl(), this);
			request->execute();
		}
		else
		{
			AnalyticsSingleton::getInstance()->shopPurchasePopupClosed(item);
		}
		_purchasePopup->setVisible(false);
		_shopCarousel->setVisible(true);
	});
	_purchasePopup->setVisible(false);
	
	if(_purchasePopup->getContentSize().width > visibleSize.width * 0.95f)
	{
		_purchasePopup->setScale((visibleSize.width * 0.95f) / _purchasePopup->getContentSize().width);
	}
	
	this->addChild(_purchasePopup);
	
	_backButton = ui::Button::create("res/shop/back_white_v_2.png");
	_backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_backButton->setAnchorPoint(Vec2(-0.25,1.25));
	_backButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			AudioMixer::getInstance()->playEffect("res/audio/Azoomee_Button_Click_01_v1.mp3");
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
		}
	});
	this->addChild(_backButton,1);
	
	_coinDisplay = CoinDisplay::create();
	_coinDisplay->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	_coinDisplay->setAnchorPoint(Vec2(1.2,(isIphoneX && isPortrait) ? 2.2f : 1.5));
	_coinDisplay->setTouchEnabled(false);
	this->addChild(_coinDisplay, 1);
	
	return true;
}

void ShopScene::onEnter()
{
    _inventoryUpdateListener = EventListenerCustom::create(ChildManager::kInventoryUpdatedEvent, [this](EventCustom* event){
        ModalMessages::getInstance()->stopLoading();
        switch (_getInvContext)
        {
            case PRE_PURCHSE:
            {
                _shopCarousel->refreshUI();
                const InventoryRef& inv = ChildManager::getInstance()->getLoggedInChild()->getInventory();
                const auto& invItems = inv->getItems();
                const auto& findFunc = [this](const InventoryItemRef& item){
                    return item->getItemId() == _purchasePopup->getItemData()->getInventoryItem()->getItemId();
                };
                if(std::find_if(invItems.begin(), invItems.end(), findFunc) != invItems.end()) // check is item already owned
                {
                    _purchasePopup->setItemData(nullptr);
                }
                else if(_purchasePopup->getItemData()->getPrice() <= ChildManager::getInstance()->getLoggedInChild()->getInventory()->getCoins())
                {
                    //toggle purchase screen for item
                    _shopCarousel->setVisible(false);
                    _purchasePopup->setVisible(true);
                }
                else
                {
                    _purchasePopup->setItemData(nullptr);
                    displayNotEnoughCoinsError();
                }
                break;
            }
            case POST_PURCAHSE:
            {
                onItemPurchaseCompleted();
                break;
            }
            case REFRESH:
            {
                _shopCarousel->refreshUI();
                break;
            }
        }
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_inventoryUpdateListener, this);
    AnalyticsSingleton::getInstance()->contentItemSelectedEvent("SHOP");
	ShopDataDownloadHandler::getInstance()->getLatestData([this](bool success){
		if(success)
		{
			_shopCarousel->setShopData(ShopDataDownloadHandler::getInstance()->getShop());
            _getInvContext = GetInvContext::REFRESH;
            ChildManager::getInstance()->updateInventory();
		}
	});
    // Make sure any rewards are redeemed on the server so we have the latest
    RewardManager::getInstance()->checkForPendingRewards();
	
	Super::onEnter();
}

void ShopScene::onExit()
{
	AnalyticsSingleton::getInstance()->contentItemClosedEvent();
	ShopDataDownloadHandler::getInstance()->setOnCompleteCallback(nullptr);
    
    _eventDispatcher->removeEventListener(_inventoryUpdateListener);
    
	Super::onExit();
}

void ShopScene::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	bool isIphoneX = TZ::Device::getInstance()->isDeviceIphoneX();
	if(_shopCarousel)
	{
		_shopCarousel->refreshUI();
	}
	_purchasePopup->setPosition(visibleSize / 2);
	if(_purchasePopup->getContentSize().width > visibleSize.width * 0.95f)
	{
		_purchasePopup->setScale((visibleSize.width * 0.95f) / _purchasePopup->getContentSize().width);
	}
	if(_purchasedAnim)
	{
		_purchasedAnim->resizeUI();
	}
	_bgColour->setContentSize(visibleSize);
	_wires->setScale(MAX(visibleSize.width, visibleSize.height) / _wires->getContentSize().width);
	_wires->setRotation(visibleSize.width < visibleSize.height ? 90 : 0);
	_gradient->setScaleX(visibleSize.width / _gradient->getContentSize().width);
	_coinDisplay->setAnchorPoint(Vec2(1.2,(isIphoneX && isPortrait) ? 2.2f : 1.5));
}

void ShopScene::onItemPurchaseCompleted()
{
    ModalMessages::getInstance()->stopLoading();
    
    _purchasePopup->setVisible(false);
    _shopCarousel->setVisible(false);
    _backButton->setVisible(false);
    
    _purchasedAnim = ShopItemPurchasedAnimation::create();
    _purchasedAnim->setItemData(_purchasePopup->getItemData());
    _purchasedAnim->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _purchasedAnim->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _purchasedAnim->setOnCompleteCallback([this](){
        _purchasedAnim->setVisible(false);
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.25), CallFunc::create([this](){// delay removing to prevent crash on android
            this->removeChild(_purchasedAnim);
            _purchasedAnim = nullptr;
        })));
        
        _backButton->setVisible(true);
        _shopCarousel->setVisible(true);
        _shopCarousel->refreshUI();
        _purchasePopup->setItemData(nullptr);
    });
    this->addChild(_purchasedAnim);
}

void ShopScene::displayNotEnoughCoinsError()
{
    PopupMessageBox* messageBox = PopupMessageBox::create();
    messageBox->setTitle(_("Oops! We can't find your coins."));
    messageBox->setBody(_("Earn more by watching and playing!"));
    messageBox->setButtonText(_("Back"));
    messageBox->setButtonColour(Colours::Color_3B::darkIndigo);
    messageBox->setPatternColour(Colours::Color_3B::azure);
    messageBox->setButtonPressedCallback([this](MessagePopupBase* pSender){
        pSender->removeFromParent();
    });
    this->addChild(messageBox, 1);
}

void ShopScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagBuyReward)
	{
        _getInvContext = GetInvContext::POST_PURCAHSE;
        ChildManager::getInstance()->updateInventory();
	}
	else
	{
		ModalMessages::getInstance()->stopLoading();
	}
}

void ShopScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	ModalMessages::getInstance()->stopLoading();
	_shopCarousel->refreshUI();
    if(errorCode == 402)
    {
        displayNotEnoughCoinsError();
    }
}

NS_AZ_END
