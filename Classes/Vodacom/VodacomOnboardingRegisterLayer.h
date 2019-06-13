//
//  VodacomOnboardingRegisterLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#ifndef VodacomOnboardingRegisterLayer_h
#define VodacomOnboardingRegisterLayer_h

#include "VodacomOnboardingLayer.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include "../SettingsMessageBox.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingRegisterLayer : public VodacomOnboardingLayer, TextInputLayerDelegate, SettingsMessageBoxDelegate
{
	typedef VodacomOnboardingLayer Super;
private:
	cocos2d::ui::Button* _closeButton = nullptr;
	cocos2d::ui::Button* _backButton = nullptr;
	cocos2d::ui::Button* _confirmButton = nullptr;
	TextInputLayer* _emailInput = nullptr;
	TextInputLayer* _confirmEmailInput = nullptr;
	TextInputLayer* _passwordInput = nullptr;
	
	void onConfirmPressed();
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(VodacomOnboardingRegisterLayer);
	
	//Delegate functions
	virtual void textInputIsValid(TextInputLayer* inputLayer, bool isValid) override;
	virtual void textInputReturnPressed(TextInputLayer* inputLayer) override;
	virtual void editBoxEditingDidBegin(TextInputLayer* inputLayer) override;
	virtual void editBoxEditingDidEnd(TextInputLayer* inputLayer) override;
	
	virtual void onButtonPressed(SettingsMessageBox* pSender, SettingsMessageBoxButtonType type) override;
};

NS_AZOOMEE_END

#endif /* VodacomOnboardingRegisterLayer_h */
#endif
