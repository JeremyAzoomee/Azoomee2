//
//  UserTypeMessagingLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 26/04/2018.
//

#include "UserTypeMessagingLayer.h"
#include "DynamicNodeHandler.h"
#include "SceneManagerScene.h"
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
    _startTrialButton->setNormalizedPosition(Vec2(isPortrait ? 0.33 : 0.5, 0.5));
    _startTrialButton->setSwallowTouches(true);
    _startTrialButton->setColor(Color3B::WHITE);
    _startTrialButton->ignoreContentAdaptWithSize(true);
    _startTrialButton->setScale9Enabled(true);
    _startTrialButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            DynamicNodeHandler::getInstance()->startIAPFlow();
        }
    });
    
    _startTrialLabel = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(USER_MSG_START_TRIAL), Style::Font::Regular, _startTrialButton->getContentSize().height * ( is18x9 ? 0.35 : 0.4 ));
    _startTrialLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _startTrialLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _startTrialLabel->setTextColor(Color4B(246,187,66,255));
    _startTrialButton->addChild(_startTrialLabel);
    
    this->addChild(_startTrialButton);
    
    _signInButton = ui::Button::create("res/artapp/white_bg.png");
    _signInButton->setContentSize(Size(this->getContentSize().width * 0.2f, this->getContentSize().height * 0.65f));
    _signInButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _signInButton->setNormalizedPosition(Vec2(isPortrait ? 0.85 : 0.9, 0.5));
    _signInButton->setSwallowTouches(true);
    _signInButton->setColor(Color3B(246,187,66));
    _signInButton->ignoreContentAdaptWithSize(true);
    _signInButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(Login));
        }
    });
    
    _signInLabel = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_IN), Style::Font::Regular, _signInButton->getContentSize().height * 0.4);
    _signInLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _signInLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _signInLabel->setTextColor(Color4B::WHITE);
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->drawRect(Vec2(0, -7), Vec2(_signInLabel->getContentSize().width, -6), Color4F::WHITE);
    _signInLabel->addChild(newDrawNode);
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
    
    _reactivateLabel = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(USER_MSG_REACTIVATE), Style::Font::Regular, _reactivateButton->getContentSize().height * 0.4);
    _reactivateLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _reactivateLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _reactivateLabel->setTextColor(Color4B(246,187,66,255));
    _reactivateButton->addChild(_reactivateLabel);
    
    this->addChild(_reactivateButton);
    
    _premiumLabel = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(isPortrait ? USER_MSG_PREMIUM_MULTILINE : USER_MSG_PREMIUM), Style::Font::Regular, 72);
    _premiumLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _premiumLabel->setTextColor(Color4B::WHITE);
    this->addChild(_premiumLabel);
    
    switch (_userType) {
        case UserType::ANON:
            _reactivateButton->setVisible(false);
            _premiumLabel->setVisible(false);
            break;
        case UserType::LAPSED:
            _signInButton->setVisible(false);
            _startTrialButton->setVisible(false);
            _premiumLabel->setVisible(false);
            break;
        case UserType::PAID:
            _reactivateButton->setVisible(false);
            _signInButton->setVisible(false);
            _startTrialButton->setVisible(false);
            break;
    }
    
}

void UserTypeMessagingLayer::setUserType(UserType userType)
{
    _userType = userType;
}

void UserTypeMessagingLayer::repositionElements()
{
    bool isPortrait = Director::getInstance()->getVisibleSize().width < Director::getInstance()->getVisibleSize().height;
    bool is18x9 = ConfigStorage::getInstance()->isDevice18x9();
    _bgSprite->setContentSize(this->getContentSize());
    
    _startTrialButton->setContentSize(Size(this->getContentSize().width * (isPortrait ? 0.65f : 0.5f), _startTrialButton->getContentSize().height));
    _startTrialButton->setNormalizedPosition(Vec2(isPortrait ? 0.33 : 0.5, 0.5));
    _startTrialLabel->setBMFontSize(_startTrialButton->getContentSize().height * ( is18x9 ? 0.35 : 0.4 ));
    
    _signInButton->setContentSize(Size(this->getContentSize().width * 0.2f, this->getContentSize().height * 0.65f));
    _signInButton->setNormalizedPosition(Vec2(isPortrait ? 0.85 : 0.9, 0.5));
    _signInLabel->setBMFontSize(_signInButton->getContentSize().height * 0.4);
    _signInLabel->removeAllChildren();
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->drawRect(Vec2(0, -7), Vec2(_signInLabel->getContentSize().width, -6), Color4F::WHITE);
    _signInLabel->addChild(newDrawNode);
    
    _reactivateButton->setContentSize(Size(this->getContentSize().width * (isPortrait ? (is18x9 ? 0.85 : 0.75f) : 0.5f), _reactivateButton->getContentSize().height));
    _reactivateButton->setNormalizedPosition(Vec2(isPortrait ? 0.5 : 0.5, 0.5));
    _reactivateLabel->setBMFontSize(_reactivateButton->getContentSize().height * 0.4);
    
    _premiumLabel->setString(StringMgr::getInstance()->getStringForKey(isPortrait ? USER_MSG_PREMIUM_MULTILINE : USER_MSG_PREMIUM));
}

NS_AZOOMEE_END
