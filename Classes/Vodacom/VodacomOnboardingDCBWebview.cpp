//
//  VodacomOnboardingDCBWebview.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/11/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomOnboardingDCBWebview.h"
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include "../BackEndCaller.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

#ifdef AZOOMEE_ENVIRONMENT_CI
const std::string VodacomOnboardingDCBWebview::kVodacomStorefrontUrl = "http://vodacom.azoomeepartners.ninja/purchase";
const std::string VodacomOnboardingDCBWebview::kVodacomPurchaseUrl = "http://vodacom.azoomeepartners.ninja/purchase?txnId=%s&productId=%s";
const std::string VodacomOnboardingDCBWebview::kVodacomPurchaseRedirectUrlStart = "http://vodacom.azoomeepartners.ninja/purchase-redirect";
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
            const std::vector<std::string>& splitByAzoomeeVector = StringFunctions::splitStringToVector(StringFunctions::stringToLower(uriString), "azoomee://");
               
            if(splitByAzoomeeVector.size() == 0 || splitByAzoomeeVector.size() > 2)
            {
                return false;
            }
               
            std::string uriStringWhole = splitByAzoomeeVector.at(0);
               
            if(splitByAzoomeVector.size() == 2)
            {
                uriStringWhole = splitByAzoomeeVector.at(1);
            }
               
            const std::vector<std::string>&  splitByForwardSlash = StringFunctions::splitStringToVector(uriStringWhole, "/");
               
            if(splitByForwardSlash.size() != 2)
            {
                return false;
            }
               
            const std::string& host = splitByForwardSlash.at(0);
            const std::string& path = splitByForwardSlash.at(1);
			if(host == "vodacom")
            {
                VodacomOnboardingScene* vodacomScene = dynamic_cast<VodacomOnboardingScene*>(Director::getInstance()->getRunningScene());
                if(vodacomScene)
                {
                    vodacomScene->moveToStateDCBProductSelected(path);
                    return true;
                }
            }
		}
		else if(url.find(kVodacomPurchaseRedirectUrlStart) != url.npos)
		{
			if(UserAccountManager::getInstance()->isUserLoggedIn())
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
#endif
