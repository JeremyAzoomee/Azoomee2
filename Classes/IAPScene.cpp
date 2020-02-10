//
//  IAPScene.cpp
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#include "IAPScene.h"
#include "SceneManagerScene.h"
#include "IAPProductDataHandler.h"
#include "RoutePaymentSingleton.h"
#include "PaymentSuccessScreen.h"
#include "PopupMessageBox.h"
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include "MarketingAssetManager.h"
#include "Style.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool IAPScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& contentSize = getContentSize();
	bool isPortrait = contentSize.width < contentSize.height;
	
	_marketingCarousel = MarketingCarousel::create();
	_marketingCarousel->setSizeType(ui::Layout::SizeType::PERCENT);
	_marketingCarousel->setSizePercent(isPortrait ? Vec2(1.0f, 0.5f) : Vec2(0.5f,1.0f));
	_marketingCarousel->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_LEFT);
	_marketingCarousel->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_LEFT);
	this->addChild(_marketingCarousel);
	
	_productLayout = ProductLayout::create();
	_productLayout->setSizeType(ui::Layout::SizeType::PERCENT);
	_productLayout->setSizePercent(isPortrait ? Vec2(1.0f, 0.34f) : Vec2(0.5f,0.68f));
	_productLayout->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_TOP_RIGHT);
	_productLayout->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE : Vec2::ANCHOR_TOP_RIGHT);
	_productLayout->setProductLayoutType(ProductLayoutType::SUBSCRIPTION);
	std::vector<std::pair<std::string, std::string>> products = {{_("7-day free trial"),IAPProductDataHandler::getInstance()->getHumanReadableProductPrice()}};
	_productLayout->setProductData(products);
	_productLayout->setIapActionCallback([this](IAPAction action){
		switch (action) {
			case IAPAction::PURCHASE:
			{
				RoutePaymentSingleton::getInstance()->startInAppPayment();
				break;
			}
			case IAPAction::RESTORE:
			{
				RoutePaymentSingleton::getInstance()->restorePayment();
				break;
			}
		}
	});
	this->addChild(_productLayout);
	
	_footer = IAPFooter::create();
	_footer->setSizeType(ui::Layout::SizeType::PERCENT);
	_footer->setSizePercent(isPortrait ? Vec2(1.0f, 0.16f) : Vec2(0.5f,0.32f));
	_footer->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_BOTTOM_RIGHT);
	_footer->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_BOTTOM_RIGHT);
	this->addChild(_footer);
	
	_closeButton = ui::Button::create("res/onboarding/close.png");
	_closeButton->setAnchorPoint(Vec2(-0.2,1.2));
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_closeButton->ignoreContentAdaptWithSize(false);
	_closeButton->setContentSize(Size(100, 100));
	_closeButton->setColor(Color3B::WHITE);
	_closeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
            const bool isLoggedIn = UserAccountManager::getInstance()->isLoggedInParentAnonymous() || ChildManager::getInstance()->isChildLoggedIn();
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(isLoggedIn ? SceneNameEnum::Base : SceneNameEnum::ChildSelector));
		}
	});
	this->addChild(_closeButton);
	
	return true;
}
void IAPScene::onEnter()
{
	_marketingCarousel->setPageData(MarketingAssetManager::getInstance()->getMarketingAssets());
	
	_paymentSuccessListener = EventListenerCustom::create(RoutePaymentSingleton::kPaymentSuccessfulEventName, [this](EventCustom* event){
		PaymentSuccessScreen* successScreen = PaymentSuccessScreen::create();
		successScreen->setContinueCallback([](){
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Signup));
		});
		this->addChild(successScreen, 1);
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_paymentSuccessListener, this);
	
	_paymentFailedListener = EventListenerCustom::create(RoutePaymentSingleton::kPaymentFailedEventName, [this](EventCustom* event){
		PopupMessageBox* messageBox = PopupMessageBox::create();
		messageBox->setTitle(_("Oops!\nThat didn't work"));
		messageBox->setBody(_("We couldn't process your payment. Please try again."));
		messageBox->setButtonText(_("Back"));
		messageBox->setButtonColour(Colours::Color_3B::darkIndigo);
		messageBox->setPatternColour(Colours::Color_3B::azure);
		messageBox->setButtonPressedCallback([this](MessagePopupBase* pSender){
			pSender->removeFromParent();
		});
		this->addChild(messageBox, 1);
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_paymentFailedListener, this);
	
	Super::onEnter();
	
	if(RoutePaymentSingleton::getInstance()->receiptDataFileExists())
	{
		if(!UserAccountManager::getInstance()->isUserLoggedIn())
		{
			PaymentSuccessScreen* successScreen = PaymentSuccessScreen::create();
			successScreen->setContinueCallback([](){
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Signup));
			});
			this->addChild(successScreen, 1);
		}
		else
		{
			RoutePaymentSingleton::getInstance()->retryReceiptValidation();
		}
	}
}
void IAPScene::onExit()
{
	if(_paymentSuccessListener)
	{
		_eventDispatcher->removeEventListener(_paymentSuccessListener);
		_paymentSuccessListener = nullptr;
	}
	if(_paymentFailedListener)
	{
		_eventDispatcher->removeEventListener(_paymentFailedListener);
		_paymentFailedListener = nullptr;
	}
	Super::onExit();
}
void IAPScene::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& contentSize = getContentSize();
	bool isPortrait = contentSize.width < contentSize.height;
	
	_marketingCarousel->setSizePercent(isPortrait ? Vec2(1.0f, 0.5f) : Vec2(0.5f,1.0f));
	_marketingCarousel->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_LEFT);
	_marketingCarousel->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_LEFT);
	
	_productLayout->setSizePercent(isPortrait ? Vec2(1.0f, 0.34f) : Vec2(0.5f,0.68f));
	_productLayout->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_TOP_RIGHT);
	_productLayout->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE : Vec2::ANCHOR_TOP_RIGHT);
	
	_footer->setSizePercent(isPortrait ? Vec2(1.0f, 0.16f) : Vec2(0.5f,0.32f));
	_footer->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_BOTTOM_RIGHT);
	_footer->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_BOTTOM_RIGHT);
}

NS_AZOOMEE_END
