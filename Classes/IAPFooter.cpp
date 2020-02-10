//
//  IAPFooter.cpp
//  Azoomee
//
//  Created by Macauley on 11/07/2019.
//

#include "IAPFooter.h"
#include "SceneManagerScene.h"
#include "LoginController.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include "Urls.h"
#include "ImportantMessageForParents.h"
#include "Style.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool IAPFooter::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setBackGroundColorType(BackGroundColorType::SOLID);
	setBackGroundColor(Color3B::WHITE);
	setClippingEnabled(true);
	
    createBackroundElements();
    createLoginButton();
    createTermsLinks();
	
	return true;
}
void IAPFooter::onEnter()
{
	_loginButton->setVisible(UserAccountManager::getInstance()->isLoggedInParentAnonymous());
	Super::onEnter();
}
void IAPFooter::onExit()
{
	Super::onExit();
}
void IAPFooter::onSizeChanged()
{
	Super::onSizeChanged();
	_bgGradient->setContentSize(Size(this->getContentSize().width,this->getContentSize().height));
	_divider->setContentSize(Size(this->getContentSize().width,4));
	_loginHeader->setMaxLineWidth(this->getContentSize().width * 0.7f);
	
	_termsLink->setTextAreaSize(Size(this->getContentSize().width * 0.25f, _termsLink->getFontSize() * 2.0f));
	_privacyPolicyLink->setTextAreaSize(Size(this->getContentSize().width * 0.25f, _privacyPolicyLink->getFontSize() * 2.0f));
	_privacyNoticeLink->setTextAreaSize(Size(this->getContentSize().width * 0.4f, _privacyPolicyLink->getFontSize() * 2.0f));
}

void IAPFooter::createLoginButton()
{
    _loginButton = DynamicText::create(_("Log In"), Style::Font::PoppinsBold(), 70);
    _loginButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loginButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _loginButton->setTextColor(Color4B::BLACK);
    _loginButton->setTextVerticalAlignment(TextVAlignment::CENTER);
    _loginButton->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _loginButton->setTouchEnabled(true);
    _loginButton->setTouchScaleChangeEnabled(true);
    _loginButton->setOverflow(Label::Overflow::RESIZE_HEIGHT);
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
    _loginHeader->setTextColor(Color4B(Colours::Color_3B::brownGrey));
    _loginHeader->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _loginHeader->setNormalizedPosition(Vec2(0.5f,1.1f));
    _loginHeader->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _loginHeader->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    _loginHeader->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    
    _loginButton->addChild(_loginHeader);
    
}
void IAPFooter::createTermsLinks()
{
    _termsLink = DynamicText::create(_("Terms of use"), Style::Font::PoppinsRegular(), 35);
    _termsLink->ignoreContentAdaptWithSize(false);
    _termsLink->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _termsLink->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    _termsLink->setTextColor(Color4B(Colours::Color_3B::brownGrey));
    _termsLink->setTextVerticalAlignment(TextVAlignment::CENTER);
    _termsLink->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _termsLink->setTouchEnabled(true);
    _termsLink->setTouchScaleChangeEnabled(true);
    _termsLink->setOverflow(Label::Overflow::SHRINK);
    _termsLink->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            ModalWebview::createWithURL(Url::TermsOfUse);
        }
    });
    addChild(_termsLink);
    
    _privacyPolicyLink = DynamicText::create(_("Privacy Policy"), Style::Font::PoppinsRegular(), 35);
    _privacyPolicyLink->ignoreContentAdaptWithSize(false);
    _privacyPolicyLink->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _privacyPolicyLink->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
    _privacyPolicyLink->setTextColor(Color4B(Colours::Color_3B::brownGrey));
    _privacyPolicyLink->setTextVerticalAlignment(TextVAlignment::CENTER);
    _privacyPolicyLink->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _privacyPolicyLink->setTouchEnabled(true);
    _privacyPolicyLink->setTouchScaleChangeEnabled(true);
    _privacyPolicyLink->setOverflow(Label::Overflow::SHRINK);
    _privacyPolicyLink->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            ModalWebview::createWithURL(Url::PrivacyPolicyNoLinks);
        }
    });
    addChild(_privacyPolicyLink);
    
    _privacyNoticeLink = DynamicText::create(_("Important Notice for Parents"), Style::Font::PoppinsRegular(), 35);
    _privacyNoticeLink->ignoreContentAdaptWithSize(false);
    _privacyNoticeLink->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _privacyNoticeLink->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _privacyNoticeLink->setTextColor(Color4B(Colours::Color_3B::brownGrey));
    _privacyNoticeLink->setTextVerticalAlignment(TextVAlignment::CENTER);
    _privacyNoticeLink->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _privacyNoticeLink->setTouchEnabled(true);
    _privacyNoticeLink->setTouchScaleChangeEnabled(true);
    _privacyNoticeLink->setOverflow(Label::Overflow::SHRINK);
    _privacyNoticeLink->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->getRunningScene()->addChild(ImportantMessageForParents::create());
        }
    });
    addChild(_privacyNoticeLink);
    
}
void IAPFooter::createBackroundElements()
{
    Sprite* bgPattern = Sprite::create("res/decoration/main_pattern_big.png");
    bgPattern->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    bgPattern->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    bgPattern->setColor(Colours::Color_3B::darkIndigo);
    bgPattern->setOpacity(102);
    addChild(bgPattern);
    
    _bgGradient = LayerGradient::create(Color4B(255,255,255,204), Color4B(255,255,255,115));
    _bgGradient->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(_bgGradient);
    
    _divider = ui::Layout::create();
    _divider->setBackGroundColorType(BackGroundColorType::SOLID);
    _divider->setBackGroundColor(Color3B(216,216,216));
    _divider->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _divider->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    addChild(_divider);
}

NS_AZOOMEE_END
