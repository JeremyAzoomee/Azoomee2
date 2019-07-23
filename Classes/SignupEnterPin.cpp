

//
//  SignupEnterPin.cpp
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#include "SignupEnterPin.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SignupEnterPin::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_inputTitle->setString(_("Set up a 4-digit pin"));
	
	_inputBg->setContentSize(Size(700, _inputBg->getContentSize().height));
	_inputBox->setContentSize(_inputBg->getContentSize() * 0.8f);
	_inputBox->setMaxLength(4);
	_inputBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	_inputBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
	
	_progressText->setString(_("Dont forget this!"));
	
	_progressBar->setNumberOfSteps(3);
	_progressBar->setProgress(3);
	
	setContinueButtonEnabled(isValidPin(_inputBox->getText()));
	
	return true;
}

void SignupEnterPin::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
	setContinueButtonEnabled(isValidPin(text.c_str()));
}
void SignupEnterPin::editBoxReturn(cocos2d::ui::EditBox* editBox)
{

}
void SignupEnterPin::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(isValidPin(editBox->getText()));
}
void SignupEnterPin::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(isValidPin(editBox->getText()));
}

NS_AZOOMEE_END
