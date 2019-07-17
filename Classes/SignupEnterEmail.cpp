//
//  SignupEnterEmail.cpp
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#include "SignupEnterEmail.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SignupEnterEmail::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_topHeading = ui::Text::create(_("We need this to get you up and running. It will only take 2 mins!"), Style::Font::Medium(), 50);
	_topHeading->setTextColor(Color4B(130,130,130,255));
	_topHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_topHeading->setNormalizedPosition(Vec2(0.5,0.975));
	_topHeading->setTextVerticalAlignment(TextVAlignment::TOP);
	_topHeading->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_topHeading->setTextAreaSize(Size(1000,120));
	addChild(_topHeading);
	
	_inputTitle->setString(_("Enter your email address"));
	
	_inputBox->setMaxLength(100);
	_inputBox->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
	
	_progressText->setString(_("First things first"));
	
	_progressBar->setNumberOfSteps(3);
	_progressBar->setProgress(1);
	
	return true;
}

NS_AZOOMEE_END
