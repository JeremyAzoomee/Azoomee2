//
//  PaymentSuccessScreen.cpp
//  Azoomee
//
//  Created by Macauley on 25/07/2019.
//

#include "PaymentSuccessScreen.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "LoginLogicHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string PaymentSuccessScreen::kPaymentSuccessScreenName = "paymentSuccessScreen";

bool PaymentSuccessScreen::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setName(kPaymentSuccessScreenName);
	
	const Size& contentSize = Director::getInstance()->getVisibleSize();
	bool isPortrait = contentSize.width < contentSize.height;
	setContentSize(contentSize);
	setBackGroundColorType(BackGroundColorType::SOLID);
	setBackGroundColor(Style::Color::darkIndigo);
	
	_pattern = Sprite::create("res/decoration/main_pattern_big.png");
	_pattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_pattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_pattern->setColor(Style::Color::strongPink);
	addChild(_pattern);
	
	_gradient = LayerGradient::create();
	_gradient->setContentSize(contentSize);
	_gradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_gradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_gradient->setStartColor(Style::Color::darkIndigo);
	_gradient->setStartOpacity(0);
	_gradient->setEndColor(Style::Color::darkIndigo);
	_gradient->setEndOpacity(isPortrait ? 140 : 180);
	_gradient->setIgnoreAnchorPointForPosition(false);
	addChild(_gradient);
	
	_bodyText = ui::Text::create(_("Create an Azoomee account to get started"), Style::Font::PoppinsBold(), 50);
	_bodyText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_bodyText->setTextVerticalAlignment(TextVAlignment::CENTER);
	_bodyText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_bodyText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_bodyText->setTextColor(Color4B::WHITE);
	Label* bodyLab = dynamic_cast<Label*>(_bodyText->getVirtualRenderer());
	if(bodyLab)
	{
		bodyLab->setOverflow(Label::Overflow::RESIZE_HEIGHT);
		bodyLab->setMaxLineWidth(872);
	}
	addChild(_bodyText);
	
	_headerText = ui::Text::create(_("Payment successful!"), Style::Font::PoppinsBold(), 105);
	_headerText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_headerText->setNormalizedPosition(Vec2(0.5f,1.2f));
	_headerText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_headerText->setTextVerticalAlignment(TextVAlignment::CENTER);
	_headerText->setTextColor(Color4B::WHITE);
	Label* headerLab = dynamic_cast<Label*>(_headerText->getVirtualRenderer());
	if(headerLab)
	{
		headerLab->setOverflow(Label::Overflow::RESIZE_HEIGHT);
		headerLab->setMaxLineWidth(1200);
	}
	_bodyText->addChild(_headerText);
	
	_continueButton = ui::Button::create("res/onboarding/rounded_button.png");
	_continueButton->setContentSize(Size(700,140));
	_continueButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_continueButton->setNormalizedPosition(Vec2(0.5f, -0.3f));
	_continueButton->ignoreContentAdaptWithSize(false);
	_continueButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_callback)
			{
				_callback();
			}
		}
	});
	_bodyText->addChild(_continueButton);
	
	Label* buttonText = Label::createWithTTF(_("Continue"), Style::Font::PoppinsBold(), 70);
	buttonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	buttonText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	buttonText->setAlignment(TextHAlignment::CENTER,TextVAlignment::CENTER);
	buttonText->setTextColor(Color4B::BLACK);
	buttonText->setDimensions(_continueButton->getContentSize().width * 0.8f, _continueButton->getContentSize().height * 0.8f);
	buttonText->setOverflow(Label::Overflow::SHRINK);
	_continueButton->addChild(buttonText);
	
	_loginButton = ui::Text::create(_("Log In"), Style::Font::PoppinsBold(), 70);
	_loginButton->ignoreContentAdaptWithSize(false);
	_loginButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_loginButton->setNormalizedPosition(Vec2(0.5f,0.1f));
	_loginButton->setTextColor(Color4B::WHITE);
	_loginButton->setTextVerticalAlignment(TextVAlignment::CENTER);
	_loginButton->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_loginButton->setTouchEnabled(true);
	_loginButton->setTouchScaleChangeEnabled(true);
	_loginButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ParentManager::getInstance()->logoutChild();
			
			AudioMixer::getInstance()->stopBackgroundMusic();
			
			LoginLogicHandler::getInstance()->forceNewLogin();
		}
	});
	addChild(_loginButton);
	
	_loginHeader = ui::Text::create(_("Already have an account?"), Style::Font::PoppinsRegular(), 49);
	_loginHeader->setTextColor(Color4B::WHITE);
	_loginHeader->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_loginHeader->setNormalizedPosition(Vec2(0.5f,1.1f));
	_loginHeader->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_loginHeader->setTextVerticalAlignment(TextVAlignment::BOTTOM);
	Label* loginHeaderLab = dynamic_cast<Label*>(_loginHeader->getVirtualRenderer());
	if(loginHeaderLab)
	{
		loginHeaderLab->setMaxLineWidth(772);
		loginHeaderLab->setOverflow(Label::Overflow::RESIZE_HEIGHT);
	}
	_loginButton->addChild(_loginHeader);
	
	return true;
}

void PaymentSuccessScreen::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& contentSize = Director::getInstance()->getVisibleSize();
	bool isPortrait = contentSize.width < contentSize.height;
	setContentSize(contentSize);
	if(_gradient)
	{
		_gradient->setContentSize(contentSize);
		_gradient->setEndOpacity(isPortrait ? 140 : 180);
	}
}

void PaymentSuccessScreen::setContinueCallback(const ContinueCallback& callback)
{
	_callback = callback;
}

NS_AZOOMEE_END
