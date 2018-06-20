//
//  UserTypeMessagingLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 26/04/2018.
//

#include "UserTypeMessagingLayer.h"
#include "DynamicNodeHandler.h"
#include "SceneManagerScene.h"
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
    
    Super::onEnter();
    
    _bgSprite = ui::Scale9Sprite::create("res/artapp/white_bg.png");
    _bgSprite->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bgSprite->setContentSize(this->getContentSize());
    _bgSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bgSprite->setColor(Color3B(246,187,66));
    this->addChild(_bgSprite);
    
    _startTrialButton = ui::Button::create("res/buttons/MainButton.png");
    _startTrialButton->setContentSize(Size(this->getContentSize().width * (isPortrait ? 0.6f : 0.5f), this->getContentSize().height * 0.65f));
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
    
    _startTrialLabel = Label::createWithTTF("Start 7 Day Trial", Style::Font::Regular, _startTrialButton->getContentSize().height * 0.4);
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
    
    _signInLabel = Label::createWithTTF("Log In", Style::Font::Regular, _signInButton->getContentSize().height * 0.4);
    _signInLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _signInLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _signInLabel->setTextColor(Color4B::WHITE);
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->drawRect(Vec2(0, -7), Vec2(_signInLabel->getContentSize().width, -6), Color4F::WHITE);
    _signInLabel->addChild(newDrawNode);
    _signInButton->addChild(_signInLabel);
    
    this->addChild(_signInButton);
    
    _reactivateButton = ui::Button::create("res/buttons/MainButton.png");
    _reactivateButton->setContentSize(Size(this->getContentSize().width * (isPortrait ? 0.6f : 0.5f), this->getContentSize().height * 0.65f));
    _reactivateButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _reactivateButton->setNormalizedPosition(Vec2(0.5, 0.5));
    _reactivateButton->setSwallowTouches(true);
    _reactivateButton->setColor(Color3B::WHITE);
    _reactivateButton->ignoreContentAdaptWithSize(true);
    _reactivateButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            DynamicNodeHandler::getInstance()->startIAPFlow();
        }
    });
    
    _reactivateLabel = Label::createWithTTF("Reactivate", Style::Font::Regular, _reactivateButton->getContentSize().height * 0.4);
    _reactivateLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _reactivateLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _reactivateLabel->setTextColor(Color4B(246,187,66,255));
    _reactivateButton->addChild(_reactivateLabel);
    
    this->addChild(_reactivateButton);
    
    _premiumLabel = Label::createWithTTF(StringUtils::format("Congratulations!%sYou are now a Premium User.",(isPortrait ? "\n" : " ")), Style::Font::Regular, 72);
    _premiumLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _premiumLabel->setTextColor(Color4B::WHITE);
    this->addChild(_premiumLabel);
    
    switch (_userType) {
        case ANON:
            _reactivateButton->setVisible(false);
            _premiumLabel->setVisible(false);
            break;
        case LAPSED:
            _signInButton->setVisible(false);
            _startTrialButton->setVisible(false);
            _premiumLabel->setVisible(false);
            break;
        case PAID:
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
    
    _bgSprite->setContentSize(this->getContentSize());
    
    _startTrialButton->setContentSize(Size(this->getContentSize().width * (isPortrait ? 0.6f : 0.5f), this->getContentSize().height * 0.65f));
    _startTrialButton->setNormalizedPosition(Vec2(isPortrait ? 0.33 : 0.5, 0.5));
    _startTrialLabel->setBMFontSize(_startTrialButton->getContentSize().height * 0.4);
    
    _signInButton->setContentSize(Size(this->getContentSize().width * 0.2f, this->getContentSize().height * 0.65f));
    _signInButton->setNormalizedPosition(Vec2(isPortrait ? 0.85 : 0.9, 0.5));
    _signInLabel->setBMFontSize(_signInButton->getContentSize().height * 0.4);
    _signInLabel->removeAllChildren();
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->drawRect(Vec2(0, -7), Vec2(_signInLabel->getContentSize().width, -6), Color4F::WHITE);
    _signInLabel->addChild(newDrawNode);
    
    _reactivateButton->setContentSize(Size(this->getContentSize().width * (isPortrait ? 0.6f : 0.5f), this->getContentSize().height * 0.65f));
    _reactivateButton->setNormalizedPosition(Vec2(isPortrait ? 0.33 : 0.5, 0.5));
    _reactivateLabel->setBMFontSize(_reactivateButton->getContentSize().height * 0.4);
    
    _premiumLabel->setString(StringUtils::format("Congratulations!%sYou are now a Premium User.",(isPortrait ? "\n" : " ")));
}

NS_AZOOMEE_END
