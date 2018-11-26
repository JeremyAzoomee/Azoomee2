//
//  VodacomOnboardingDCBWebview.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/11/2018.
//

#include "VodacomOnboardingDCBWebview.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "../DeepLinkingSingleton.h"
#include "../BackEndCaller.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

#ifdef USINGCI
const std::string VodacomOnboardingDCBWebview::kVodacomStorefrontUrl = "http://vodacom.azoomeepartners.ninja/purchase";
const std::string VodacomOnboardingDCBWebview::kVodacomPurchaseUrl = "http://vodacom.azoomeepartners.ninja/purchase?txnId=%s&productId=%s";
const std::string VodacomOnboardingDCBWebview::kVodacomPurchaseRedirectUrlStart = "http://vodacom.azoomeepartners.ninja/purchase-redirect";
//const std::string VodacomOnboardingDCBWebview::kVodacomStorefrontUrl = "http://b8867b46.ngrok.io/purchase?on-network=true";
//const std::string VodacomOnboardingDCBWebview::kVodacomPurchaseUrl = "http://b8867b46.ngrok.io/purchase?txnId=%s&productId=%s?on-network=true";
#else
const std::string VodacomOnboardingDCBWebview::kVodacomStorefrontUrl = "http://vodacom.azoomeepartners.com/purchase";
const std::string VodacomOnboardingDCBWebview::kVodacomPurchaseUrl = "http://vodacom.azoomeepartners.com/purchase?txnId=%s&productId=%s";
const std::string VodacomOnboardingDCBWebview::kVodacomPurchaseRedirectUrlStart = "http://vodacom.azoomeepartners.com/purchase-redirect";
#endif

bool VodacomOnboardingDCBWebview::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void VodacomOnboardingDCBWebview::onEnter()
{
	const Size& contentSize = this->getContentSize();
	
	_webview = experimental::ui::WebView::create();
	_webview->setContentSize(contentSize * 0.8f);
	_webview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_webview->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	_webview->setOnDidFinishLoading([&](experimental::ui::WebView *sender, const std::string &url){
		cocos2d::log("DCB webview finish: %s",url.c_str());
	});
	_webview->setOnDidFailLoading([&](experimental::ui::WebView *sender, const std::string &url){
		cocos2d::log("DCB webview fail: %s",url.c_str());
		if(url.find("azoomee://") == url.npos) //failed url isnt deeplink, deeplink from storefront will trigger failed url load in webview before its processed by DeeplinkingSingleton
		{
			_flowData->setErrorType(ErrorType::GENERIC);
			if(_delegate)
			{
				_delegate->moveToState(FlowState::ERROR);
			}
		}
	});
	_webview->setOnShouldStartLoading([&](experimental::ui::WebView *sender, const std::string &url){
		cocos2d::log("DCB webview start: %s",url.c_str());
		if(url.find("azoomee://") != url.npos) // manually fire deeplink
		{
			_webview->setVisible(false);
			DeepLinkingSingleton::getInstance()->setDeepLink(url);
		}
		else if(url.find(kVodacomPurchaseRedirectUrlStart) != url.npos)
		{
			if(ParentDataProvider::getInstance()->isUserLoggedIn())
			{
				_flowData->setDCBCompete(true);
				BackEndCaller::getInstance()->updateBillingData();
			}
		}
		return true;
	});

	
	this->addChild(_webview);
	
	_closeButton = ui::Button::create("res/vodacom/close.png");
	_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::EXIT);
			}
		}
	});
	
	this->addChild(_closeButton);
	
	if(_flowData->getProductId() == "" || (_flowData->getUserType() == UserType::ANON || _flowData->getUserType() == UserType::ANON_FREE))
	{
		_webview->loadURL(kVodacomStorefrontUrl);
	}
	else if(_flowData->getTransactionId() != "")
	{
		_webview->loadURL(StringUtils::format(kVodacomPurchaseUrl.c_str(), _flowData->getTransactionId().c_str(), _flowData->getProductId().c_str()));
	}
	
	Super::onEnter();
}

NS_AZOOMEE_END
