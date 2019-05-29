//
//  UserTypeMessagingLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 26/04/2018.
//

#include "UserTypeMessagingLayer.h"
#include "DynamicNodeHandler.h"
#include "SceneManagerScene.h"
#include "IAPProductDataHandler.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool UserTypeMessagingLayer::init()
{
    if(!Super::init())
    {
        return false;
    }
	setCascadeOpacityEnabled(true);
    return true;
}

void UserTypeMessagingLayer::onEnter()
{
    bool isPortrait = Director::getInstance()->getVisibleSize().width < Director::getInstance()->getVisibleSize().height;
    bool is18x9 = ConfigStorage::getInstance()->isDevice18x9();
    Super::onEnter();
    
    _bgSprite = ui::Scale9Sprite::create("res/artapp/white_bg.png");
    _bgSprite->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bgSprite->setContentSize(this->getContentSize());
    _bgSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bgSprite->setColor(Color3B(246,187,66));
    this->addChild(_bgSprite);
    
    _startTrialButton = ui::Button::create("res/buttons/MainButton.png");
    _startTrialButton->setContentSize(Size(this->getContentSize().width * (isPortrait ? 0.65f : 0.5f), _startTrialButton->getContentSize().height));
    _startTrialButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
#ifdef VODACOM_BUILD
    _startTrialButton->setNormalizedPosition(Vec2(isPortrait ? 0.35 : 0.5, 0.5));
#else
	_startTrialButton->setNormalizedPosition(Vec2(isPortrait ? 0.35 : 0.5, 0.4));
#endif
    _startTrialButton->setSwallowTouches(true);
    _startTrialButton->setColor(Color3B::WHITE);
    _startTrialButton->ignoreContentAdaptWithSize(true);
    _startTrialButton->setScale9Enabled(true);
    _startTrialButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
#ifdef ALLOW_UNPAID_SIGNUP
			DynamicNodeHandler::getInstance()->startSignupFlow();
#else
            DynamicNodeHandler::getInstance()->startIAPFlow();
#endif
        }
    });
	
#ifdef VODACOM_BUILD
	_startTrialLabel = Label::createWithTTF(_("Unlock everything"), Style::Font::Regular(), _startTrialButton->getContentSize().height * ( is18x9 ? 0.35 : 0.4 ));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	_startTrialLabel = Label::createWithTTF(_("Buy Azoomee Pass"), Style::Font::Regular(), _startTrialButton->getContentSize().height * ( is18x9 ? 0.35 : 0.4 ));
#else
    _startTrialLabel = Label::createWithTTF(_("Get 7 Days free"), Style::Font::Regular(), _startTrialButton->getContentSize().height * ( is18x9 ? 0.35 : 0.4 ));
#endif
    _startTrialLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _startTrialLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _startTrialLabel->setTextColor(Color4B(246,187,66,255));
	_startTrialLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	_startTrialLabel->setVerticalAlignment(TextVAlignment::CENTER);
	_startTrialLabel->setDimensions(_startTrialButton->getContentSize().width - 160, _startTrialButton->getContentSize().height * 0.8f);
	_startTrialLabel->setOverflow(Label::Overflow::SHRINK);
    _startTrialButton->addChild(_startTrialLabel);
    
    this->addChild(_startTrialButton);
	
#ifndef VODACOM_BUILD
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	_smallprintLabel = Label::createWithTTF(stringReplace(_("Get 30 days of Azoomee for only <iapPrice>."),"<iapPrice>",IAPProductDataHandler::getInstance()->getHumanReadableProductPrice().c_str()), Style::Font::Regular(), 50);
#else
	_smallprintLabel = Label::createWithTTF(StringUtils::format(_("Plan automatically renews for %s/month after trial ends").c_str(),IAPProductDataHandler::getInstance()->getHumanReadableProductPrice().c_str()), Style::Font::Regular(), 50);
