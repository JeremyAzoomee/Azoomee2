//
//  StickerPlacementNode.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 05/06/2017.
//
//

#include "StickerPlacementNode.h"
#include "DrawingCanvas.h"

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

bool StickerPlacementNode::init()
{
    
    if(!Super::init())
        return false;
    
    return true;
}

void StickerPlacementNode::onEnter()
{
    
    Super::onEnter();
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2& visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    this->setContentSize(visibleSize);
    
    sticker = Sprite::create(ArtAppAssetLoc + "art_button_sticker.png");
    
    sticker->setAnchorPoint(Vec2(0.5,0.5));
    sticker->setPosition(visibleSize/2);
    this->addChild(sticker);
    
    scaleSlider = ui::Slider::create();
    scaleSlider->setTouchEnabled(true);
    scaleSlider->loadBarTexture(ArtAppAssetLoc + "slideBack.png");
    scaleSlider->loadSlidBallTextures(ArtAppAssetLoc + "sizeSlider.png",ArtAppAssetLoc + "sizeSlider.png","");
    scaleSlider->setPercent(20);
    scaleFactor = 1.1;
    scaleSlider->setAnchorPoint(Vec2(0.5,0.5));
    scaleSlider->setRotation(-90);
    scaleSlider->setPosition(Vec2(visibleOrigin.x + scaleSlider->getContentSize().height,visibleOrigin.y + visibleSize.height/2));
    scaleSlider->addEventListener(CC_CALLBACK_2(StickerPlacementNode::onScaleSliderInteract, this));
    this->addChild(scaleSlider,1);
    
    
    rotationSlider = ui::Slider::create();
    rotationSlider->setTouchEnabled(true);
    rotationSlider->loadBarTexture(ArtAppAssetLoc + "slideBack.png");
    rotationSlider->loadSlidBallTextures(ArtAppAssetLoc + "rotateSlider.png",ArtAppAssetLoc + "rotateSlider.png","");
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
    Super::onExit();
}

void StickerPlacementNode::setupTouchHandling()
{
    static bool touchDetected = false;
    
    touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [=](Touch* touch, Event* event)
    {
        if(touchDetected)
            return false;
        
        sticker->setPosition(touch->getLocation());
        
        touchDetected = true;
        
        return true;
    };
    
    touchListener->onTouchMoved = [=](Touch* touch, Event* event)
    {
        sticker->setPosition(touch->getLocation());
        
    };
    
    touchListener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        
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
    if(this->sticker)
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

void StickerPlacementNode::resetSliders()
{
    rotationSlider->setPercent(0);
    rotationAngle = 0;
    scaleSlider->setPercent(20);
    scaleFactor = 1.1;
    
}

NS_AZOOMEE_AA_END
