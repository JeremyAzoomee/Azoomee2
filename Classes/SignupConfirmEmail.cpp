//
//  SignupConfirmEmail.cpp
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#include "SignupConfirmEmail.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SignupConfirmEmail::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_email = ui::Text::create("", Style::Font::Bold(), 70);
	_email->setTextColor(Color4B(Style::Color::strongPink));
	_email->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_email->setNormalizedPosition(Vec2(0.5f,0.0f));
	_email->setTextVerticalAlignment(TextVAlignment::CENTER);
	_email->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_email->setTextAreaSize(Size(1000,240));
	_inputTitle->addChild(_email);
	
	_inputTitle->setString(_("Confirm your email address"));
	
	_inputBox->setVisible(false);
	
	_progressText->setString(_("No harm in double checking"));
	
	_progressBar->setNumberOfSteps(3);
	_progressBar->setProgress(1);
	
	return true;
}

void SignupConfirmEmail::setEmail(const std::string &email)
{
	_email->setString(email);
}

NS_AZOOMEE_END
