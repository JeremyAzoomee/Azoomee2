//
//  VodacomOnboardingErrorLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingErrorLayer_h
#define VodacomOnboardingErrorLayer_h

#include "VodacomOnboardingLayer.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>

NS_AZOOMEE_BEGIN

class VodacomOnboardingErrorLayer : public VodacomOnboardingLayer, HttpRequestCreatorResponseDelegate
{
	typedef VodacomOnboardingLayer Super;
private:
	// Voucher UI
	cocos2d::ui::Button* _confirmButton = nullptr;
	TextInputLayer* _voucherInput = nullptr;
	
	// Login UI
	
	
	// Password reset UI
	
	
	// Already registered UI
	
	
	// Already premium UI
	
	void setupForLoginError();
	void setupForVoucherError();
	void setupForPasswordReset();
	void setupForAlreadyPremium();
	void setupForAlreadyRegistered();
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(VodacomOnboardingErrorLayer);
	
	//Delegate Functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
};

NS_AZOOMEE_END

#endif /* VodacomOnboardingErrorLayer_h */
