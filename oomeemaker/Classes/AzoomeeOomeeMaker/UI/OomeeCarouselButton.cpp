//
//  OomeeCarouselButton.cpp
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/03/2018.
//

#include "OomeeCarouselButton.h"
#include "SimpleAudioEngine.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include "OomeeSelectScene.h"
#include <AzoomeeCommon/UI/Style.h>

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
    _innerCircle = Sprite::create("res/oomeeMaker/circle_0.png");
    _innerCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //bgCircle1->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    //float targetScale = (Vec2(this->getContentSize()).length() * 0.49f) / bgCircle1->getContentSize().height;
    //bgCircle1->setPosition(Vec2(this->getContentSize().width / 2,  (bgCircle1->getContentSize().height * targetScale) / 2));
    _innerCircle->setScale(0);
    _innerCircle->setRotation(RandomHelper::random_real(0.0,M_PI));
    this->addChild(_innerCircle, -1);
    
    auto popIn1 = EaseBackOut::create(ScaleTo::create(0.5, (Vec2(this->getContentSize()).length() * 0.55f) / _innerCircle->getContentSize().height));
    auto rotate1 = RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360));
    
    _innerCircle->runAction(popIn1);
    _innerCircle->runAction(rotate1);
    
    _outerCircle = Sprite::create("res/oomeeMaker/circle_1.png");
    _outerCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    //bgCircle2->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    //bgCircle2->setPosition(Vec2(this->getContentSize().width / 2,  (bgCircle2->getContentSize().height * targetScale) / 2));
    _outerCircle->setScale(0);
    _outerCircle->setRotation(RandomHelper::random_real(0.0,M_PI));
    this->addChild(_outerCircle, -1);
    
    auto popIn2 = EaseBackOut::create(ScaleTo::create(0.5, (Vec2(this->getContentSize()).length() * 0.75f) / _outerCircle->getContentSize().height));
    auto rotate2 = RepeatForever::create(RotateBy::create(30 +  CCRANDOM_0_1() * 30, -360));
        
    _outerCircle->runAction(popIn2);
    _outerCircle->runAction(rotate2);
    
    float targetScale = (Vec2(this->getContentSize()).length() * 0.75f) / _outerCircle->getContentSize().height;
    _innerCircle->setNormalizedPosition(Vec2(0.5, ((_outerCircle->getContentSize().height * targetScale) / 2.5) / this->getContentSize().height));
    _outerCircle->setNormalizedPosition(Vec2(0.5, ((_outerCircle->getContentSize().height * targetScale) / 2.5) / this->getContentSize().height));
    
    _outerCircle->setColor(Style::Color::darkTeal);
    _innerCircle->setColor(Style::Color::darkTeal);
    
    _deleteButton = ui::Button::create("res/oomeeMaker/bin_button.png");
    _deleteButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _deleteButton->setPosition(this->getContentSize()/2);
    _deleteButton->setScale(0);
    _deleteButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == TouchEventType::ENDED)
        {
            log("delete oomee");
            if(_delegate)
            {
                _delegate->deleteOomee(_oomeeFileName);
            }
        }
    });
    this->addChild(_deleteButton);
    
    _makeAvatarButton = ui::Button::create("res/oomeeMaker/make_oomee_button.png");
    _makeAvatarButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _makeAvatarButton->setPosition(this->getContentSize()/2);
    _makeAvatarButton->setScale(0);
    _makeAvatarButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == TouchEventType::ENDED)
        {
            log("set as avatar");
            if(_delegate)
            {
                _delegate->makeAvatar(_oomeeFileName);
            }
        }
    });
    this->addChild(_makeAvatarButton);
    
    _shareButton = ui::Button::create("res/oomeeMaker/message_button.png");
    _shareButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _shareButton->setPosition(this->getContentSize()/2);
    _shareButton->setScale(0);
    _shareButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == TouchEventType::ENDED)
        {
            log("share oomee");
            if(_delegate)
            {
                _delegate->shareOomee(_oomeeFileName);
            }
        }
    });
    this->addChild(_shareButton);
    
    Super::onEnter();
}

void OomeeCarouselButton::setOomeeData(const std::string &filename)
{
    _oomeeFileName = filename;
    setMainImage(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + filename + ".png");
    setPlaceholderImage("res/oomeeMaker/1_Oomee_Reference.png");
    loadPlaceholderImage();
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

bool OomeeCarouselButton::getInFocus() const
{
    return _inFocus;
}

void OomeeCarouselButton::setDelegate(OomeeCarouselButtonDelegate *delegate)
{
    _delegate = delegate;
}

void OomeeCarouselButton::animateButtonsIn(float duration)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Homescreen_Buttons_Appear.mp3");
    const float centerWidth = this->getContentSize().width / 2.0f;
    const float centerHeight = this->getContentSize().height / 2.0f;
    const float durationMod = 1 - _deleteButton->getScale();
    duration *= durationMod;
    Action* scaleAction = ScaleTo::create(duration, 1);
    
    Vec2 basePos = Vec2(centerWidth, centerHeight + this->getContentSize().height*0.65f);
    
    Action* moveActionAvatar = EaseBackOut::create(MoveTo::create(duration, basePos));
    _makeAvatarButton->stopAllActions();
    _makeAvatarButton->runAction(scaleAction->clone());
    _makeAvatarButton->runAction(moveActionAvatar);
    
    basePos.rotate(Vec2(centerWidth,centerHeight),CC_DEGREES_TO_RADIANS(-30));
    Action* moveActionShare = EaseBackOut::create(MoveTo::create(duration, basePos));
    _shareButton->stopAllActions();
    _shareButton->runAction(scaleAction->clone());
    _shareButton->runAction(moveActionShare);
    
    
    basePos.rotate(Vec2(centerWidth,centerHeight),CC_DEGREES_TO_RADIANS(60));
    Action* moveActionDelete = EaseBackOut::create(MoveTo::create(duration, basePos));
    _deleteButton->stopAllActions();
    _deleteButton->runAction(scaleAction->clone());
    _deleteButton->runAction(moveActionDelete);
    
}

void OomeeCarouselButton::animateButtonsOut(float duration)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
    const float durationMod = _deleteButton->getScale();
    duration *= durationMod;
    Action* moveAction = EaseBackIn::create(MoveTo::create(duration, Vec2(this->getContentSize()/2)));
    Action* scaleAction = ScaleTo::create(duration, 0);
    
    _deleteButton->stopAllActions();
    _deleteButton->runAction(scaleAction->clone());
    _deleteButton->runAction(moveAction->clone());
    
    _shareButton->stopAllActions();
    _shareButton->runAction(scaleAction->clone());
    _shareButton->runAction(moveAction->clone());
    
    _makeAvatarButton->stopAllActions();
    _makeAvatarButton->runAction(scaleAction->clone());
    _makeAvatarButton->runAction(moveAction->clone());
}

void OomeeCarouselButton::enableHighlight(bool enable)
{
    if(enable)
    {
        _outerCircle->runAction(TintTo::create(0.5, Style::Color::oomeeGreen));
        _innerCircle->runAction(TintTo::create(0.5, Style::Color::oomeeGreen));
    }
    else
    {
        _outerCircle->runAction(TintTo::create(0.5, Style::Color::darkTeal));
        _innerCircle->runAction(TintTo::create(0.5, Style::Color::darkTeal));
    }
}


NS_AZOOMEE_OM_END
