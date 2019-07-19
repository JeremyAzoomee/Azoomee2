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
	
	_inputTitle->setString(_("Create a password"));
	
	_inputBox->setMaxLength(50);
	_inputBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	
	_progressText->setString(_("Something safe and secure"));
	
	_progressBar->setNumberOfSteps(3);
	_progressBar->setProgress(2);
	
	_continueButton->setEnabled(isValidPassword(_inputBox->getText(), 6));
	
	return true;
}

void SignupEnterPassword::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
	_continueButton->setEnabled(isValidPassword(text.c_str(), 6));
}
void SignupEnterPassword::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	if(isValidPassword(editBox->getText(), 6))
	{
		if(_continueCallback)
		{
			_continueCallback(editBox->getText());
		}
	}
}
void SignupEnterPassword::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
	_continueButton->setEnabled(isValidPassword(editBox->getText(), 6));
}
void SignupEnterPassword::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	_continueButton->setEnabled(isValidPassword(editBox->getText(), 6));
}

NS_AZOOMEE_END

