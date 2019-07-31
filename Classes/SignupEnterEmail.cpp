//
//  SignupEnterEmail.cpp
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#include "SignupEnterEmail.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SignupEnterEmail::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_topHeading->setString(_("We need this to get you up and running. It will only take 2 mins!"));
	
	_inputTitle->setString(_("Enter your email address"));
	
	_inputBox->setMaxLength(100);
	_inputBox->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
	
	_progressText->setString(_("First thing's first"));
	
	_progressBar->setNumberOfSteps(3);
	_progressBar->setProgress(1);
	
	setContinueButtonEnabled(isValidEmailAddress(_inputBox->getText()));
	
	return true;
}

void SignupEnterEmail::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
	setContinueButtonEnabled(isValidEmailAddress(text.c_str()));
}
void SignupEnterEmail::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	
}
void SignupEnterEmail::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(isValidEmailAddress(editBox->getText()));
}
void SignupEnterEmail::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(isValidEmailAddress(editBox->getText()));
}

NS_AZOOMEE_END
