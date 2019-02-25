//
//  VodacomOnboardingAddChildLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//
#ifdef VODACOM_BUILD
#ifndef VodacomOnboardingAddChildLayer_h
#define VodacomOnboardingAddChildLayer_h

#include "VodacomOnboardingLayer.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include "../SettingsMessageBox.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingAddChildLayer : public VodacomOnboardingLayer, HttpRequestCreatorResponseDelegate, TextInputLayerDelegate, SettingsMessageBoxDelegate
{
	typedef VodacomOnboardingLayer Super;
private:
	cocos2d::ui::Button* _closeButton = nullptr;
	cocos2d::ui::Button* _backButton = nullptr;
	cocos2d::ui::Button* _confirmButton = nullptr;
	TextInputLayer* _nameInput = nullptr;
	TextInputLayer* _ageInput = nullptr;
	
	void onConfirmPressed();
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(VodacomOnboardingAddChildLayer);
	
	//Delegate functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
	
	virtual void textInputIsValid(TextInputLayer* inputLayer, bool isValid) override;
	virtual void textInputReturnPressed(TextInputLayer* inputLayer) override;
	virtual void editBoxEditingDidBegin(TextInputLayer* inputLayer) override;
	virtual void editBoxEditingDidEnd(TextInputLayer* inputLayer) override;
	
	virtual void onButtonPressed(SettingsMessageBox* pSender, SettingsMessageBoxButtonType type) override;
};

NS_AZOOMEE_END


#endif /* VodacomOnboardingAddChildLayer_h */
#endif
