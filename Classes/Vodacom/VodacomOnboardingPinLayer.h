//
//  VodacomOnboardingPinLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#ifndef VodacomOnboardingPinLayer_h
#define VodacomOnboardingPinLayer_h

#include "VodacomOnboardingLayer.h"
#include "../TextInputLayer.h"
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include "../SettingsMessageBox.h"
#include "../Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class VodacomOnboardingPinLayer : public VodacomOnboardingLayer, HttpRequestCreatorResponseDelegate, TextInputLayerDelegate, SettingsMessageBoxDelegate
{
	typedef VodacomOnboardingLayer Super;
private:
	cocos2d::ui::Button* _closeButton = nullptr;
	cocos2d::ui::Button* _backButton = nullptr;
	cocos2d::ui::Button* _confirmButton = nullptr;
	TextInputLayer* _pinInput = nullptr;
	
	void onConfirmPressed();
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(VodacomOnboardingPinLayer);
	
	//Delegate Functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
	
	virtual void textInputIsValid(TextInputLayer* inputLayer, bool isValid) override;
	virtual void textInputReturnPressed(TextInputLayer* inputLayer) override;
	virtual void editBoxEditingDidBegin(TextInputLayer* inputLayer) override;
	virtual void editBoxEditingDidEnd(TextInputLayer* inputLayer) override;
	
	virtual void onButtonPressed(SettingsMessageBox* pSender, SettingsMessageBoxButtonType type) override;
};

NS_AZ_END


#endif /* VodacomOnboardingPinLayer_h */
#endif