#endif
	_smallprintLabel->setAnchorPoint(Vec2(0.5,-0.1));
	_smallprintLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
	_smallprintLabel->setTextColor(Color4B(255,255,255,255));
	_smallprintLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	_smallprintLabel->setVerticalAlignment(TextVAlignment::CENTER);
	_smallprintLabel->setDimensions(_startTrialButton->getContentSize().width, 50);
	_smallprintLabel->setOverflow(Label::Overflow::SHRINK);
	_startTrialButton->addChild(_smallprintLabel);
#endif
	
    _signInButton = ui::Button::create("res/artapp/white_bg.png");
    _signInButton->setContentSize(Size(this->getContentSize().width * 0.3f, this->getContentSize().height * 0.5f));
    _signInButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _signInButton->setNormalizedPosition(Vec2(isPortrait ? 0.85 : 0.9, 0.5));
    _signInButton->setSwallowTouches(true);
    _signInButton->setColor(Color3B(246,187,66));
    _signInButton->ignoreContentAdaptWithSize(false);
    _signInButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
        }
    });
	
	const Size& signInButtonSize = _signInButton->getContentSize();
    _signInLabel = Label::createWithTTF(_("Log in"), Style::Font::Regular(), 70);
    _signInLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _signInLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _signInLabel->setTextColor(Color4B::WHITE);
	_signInLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	_signInLabel->setVerticalAlignment(TextVAlignment::CENTER);
	_signInLabel->setOverflow(Label::Overflow::SHRINK);
	const Size& signInLabelSize = _signInLabel->getContentSize();
	DrawNode* underline = DrawNode::create();
	float heightOffset = signInButtonSize.width < signInLabelSize.width ? 0.0f : (signInButtonSize.height - signInLabelSize.height) / 2.0f;
	underline->drawRect(Vec2(signInButtonSize.width / 2 - MIN(signInLabelSize.width / 2, signInButtonSize.width / 2), heightOffset - 7), Vec2(signInButtonSize.width / 2 + MIN(signInLabelSize.width / 2, signInButtonSize.width / 2), heightOffset - 6), Color4F::WHITE);
    _signInLabel->addChild(underline);
	_signInLabel->setDimensions(signInButtonSize.width, signInButtonSize.height);
    _signInButton->addChild(_signInLabel);
    
    this->addChild(_signInButton);
    
    _reactivateButton = ui::Button::create("res/buttons/MainButton.png");
    _reactivateButton->setContentSize(Size(this->getContentSize().width * (isPortrait ? (is18x9 ? 0.85 : 0.75f) : 0.5f), _reactivateButton->getContentSize().height));
    _reactivateButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _reactivateButton->setNormalizedPosition(Vec2(0.5, 0.5));
    _reactivateButton->setSwallowTouches(true);
    _reactivateButton->setColor(Color3B::WHITE);
    _reactivateButton->ignoreContentAdaptWithSize(true);
    _reactivateButton->setScale9Enabled(true);
    _reactivateButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            DynamicNodeHandler::getInstance()->startIAPFlow();
        }
    });
	
#ifdef VODACOM_BUILD
	_reactivateLabel = Label::createWithTTF(_("Unlock everything"), Style::Font::Regular(), _reactivateButton->getContentSize().height * 0.4);
#else
    _reactivateLabel = Label::createWithTTF(_("Reactivate your account"), Style::Font::Regular(), _reactivateButton->getContentSize().height * 0.4);
#endif
    _reactivateLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _reactivateLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _reactivateLabel->setTextColor(Color4B(246,187,66,255));
	_reactivateLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	_reactivateLabel->setVerticalAlignment(TextVAlignment::CENTER);
	_reactivateLabel->setDimensions(_reactivateButton->getContentSize().width - 160, _reactivateButton->getContentSize().height * 0.8f);
	_reactivateLabel->setOverflow(Label::Overflow::SHRINK);
    _reactivateButton->addChild(_reactivateLabel);
    
    this->addChild(_reactivateButton);
    
    _premiumLabel = Label::createWithTTF(_("Congratulations! You are now a Premium User"), Style::Font::Regular(), 72);
    _premiumLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _premiumLabel->setTextColor(Color4B::WHITE);
	_premiumLabel->setOverflow(Label::Overflow::SHRINK);
	_premiumLabel->setDimensions(this->getContentSize().width * 0.8f, this->getContentSize().height * 0.8f);
    this->addChild(_premiumLabel);
    
    switch (_userType) {
        case UserBillingType::ANON:
            _reactivateButton->setVisible(false);
            _premiumLabel->setVisible(false);
            break;
        case UserBillingType::LAPSED:
            _signInButton->setVisible(false);
            _startTrialButton->setVisible(false);
            _premiumLabel->setVisible(false);
            break;
        case UserBillingType::PAID:
            _reactivateButton->setVisible(false);
            _signInButton->setVisible(false);
            _startTrialButton->setVisible(false);
            break;
    }
    
}

