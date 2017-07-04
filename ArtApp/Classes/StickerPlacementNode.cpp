//
//  StickerPlacementNode.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 05/06/2017.
//
//

#include "StickerPlacementNode.h"

using namespace cocos2d;

bool StickerPlacementNode::init()
{
    
    if(!Node::init())
        return false;
    
    return true;
}

void StickerPlacementNode::onEnter()
{
    
    Node::onEnter();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setContentSize(visibleSize);
    
    sticker = Sprite::create("res/artapp/style/images/articons/art_button_sticker.png");
    
    sticker->setAnchorPoint(Vec2(0.5,0.5));
    sticker->setPosition(visibleSize/2);
    this->addChild(sticker);
    
    setupTouchHandling();
}

void StickerPlacementNode::onExit()
{
    Node::onExit();
}

void StickerPlacementNode::setupTouchHandling()
{
    
    static int touchNumber = 0;
    static std::vector<Touch*> touches;
    static float lastDistance;
    static bool pinchDetected = false;
    
    touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [=](Touch* touch, Event* event)
    {
        if(pinchDetected)
            return false;
        
        touchNumber++;
        touches.push_back(touch);
        
        if(touches.size() == 2)
            pinchDetected = true;
        
        return true;
    };
    
    touchListener->onTouchMoved = [=](Touch* touch, Event* event)
    {
        if(pinchDetected)
        {
            Touch* touch1 = touches[0];
            Touch* touch2 = touches[1];
        
            float distance = touch1->getLocation().distance(touch2->getLocation());
        
            if(!lastDistance)
            lastDistance = distance;
        
            float distanceDiff = (distance - lastDistance)/300;
            lastDistance = distance;
            sticker->setScale(sticker->getScale() + distanceDiff);
        
            Vec2 position = (touch1->getLocation() + touch2->getLocation())/2;
        
            sticker->setPosition(position);
        }
        else
        {
        
        Point prevVector = touch->getPreviousLocation() - sticker->getPosition();
        float prevAngle = CC_RADIANS_TO_DEGREES(-prevVector.getAngle());
        
        Point curVector = touch->getLocation() - sticker->getPosition();
        float currAngle = CC_RADIANS_TO_DEGREES(-curVector.getAngle());
        
        sticker->setRotation(sticker->getRotation() + (currAngle - prevAngle));
        }
        
    };
    
    touchListener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        pinchDetected = false;
        lastDistance = 0;
        touchNumber--;
        
        touches.erase(std::remove_if(touches.begin(), touches.end(), [=](Touch* t){return t == touch ? true: false;}),touches.end());
        
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
}

void StickerPlacementNode::setTouchListenerEnabled(bool enabled)
{
    touchListener->setEnabled(enabled);
}

void StickerPlacementNode::setSticker(Sprite *sticker)
{
    this->removeChild(this->sticker);
    this->sticker = sticker;
    this->addChild(sticker);
}

Sprite* StickerPlacementNode::getSticker()
{
    return sticker;
}

