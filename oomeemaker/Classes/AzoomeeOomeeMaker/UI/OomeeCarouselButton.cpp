//
//  OomeeCarouselButton.cpp
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/03/2018.
//

#include "OomeeCarouselButton.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include "OomeeSelectScene.h"

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

bool OomeeCarouselButton::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void OomeeCarouselButton::onEnter()
{
    _deleteButton = ui::Button::create("res/oomeeMaker/menu3.png");
    _deleteButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _deleteButton->setPosition(this->getContentSize()/2);
    _deleteButton->setScale(0);
    _deleteButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == TouchEventType::ENDED)
        {
            log("delete oomee");
        }
    });
    this->addChild(_deleteButton);
    
    _editButton = ui::Button::create("res/oomeeMaker/menu3.png");
    _editButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _editButton->setPosition(this->getContentSize()/2);
    _editButton->setScale(0);
    _editButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == TouchEventType::ENDED)
        {
            log("edit oomee");
            OomeeSelectScene::editOomee(OomeeMakerDataHandler::getInstance()->getLocalSaveDir() + _oomeeFileName);
        }
    });
    this->addChild(_editButton);
    
    _makeAvatarButton = ui::Button::create("res/oomeeMaker/menu3.png");
    _makeAvatarButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _makeAvatarButton->setPosition(this->getContentSize()/2);
    _makeAvatarButton->setScale(0);
    _makeAvatarButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == TouchEventType::ENDED)
        {
            log("set as avatar");
        }
    });
    this->addChild(_makeAvatarButton);
    
    _shareButton = ui::Button::create("res/oomeeMaker/menu3.png");
    _shareButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _shareButton->setPosition(this->getContentSize()/2);
    _shareButton->setScale(0);
    _shareButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == TouchEventType::ENDED)
        {
            log("share oomee");
        }
    });
    this->addChild(_shareButton);
    
    Super::onEnter();
}

void OomeeCarouselButton::setOomeeData(const std::string &filename)
{
    _oomeeFileName = filename;
    setMainImage(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + filename + ".png");
    setPlaceholderImage("res/OomeeMaker/body_00.png");
}

void OomeeCarouselButton::setInFocus(bool inFocus)
{
    if(inFocus == _inFocus)
    {
        //dont do anything if already in state
        return;
    }
    
    _inFocus = inFocus;
    if(inFocus)
    {
        animateButtonsIn(1);
    }
    else
    {
        animateButtonsOut(0.3);
    }
}

void OomeeCarouselButton::animateButtonsIn(float duration)
{
    const float centerWidth = this->getContentSize().width / 2.0f;
    const float centerHeight = this->getContentSize().height / 2.0f;
    const float durationMod = 1 - _deleteButton->getScale();
    duration *= durationMod;
    Action* scaleAction = ScaleTo::create(duration, 1);
    Action* moveActionDelete = EaseBackOut::create(MoveTo::create(duration, Vec2(centerWidth - this->getContentSize().height/2, centerHeight + this->getContentSize().height/2)));
    
    _deleteButton->stopAllActions();
    _deleteButton->runAction(scaleAction->clone());
    _deleteButton->runAction(moveActionDelete);
    
    Action* moveActionEdit = EaseBackOut::create(MoveTo::create(duration, Vec2(centerWidth + this->getContentSize().height/2, centerHeight + this->getContentSize().height/2)));
    _editButton->stopAllActions();
    _editButton->runAction(scaleAction->clone());
    _editButton->runAction(moveActionEdit);
    
    Action* moveActionShare = EaseBackOut::create(MoveTo::create(duration, Vec2(centerWidth - this->getContentSize().height/2, centerHeight - this->getContentSize().height/2)));
    _shareButton->stopAllActions();
    _shareButton->runAction(scaleAction->clone());
    _shareButton->runAction(moveActionShare);
    
    Action* moveActionAvatar = EaseBackOut::create(MoveTo::create(duration, Vec2(centerWidth + this->getContentSize().height/2, centerHeight - this->getContentSize().height/2)));
    _makeAvatarButton->stopAllActions();
    _makeAvatarButton->runAction(scaleAction->clone());
    _makeAvatarButton->runAction(moveActionAvatar);
    
}

void OomeeCarouselButton::animateButtonsOut(float duration)
{
    const float durationMod = _deleteButton->getScale();
    duration *= durationMod;
    Action* moveAction = EaseBackIn::create(MoveTo::create(duration, Vec2(this->getContentSize()/2)));
    Action* scaleAction = ScaleTo::create(duration, 0);
    
    _deleteButton->stopAllActions();
    _deleteButton->runAction(scaleAction->clone());
    _deleteButton->runAction(moveAction->clone());
    
    _editButton->stopAllActions();
    _editButton->runAction(scaleAction->clone());
    _editButton->runAction(moveAction->clone());
    
    _shareButton->stopAllActions();
    _shareButton->runAction(scaleAction->clone());
    _shareButton->runAction(moveAction->clone());
    
    _makeAvatarButton->stopAllActions();
    _makeAvatarButton->runAction(scaleAction->clone());
    _makeAvatarButton->runAction(moveAction->clone());
}


NS_AZOOMEE_OM_END
