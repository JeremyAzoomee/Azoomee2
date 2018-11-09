//
//  VodacomOnboardingDCBWebview.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/11/2018.
//

#include "VodacomOnboardingDCBWebview.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
	_webview->setContentSize(contentSize);
	_webview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_webview->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_webview->loadURL("azoomee://moveto/signup");
	
	_webview->setOnDidFinishLoading([&](experimental::ui::WebView *sender, const std::string &url){
		cocos2d::log("DCB webview finish: %s",url.c_str());
	});
	_webview->setOnDidFailLoading([&](experimental::ui::WebView *sender, const std::string &url){
		cocos2d::log("DCB webview fail: %s",url.c_str());
	});
	_webview->setOnShouldStartLoading([&](experimental::ui::WebView *sender, const std::string &url){
		cocos2d::log("DCB webview start: %s",url.c_str());
		return true;
	});
	
	this->addChild(_webview);
	
	Super::onEnter();
}

NS_AZOOMEE_END
