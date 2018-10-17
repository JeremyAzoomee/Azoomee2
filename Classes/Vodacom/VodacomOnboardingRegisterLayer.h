//
//  VodacomOnboardingRegisterLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingRegisterLayer_h
#define VodacomOnboardingRegisterLayer_h

#include "VodacomOnboardingLayer.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>

NS_AZOOMEE_BEGIN

class VodacomOnboardingRegisterLayer : public VodacomOnboardingLayer, TextInputLayerDelegate
{
	typedef VodacomOnboardingLayer Super;
private:
	cocos2d::ui::Button* _closeButton = nullptr;
	cocos2d::ui::Button* _backButton = nullptr;
	cocos2d::ui::Button* _confirmButton = nullptr;
	TextInputLayer* _emailInput = nullptr;
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
};

NS_AZOOMEE_END

#endif /* VodacomOnboardingRegisterLayer_h */