void UserTypeMessagingLayer::setUserType(UserBillingType userType)
{
    _userType = userType;
}

void UserTypeMessagingLayer::repositionElements()
{
    bool isPortrait = Director::getInstance()->getVisibleSize().width < Director::getInstance()->getVisibleSize().height;
    bool is18x9 = ConfigStorage::getInstance()->isDevice18x9();
    _bgSprite->setContentSize(this->getContentSize());
    
    _startTrialButton->setContentSize(Size(this->getContentSize().width * (isPortrait ? 0.65f : 0.5f), _startTrialButton->getContentSize().height));
#ifdef VODACOM_BUILD
	_startTrialButton->setNormalizedPosition(Vec2(isPortrait ? 0.35 : 0.5, 0.5));
#else
	_startTrialButton->setNormalizedPosition(Vec2(isPortrait ? 0.35 : 0.5, 0.4));
#endif
    _startTrialLabel->setDimensions(_startTrialButton->getContentSize().width - 160, _startTrialButton->getContentSize().height * 0.7f);
	if(_smallprintLabel)
	{
		_smallprintLabel->setDimensions(_startTrialButton->getContentSize().width - 160, 60);
	}
    
    _signInButton->setContentSize(Size(this->getContentSize().width * 0.3f, this->getContentSize().height * 0.5f));
    _signInButton->setNormalizedPosition(Vec2(isPortrait ? 0.85 : 0.9, 0.5));
	_signInLabel->removeFromParent();
	const Size& signInButtonSize = _signInButton->getContentSize();
	_signInLabel = Label::createWithTTF(_("Log in"), Style::Font::Regular(), 70);
	_signInLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_signInLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_signInLabel->setTextColor(Color4B::WHITE);
	_signInLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	_signInLabel->setVerticalAlignment(TextVAlignment::CENTER);
	const Size& signInLabelSize = _signInLabel->getContentSize();
	DrawNode* underline = DrawNode::create();
	float heightOffset = signInButtonSize.width < signInLabelSize.width ? 0.0f : (signInButtonSize.height - signInLabelSize.height) / 2.0f;
	underline->drawRect(Vec2(signInButtonSize.width / 2 - MIN(signInLabelSize.width / 2, signInButtonSize.width / 2), heightOffset - 7), Vec2(signInButtonSize.width / 2 + MIN(signInLabelSize.width / 2, signInButtonSize.width / 2), heightOffset - 6), Color4F::WHITE);
	_signInLabel->addChild(underline);
	_signInLabel->setDimensions(signInButtonSize.width, signInButtonSize.height);
	_signInLabel->setOverflow(Label::Overflow::SHRINK);
	_signInButton->addChild(_signInLabel);
    
    _reactivateButton->setContentSize(Size(this->getContentSize().width * (isPortrait ? (is18x9 ? 0.85 : 0.75f) : 0.5f), _reactivateButton->getContentSize().height));
    _reactivateButton->setNormalizedPosition(Vec2(isPortrait ? 0.5 : 0.5, 0.5));
    _reactivateLabel->setDimensions(_reactivateButton->getContentSize().width - 160, _reactivateButton->getContentSize().height * 0.7f);
    
    _premiumLabel->setDimensions(this->getContentSize().width * 0.8f, this->getContentSize().height * 0.8f);
}

NS_AZOOMEE_END
