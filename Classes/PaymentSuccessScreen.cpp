//
//  PaymentSuccessScreen.cpp
//  Azoomee
//
//  Created by Macauley on 25/07/2019.
//

#include "PaymentSuccessScreen.h"
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "LoginController.h"

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
	
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f,1.0f));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    
	setBackGroundColorType(BackGroundColorType::SOLID);
	setBackGroundColor(Style::Color::darkIndigo);
	
    createBackground();
    createBody();
    createLoginButton();
	
	return true;
}

void PaymentSuccessScreen::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& contentSize = Director::getInstance()->getVisibleSize();
	bool isPortrait = contentSize.width < contentSize.height;
    
	if(_gradient)
	{
		_gradient->setContentSize(contentSize);
		_gradient->setEndOpacity(isPortrait ? 140 : 180);
	}
}

void PaymentSuccessScreen::createBackground()
{
    const Size& contentSize = Director::getInstance()->getVisibleSize();
    bool isPortrait = contentSize.width < contentSize.height;
    
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
    
}
void PaymentSuccessScreen::createBody()
{
    _bodyText = DynamicText::create(_("Create an Azoomee account to get started"), Style::Font::PoppinsBold(), 50);
    _bodyText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _bodyText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _bodyText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bodyText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bodyText->setTextColor(Color4B::WHITE);
    _bodyText->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    _bodyText->setMaxLineWidth(872);
    addChild(_bodyText);
    
    _headerText = DynamicText::create(_("Payment successful!"), Style::Font::PoppinsBold(), 105);
    _headerText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _headerText->setNormalizedPosition(Vec2(0.5f,1.2f));
    _headerText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _headerText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _headerText->setTextColor(Color4B::WHITE);
    _headerText->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    _headerText->setMaxLineWidth(1200);
    
    _bodyText->addChild(_headerText);
    
    _continueButton = TextButton::create("res/onboarding/rounded_button.png");
    _continueButton->setContentSize(Size(700,140));
    _continueButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _continueButton->setNormalizedPosition(Vec2(0.5f, -0.3f));
    _continueButton->ignoreContentAdaptWithSize(false);
    _continueButton->setText(_("Continue"));
    _continueButton->setTextFontInfo(Style::Font::PoppinsBold(), 70);
    _continueButton->setTextColour(Color4B::BLACK);
    _continueButton->setTextAreaSizePercent(Vec2(0.8f,0.8f));
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
    
}
void PaymentSuccessScreen::createLoginButton()
{
    _loginButton = DynamicText::create(_("Log In"), Style::Font::PoppinsBold(), 70);
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
            UserAccountManager::getInstance()->logoutChild();
            
            AudioMixer::getInstance()->stopBackgroundMusic();
            
            LoginController::getInstance()->forceNewLogin(LoginOrigin::IAP_PAYWALL);
        }
    });
    addChild(_loginButton);
    
    _loginHeader = DynamicText::create(_("Already have an account?"), Style::Font::PoppinsRegular(), 49);
    _loginHeader->setTextColor(Color4B::WHITE);
    _loginHeader->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _loginHeader->setNormalizedPosition(Vec2(0.5f,1.1f));
    _loginHeader->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _loginHeader->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    _loginHeader->setMaxLineWidth(772);
    _loginHeader->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    _loginButton->addChild(_loginHeader);
}

void PaymentSuccessScreen::setContinueCallback(const ContinueCallback& callback)
{
	_callback = callback;
}

NS_AZOOMEE_END
