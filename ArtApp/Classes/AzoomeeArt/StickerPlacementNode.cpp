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
    
    stickerButton_rotate = ui::Button::create();
    stickerButton_rotate->loadTextures(ArtAppAssetLoc + "rotateSlider.png", ArtAppAssetLoc + "rotateSlider.png");
    stickerButton_rotate->setAnchorPoint(Vec2(0.5,0.5));
    stickerButton_rotate->setPosition(sticker->getPosition() - sticker->getContentSize()/2 - stickerButton_rotate->getContentSize()/2);
    this->addChild(stickerButton_rotate);
    
    stickerButton_scale= ui::Button::create();
    stickerButton_scale->loadTextures(ArtAppAssetLoc + "sizeSlider.png", ArtAppAssetLoc + "sizeSlider.png");
    stickerButton_scale->setAnchorPoint(Vec2(0.5,0.5));
    stickerButton_scale->setPosition(sticker->getPosition() + sticker->getContentSize()/2 + stickerButton_scale->getContentSize()/2);
    this->addChild(stickerButton_scale);
    
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
        
        Vec2 offset_rotate = -Vec2((sticker->getContentSize()/2) * sticker->getScale()) - stickerButton_rotate->getContentSize()/2;
        offset_rotate.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-rotationAngle));
        stickerButton_rotate->setPosition(sticker->getPosition() + offset_rotate);
        
        Vec2 offset_scale = ((sticker->getContentSize()/2) * sticker->getScale()) + stickerButton_scale->getContentSize()/2;
        offset_scale.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-rotationAngle));
        stickerButton_scale->setPosition(sticker->getPosition() + offset_scale);
        
        touchDetected = true;
        
        return true;
    };
    
    touchListener->onTouchMoved = [=](Touch* touch, Event* event)
    {
        sticker->setPosition(touch->getLocation());
        
        Vec2 offset_rotate = -Vec2((sticker->getContentSize()/2) * sticker->getScale()) - stickerButton_rotate->getContentSize()/2;
        offset_rotate.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-rotationAngle));
        stickerButton_rotate->setPosition(sticker->getPosition() + offset_rotate);
        
        Vec2 offset_scale = ((sticker->getContentSize()/2) * sticker->getScale()) + stickerButton_scale->getContentSize()/2;
        offset_scale.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-rotationAngle));
        stickerButton_scale->setPosition(sticker->getPosition() + offset_scale);
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
    
    Vec2 offset_rotate = -Vec2((sticker->getContentSize()/2) * sticker->getScale()) - stickerButton_rotate->getContentSize()/2;
    offset_rotate.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-rotationAngle));
    stickerButton_rotate->setPosition(sticker->getPosition() + offset_rotate);
    
    Vec2 offset_scale = ((sticker->getContentSize()/2) * sticker->getScale()) + stickerButton_scale->getContentSize()/2;
    offset_scale.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-rotationAngle));
    stickerButton_scale->setPosition(sticker->getPosition() + offset_scale);
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
        
        Vec2 offset_rotate = -Vec2((sticker->getContentSize()/2) * sticker->getScale()) - stickerButton_rotate->getContentSize()/2;
        offset_rotate.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-rotationAngle));
        stickerButton_rotate->setPosition(sticker->getPosition() + offset_rotate);
        
        Vec2 offset_scale = ((sticker->getContentSize()/2) * sticker->getScale()) + stickerButton_scale->getContentSize()/2;
        offset_scale.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-rotationAngle));
        stickerButton_scale->setPosition(sticker->getPosition() + offset_scale);
    }
}

void StickerPlacementNode::onRotationSliderInteract(Ref *pSender, ui::Slider::EventType eEventType)
{
    ui::Slider* slider = static_cast<ui::Slider*>(pSender);
    
    if(eEventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        rotationAngle = slider->getPercent()*3.6f;
        sticker->setRotation(rotationAngle);
        stickerButton_rotate->setRotation(rotationAngle);
        stickerButton_scale->setRotation(rotationAngle);
        
        Vec2 offset_rotate = -Vec2((sticker->getContentSize()/2) * sticker->getScale()) - stickerButton_rotate->getContentSize()/2;
        offset_rotate.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-rotationAngle));
        stickerButton_rotate->setPosition(sticker->getPosition() + offset_rotate);
        
        Vec2 offset_scale = ((sticker->getContentSize()/2) * sticker->getScale()) + stickerButton_scale->getContentSize()/2;
        offset_scale.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-rotationAngle));
        stickerButton_scale->setPosition(sticker->getPosition() + offset_scale);
        
        
    }
}

void StickerPlacementNode::resetSliders()
{
    rotationSlider->setPercent(0);
    rotationAngle = 0;
    scaleSlider->setPercent(20);
    scaleFactor = 1.1;
    stickerButton_rotate->setRotation(rotationAngle);
    stickerButton_scale->setRotation(rotationAngle);
}

NS_AZOOMEE_AA_END
