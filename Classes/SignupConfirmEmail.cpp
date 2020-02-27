//
//  SignupConfirmEmail.cpp
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#include "SignupConfirmEmail.h"
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/Utils/LocaleManager.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool SignupConfirmEmail::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_email = ui::Text::create("", Style::Font::PoppinsBold(), 70);
	_email->setTextColor(Color4B(Colours::Color_3B::strongPink));
	_email->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_email->setNormalizedPosition(Vec2(0.5f,0.2f));
	_email->setTextVerticalAlignment(TextVAlignment::CENTER);
	_email->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_email->setTextAreaSize(Size(1000,240));
	_email->setTouchEnabled(true);
	_email->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_backCallback)
			{
				_backCallback(_email->getString());
			}
		}
	});
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

NS_AZ_END
