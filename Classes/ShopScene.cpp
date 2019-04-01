//
//  ShopScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 07/03/2019.
//

#include "ShopScene.h"
#include "SceneManagerScene.h"
#include "DynamicNodeHandler.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Shop/ShopDisplayItem.h>
#include <AzoomeeCommon/Data/Shop/ShopDataHandler.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ShopScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
	
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
		//toggle purchase screen for item
		_shopCarousel->setVisible(false);
		_purchasePopup->setVisible(true);
		_purchasePopup->setItemData(item);
		
	});
	this->addChild(_shopCarousel);
	
	_purchasePopup = ShopItemPurchasePopup::create();
	_purchasePopup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_purchasePopup->setPosition(visibleSize / 2);
	_purchasePopup->setPurchaseCallback([this](const ShopDisplayItemRef& item, bool purchased){
		if(purchased)
		{
			ModalMessages::getInstance()->startLoading();
			HttpRequestCreator* request = API::BuyReward(item->getPurchaseUrl(), this);
			request->execute();
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
			AudioMixer::getInstance()->playEffect(BACK_BUTTON_AUDIO_EFFECT);
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
	ShopDataHandler::getInstance()->getLatestData([this](bool success){
		if(success)
		{
			_shopCarousel->setShopData(ShopDataHandler::getInstance()->getShop());
		}
	});
	Super::onEnter();
}
void ShopScene::onExit()
{
	ShopDataHandler::getInstance()->setOnCompleteCallback(nullptr);
	Super::onExit();
}
void ShopScene::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
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
	DynamicNodeHandler::getInstance()->rebuildCurrentCTA();
}

void ShopScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagBuyReward)
	{
		HttpRequestCreator* request = API::GetInventory(ChildDataProvider::getInstance()->getParentOrChildId(), this);
		request->execute();
	}
	else if(requestTag == API::TagGetInventory)
	{
		ModalMessages::getInstance()->stopLoading();
		ChildDataParser::getInstance()->parseChildInventory(body);
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
	else
	{
		ModalMessages::getInstance()->stopLoading();
	}
}
void ShopScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	ModalMessages::getInstance()->stopLoading();
	_shopCarousel->refreshUI();
}

NS_AZOOMEE_END
