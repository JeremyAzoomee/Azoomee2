//
//  VodacomOnboardingDCBWebview.h
//  azoomee2
//
//  Created by Macauley on 08/11/2018.
//
#ifdef VODACOM_BUILD
#ifndef VodacomOnboardingDCBWebview_h
#define VodacomOnboardingDCBWebview_h

#include "VodacomOnboardingLayer.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingDCBWebview : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	static const std::string kVodacomStorefrontUrl;
	static const std::string kVodacomPurchaseUrl;
	static const std::string kVodacomPurchaseRedirectUrlStart;
	
	cocos2d::ui::Button* _closeButton = nullptr;
	cocos2d::experimental::ui::WebView* _webview = nullptr;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(VodacomOnboardingDCBWebview);
};

NS_AZOOMEE_END


#endif /* VodacomOnboardingDCBWebview_h */
#endif
