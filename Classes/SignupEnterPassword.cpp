//
//  SignupEnterPassword.cpp
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#include "SignupEnterPassword.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SignupEnterPassword::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_topHeading->setString(_("Heads up! Make sure your password is atleast six charecters long."));
	
	_inputTitle->setString(_("Create a password"));
	
	_inputBox->setMaxLength(50);
	_inputBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	
	_progressText->setString(_("Something safe and secure"));
	
	_progressBar->setNumberOfSteps(3);
	_progressBar->setProgress(2);
	
	setContinueButtonEnabled(isValidPassword(_inputBox->getText(), 6));
	
	return true;
}

void SignupEnterPassword::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
	setContinueButtonEnabled(isValidPassword(text.c_str(), 6));
}
void SignupEnterPassword::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	
}
void SignupEnterPassword::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(isValidPassword(editBox->getText(), 6));
}
void SignupEnterPassword::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(isValidPassword(editBox->getText(), 6));
}

NS_AZOOMEE_END

