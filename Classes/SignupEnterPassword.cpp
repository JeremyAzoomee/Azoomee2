//
//  SignupEnterPassword.cpp
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#include "SignupEnterPassword.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool SignupEnterPassword::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_topHeading->setString(_("Make sure your password is at least six characters long."));
	
	_inputTitle->setString(_("Create a password"));
	
	_inputBox->setMaxLength(50);
	_inputBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	
	_progressText->setString(_("Something safe and secure"));
	
	_progressBar->setNumberOfSteps(3);
	_progressBar->setProgress(2);
	
	setContinueButtonEnabled(TextInputChecker::isValidPassword(_inputBox->getText(), TextInputChecker::kMinNewPasswordLength));
	
	return true;
}

void SignupEnterPassword::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
	setContinueButtonEnabled(TextInputChecker::isValidPassword(text.c_str(), TextInputChecker::kMinNewPasswordLength));
}
void SignupEnterPassword::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	
}
void SignupEnterPassword::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(TextInputChecker::isValidPassword(editBox->getText(), TextInputChecker::kMinNewPasswordLength));
}
void SignupEnterPassword::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(TextInputChecker::isValidPassword(editBox->getText(), TextInputChecker::kMinNewPasswordLength));
}

NS_AZ_END

