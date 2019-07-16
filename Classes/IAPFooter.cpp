//
//  IAPFooter.cpp
//  Azoomee
//
//  Created by Macauley on 11/07/2019.
//

#include "IAPFooter.h"
#include "SceneManagerScene.h"
#include "LoginLogicHandler.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include <AzoomeeCommon/Data/Urls.h>

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
	
	_bgPattern = Sprite::create("res/decoration/main_pattern_big.png");
	_bgPattern->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_bgPattern->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_bgPattern->setColor(Style::Color::darkIndigo);
	_bgPattern->setOpacity(102);
	addChild(_bgPattern);
	
	_bgGradient = LayerGradient::create(Color4B(255,255,255,204), Color4B(255,255,255,115));
	_bgGradient->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
	addChild(_bgGradient);
	
	_loginButton = ui::Text::create(_("Log In"), Style::Font::Bold(), 70);
	_loginButton->ignoreContentAdaptWithSize(false);
	_loginButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_loginButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_loginButton->setTextColor(Color4B::BLACK);
	_loginButton->setTextVerticalAlignment(TextVAlignment::CENTER);
	_loginButton->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_loginButton->setTouchEnabled(true);
	_loginButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ParentManager::getInstance()->logoutChild();
			
			AudioMixer::getInstance()->stopBackgroundMusic();
			
			LoginLogicHandler::getInstance()->forceNewLogin();
		}
	});
	addChild(_loginButton);
	
	_loginHeader = ui::Text::create(_("Already have an account?"), Style::Font::Regular(), 49);
	_loginHeader->setTextColor(Color4B(130,130,130,255));
	_loginHeader->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_loginHeader->setNormalizedPosition(Vec2(0.5f,1.1f));
	_loginHeader->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_loginHeader->setTextVerticalAlignment(TextVAlignment::BOTTOM);
	Label* loginHeaderLab = dynamic_cast<Label*>(_loginHeader->getVirtualRenderer());
	if(loginHeaderLab)
	{
		loginHeaderLab->setOverflow(Label::Overflow::RESIZE_HEIGHT);
	}
	_loginButton->addChild(_loginHeader);
	
	_termsLink = ui::Text::create(_("Terms of use"), Style::Font::Regular(), 35);
	_termsLink->ignoreContentAdaptWithSize(false);
	_termsLink->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_termsLink->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
	_termsLink->setTextColor(Color4B(130,130,130,255));
	_termsLink->setTextVerticalAlignment(TextVAlignment::CENTER);
	_termsLink->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_termsLink->setTouchEnabled(true);
	_termsLink->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalWebview::createWithURL(Url::TermsOfUse);
		}
	});
	Label* termsLinkLab = dynamic_cast<Label*>(_termsLink->getVirtualRenderer());
	if(termsLinkLab)
	{
		termsLinkLab->setOverflow(Label::Overflow::SHRINK);
	}
	addChild(_termsLink);
	
	_privacyPolicyLink = ui::Text::create(_("Privacy Policy"), Style::Font::Regular(), 35);
	_privacyPolicyLink->ignoreContentAdaptWithSize(false);
	_privacyPolicyLink->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	_privacyPolicyLink->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
	_privacyPolicyLink->setTextColor(Color4B(130,130,130,255));
	_privacyPolicyLink->setTextVerticalAlignment(TextVAlignment::CENTER);
	_privacyPolicyLink->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_privacyPolicyLink->setTouchEnabled(true);
	_privacyPolicyLink->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalWebview::createWithURL(Url::PrivacyPolicyNoLinks);
		}
	});
	Label* privacyPolicyLab = dynamic_cast<Label*>(_privacyPolicyLink->getVirtualRenderer());
	if(privacyPolicyLab)
	{
		privacyPolicyLab->setOverflow(Label::Overflow::SHRINK);
	}
	addChild(_privacyPolicyLink);
	
	_privacyNoticeLink = ui::Text::create(_("Important Notice for Parents"), Style::Font::Regular(), 35);
	_privacyNoticeLink->ignoreContentAdaptWithSize(false);
	_privacyNoticeLink->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_privacyNoticeLink->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_privacyNoticeLink->setTextColor(Color4B(130,130,130,255));
	_privacyNoticeLink->setTextVerticalAlignment(TextVAlignment::CENTER);
	_privacyNoticeLink->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_privacyNoticeLink->setTouchEnabled(true);
	_privacyNoticeLink->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalWebview::createWithURL(Url::PrivacyPolicyNoLinks);
		}
	});
	Label* privacyNoticeLab = dynamic_cast<Label*>(_privacyNoticeLink->getVirtualRenderer());
	if(privacyNoticeLab)
	{
		privacyNoticeLab->setOverflow(Label::Overflow::SHRINK);
	}
	addChild(_privacyNoticeLink);
	
	_divider = ui::Layout::create();
	_divider->setBackGroundColorType(BackGroundColorType::SOLID);
	_divider->setBackGroundColor(Color3B(216,216,216));
	_divider->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_divider->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
	addChild(_divider);
	
	return true;
}
void IAPFooter::onEnter()
{
	Super::onEnter();
}
void IAPFooter::onExit()
{
	Super::onExit();
}
void IAPFooter::onSizeChanged()
{
	Super::onSizeChanged();
	_bgGradient->setContentSize(Size(_bgPattern->getContentSize().width,this->getContentSize().height));
	_divider->setContentSize(Size(this->getContentSize().width,4));
	Label* loginHeaderLab = dynamic_cast<Label*>(_loginHeader->getVirtualRenderer());
	if(loginHeaderLab)
	{
		loginHeaderLab->setMaxLineWidth(this->getContentSize().width * 0.7f);
	}
	
	_termsLink->setTextAreaSize(Size(this->getContentSize().width * 0.25f, _termsLink->getFontSize() * 2.0f));
	_privacyPolicyLink->setTextAreaSize(Size(this->getContentSize().width * 0.25f, _privacyPolicyLink->getFontSize() * 2.0f));
	_privacyNoticeLink->setTextAreaSize(Size(this->getContentSize().width * 0.4f, _privacyPolicyLink->getFontSize() * 2.0f));
}

NS_AZOOMEE_END
