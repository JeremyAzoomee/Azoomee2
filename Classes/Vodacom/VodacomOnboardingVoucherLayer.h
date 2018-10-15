//
//  VodacomOnboardingVoucherLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingVoucherLayer_h
#define VodacomOnboardingVoucherLayer_h

#include "VodacomOnboardingLayer.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>

NS_AZOOMEE_BEGIN

class VodacomOnboardingVoucherLayer : public VodacomOnboardingLayer, HttpRequestCreatorResponseDelegate
{
	typedef VodacomOnboardingLayer Super;
private:
	cocos2d::ui::Button* _closeButton = nullptr;
	cocos2d::ui::Button* _backButton = nullptr;
	cocos2d::ui::Button* _confirmButton = nullptr;
	TextInputLayer* _voucherInput = nullptr;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(VodacomOnboardingVoucherLayer);
	
	//Delegate Functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
};

NS_AZOOMEE_END


#endif /* VodacomOnboardingVoucherLayer_h */
