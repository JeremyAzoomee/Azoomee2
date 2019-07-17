//
//  SignupEnterPassword.cpp
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#include "SignupEnterPassword.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>

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
	
	return true;
}

NS_AZOOMEE_END

