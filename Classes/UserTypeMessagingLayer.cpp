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
    Super::onEnter();
    
    _bgSprite = ui::Scale9Sprite::create("res/artapp/popup_menu_bg.png");
    _bgSprite->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bgSprite->setContentSize(this->getContentSize());
    _bgSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->addChild(_bgSprite);
    
    _userTypeLabel = Label::createWithTTF(_userType == ANON ? "Free User" : "Expired User", Style::Font::Regular, 72);
    _userTypeLabel->setNormalizedPosition(Vec2(0.05,0.5));
    _userTypeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    this->addChild(_userTypeLabel);
    
    _startTrialButton = ui::Button::create("res/buttons/greenMainButton.png");
    _startTrialButton->setContentSize(Size(this->getContentSize().width * 0.2f, this->getContentSize().height * 0.8f));
    _startTrialButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _startTrialButton->setNormalizedPosition(Vec2(0.35, 0.5));
    _startTrialButton->setSwallowTouches(true);
    _startTrialButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            DynamicNodeHandler::getInstance()->startIAPFlow();
        }
    });
    
    Label* startTrialLabel = Label::createWithTTF("Start Trial", Style::Font::Regular, _startTrialButton->getContentSize().height * 0.4);
    startTrialLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    startTrialLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _startTrialButton->addChild(startTrialLabel);
    
    this->addChild(_startTrialButton);
    
    _signInButton = ui::Button::create("res/buttons/greenMainButton.png");
    _signInButton->setContentSize(Size(this->getContentSize().width * 0.2f, this->getContentSize().height * 0.8f));
    _signInButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _signInButton->setNormalizedPosition(Vec2(0.65, 0.5));
    _signInButton->setSwallowTouches(true);
    _signInButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(Login));
        }
    });
    
    Label* signInLabel = Label::createWithTTF("Log In", Style::Font::Regular, _signInButton->getContentSize().height * 0.4);
    signInLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    signInLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _signInButton->addChild(signInLabel);
    
    this->addChild(_signInButton);
    
    _reactivateButton = ui::Button::create("res/buttons/greenMainButton.png");
    _reactivateButton->setContentSize(Size(this->getContentSize().width * 0.3f, this->getContentSize().height * 0.8f));
    _reactivateButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _reactivateButton->setNormalizedPosition(Vec2(0.5, 0.5));
    _reactivateButton->setSwallowTouches(true);
    _reactivateButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            DynamicNodeHandler::getInstance()->startIAPFlow();
        }
    });
    
    Label* reactivateLabel = Label::createWithTTF("Reactivate", Style::Font::Regular, _reactivateButton->getContentSize().height * 0.4);
    reactivateLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    reactivateLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _reactivateButton->addChild(reactivateLabel);
    
    this->addChild(_reactivateButton);
    
    switch (_userType) {
        case ANON:
            _reactivateButton->setVisible(false);
            break;
        case LAPSED:
            _signInButton->setVisible(false);
            _startTrialButton->setVisible(false);
            break;
        case PAID:
            _reactivateButton->setVisible(false);
            _signInButton->setVisible(false);
            _startTrialButton->setVisible(false);
            _userTypeLabel->setVisible(false);
            break;
    }
    
    _infoButton = ui::Button::create("res/mainhub/blue_circle.png");
    _infoButton->setContentSize(Size(this->getContentSize().height * 0.6f, this->getContentSize().height * 0.6f));
    _infoButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _infoButton->setNormalizedPosition(Vec2(0.95, 0.5));
    _infoButton->setSwallowTouches(true);
    _infoButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::BEGAN)
        {
            DynamicNodeHandler::getInstance()->createDynamicNodeById("iap_learn_more.json");
        }
    });
    
    Label* infoLabel = Label::createWithTTF("i", Style::Font::Regular, _infoButton->getContentSize().height * 0.8);
    infoLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    infoLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _infoButton->addChild(infoLabel);
    
    this->addChild(_infoButton);
}

void UserTypeMessagingLayer::setUserType(UserType userType)
{
    _userType = userType;
}

NS_AZOOMEE_END
