//
//  StickerPlacementNode.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 05/06/2017.
//
//

#include "StickerPlacementNode.h"

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

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
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    this->setContentSize(visibleSize);
    
    sticker = Sprite::create("res/artapp/style/images/artIcons/art_button_sticker.png");
    
    sticker->setAnchorPoint(Vec2(0.5,0.5));
    sticker->setPosition(visibleSize/2);
    this->addChild(sticker);
    
    scaleSlider = ui::Slider::create();
    scaleSlider->setTouchEnabled(true);
    scaleSlider->loadBarTexture("res/artapp/style/images/artIcons/slideBack.png");
    scaleSlider->loadSlidBallTextures("res/artapp/style/images/artIcons/sizeSlider.png","res/artapp/style/images/artIcons/sizeSlider.png","");
    scaleSlider->setPercent(20);
    scaleFactor = 1.1;
    scaleSlider->setAnchorPoint(Vec2(0.5,0.5));
    scaleSlider->setRotation(-90);
    scaleSlider->setPosition(Vec2(visibleOrigin.x + scaleSlider->getContentSize().height,visibleOrigin.y + visibleSize.height/2));
    scaleSlider->addEventListener(CC_CALLBACK_2(StickerPlacementNode::onScaleSliderInteract, this));
    this->addChild(scaleSlider,1);
    
    
    rotationSlider = ui::Slider::create();
    rotationSlider->setTouchEnabled(true);
    rotationSlider->loadBarTexture("res/artapp/style/images/artIcons/slideBack.png");
    rotationSlider->loadSlidBallTextures("res/artapp/style/images/artIcons/rotateSlider.png","res/artapp/style/images/artIcons/rotateSlider.png","");
    rotationSlider->setPercent(0);
    rotationAngle = 0;
    rotationSlider->setAnchorPoint(Vec2(0.5,0.5));
    rotationSlider->setRotation(-90);
    rotationSlider->setPosition(Vec2(visibleOrigin.x + visibleSize.width - rotationSlider->getContentSize().height,visibleOrigin.y + visibleSize.height/2));
    rotationSlider->addEventListener(CC_CALLBACK_2(StickerPlacementNode::onRotationSliderInteract, this));
    this->addChild(rotationSlider,1);
    
    setupTouchHandling();
}

void StickerPlacementNode::onExit()
{
    Node::onExit();
}

void StickerPlacementNode::setupTouchHandling()
{
    
    //static int touchNumber = 0;
    //static std::vector<Touch*> touches;
    //static float lastDistance;
    //static bool pinchDetected = false;
    
    static bool touchDetected = false;
    
    touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [=](Touch* touch, Event* event)
    {
        //if(pinchDetected)
        //    return false;
        
        //touchNumber++;
        //touches.push_back(touch);
        
        //if(touches.size() == 2)
        //    pinchDetected = true;
        
        if(touchDetected)
            return false;
        
        sticker->setPosition(touch->getLocation());
        
        touchDetected = true;
        
        return true;
    };
    
    touchListener->onTouchMoved = [=](Touch* touch, Event* event)
    {
        /*if(pinchDetected)
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
        }*/
        
        sticker->setPosition(touch->getLocation());
        
    };
    
    touchListener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        //pinchDetected = false;
        //lastDistance = 0;
        //touchNumber--;
        
        //touches.erase(std::remove_if(touches.begin(), touches.end(), [=](Touch* t){return t == touch ? true: false;}),touches.end());
        
        touchDetected = false;
        
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

void StickerPlacementNode::onScaleSliderInteract(Ref *pSender, ui::Slider::EventType eEventType)
{
    ui::Slider* slider = static_cast<ui::Slider*>(pSender);
    
    if(eEventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        scaleFactor = 0.1 + slider->getPercent()/20.0f;
        sticker->setScale(scaleFactor);
    }
}

void StickerPlacementNode::onRotationSliderInteract(Ref *pSender, ui::Slider::EventType eEventType)
{
    ui::Slider* slider = static_cast<ui::Slider*>(pSender);
    
    if(eEventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        rotationAngle = slider->getPercent()*3.6f;
        sticker->setRotation(rotationAngle);
    }
}

NS_AZOOMEE_AA_END
