//
//  StickerPlacementNode.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 05/06/2017.
//
//

#include "StickerPlacementNode.h"
#include "DrawingCanvas.h"
#include <AzoomeeCommon/Application.h>
#include <AzoomeeCommon/Gestures/GestureRecognizerUtils.h>

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
    
    this->setContentSize(visibleSize);
    
    _stickerFrame = ui::Scale9Sprite::create(kArtAppAssetLoc + "dotted_box.png");
    _stickerFrame->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(_stickerFrame);
    
    _sticker = Sprite::create();
    _sticker->setAnchorPoint(Vec2(0.5,0.5));
    _sticker->setPosition(visibleSize/2);
    this->addChild(_sticker);
    
    _stickerButton_rotate = Sprite::create(kArtAppAssetLoc + "rotate.png");
    _stickerButton_rotate->setAnchorPoint(Vec2(0.5,0.5));
    _stickerButton_rotate->setPosition(_sticker->getPosition() - _sticker->getContentSize()/2 - _stickerButton_rotate->getContentSize()/2);
    _stickerButton_rotate->setScale(1.5);
    this->addChild(_stickerButton_rotate);
    
    _stickerButton_scale= Sprite::create(kArtAppAssetLoc + "scale.png");
    _stickerButton_scale->setAnchorPoint(Vec2(0.5,0.5));
    _stickerButton_scale->setPosition(_sticker->getPosition() + _sticker->getContentSize()/2 + _stickerButton_scale->getContentSize()/2);
    _stickerButton_scale->setScale(1.5);
    this->addChild(_stickerButton_scale);
    
    _stickerFrame->setContentSize(_sticker->getContentSize() + _stickerButton_rotate->getContentSize());
    _stickerFrame->setPosition(_sticker->getPosition());
    
    addGestureRecognizers();
}

void StickerPlacementNode::onExit()
{
    Super::onExit();
}

void StickerPlacementNode::addGestureRecognizers()
{
    _pinchGesture = PinchGestureRecognizer::create();
    _pinchGesture->onPinch = CC_CALLBACK_1(StickerPlacementNode::onPinch, this);
    addChild(_pinchGesture);
    
    const float fingers = 1;
    _panGesture = PanGestureRecognizer::create(fingers);
    _panGesture->onPan = CC_CALLBACK_1(StickerPlacementNode::onPan, this);
    addChild(_panGesture);
}

void StickerPlacementNode::onPinch(PinchGestureRecognizer* recognizer)
{
    auto state = recognizer->getStatus();
    auto location = recognizer->getGestureLocation();
    
    static bool filterTouch = true;
    if(state == GestureStatus::BEGAN)
    {
        filterTouch = false;
        // TODO: filterTouch = true if touch is outside canvas?
        //CCLOG("Pinch Began");
//        if(sprite)
//        {
//            filterTouch = !nodeContainsThePoint(sprite, location);
//        }
    }
    else if(state == GestureStatus::CHANGED)
    {
        auto factor = recognizer->getPinchFactor();
        auto angle  = recognizer->getPinchRotation();
//        auto trasl  = recognizer->getPinchTraslation();
        if(_sticker && !filterTouch)
        {
            _scaleFactor = _sticker->getScale() * factor;
            _sticker->setScale(_scaleFactor);
            _rotationAngle = _sticker->getRotation() + angle;
            _sticker->setRotation(_rotationAngle);
//            _sticker->setPosition(_sticker->getPosition() + trasl);
            
//            _stickerFrame->setPosition(_sticker->getPosition());
            _stickerButton_rotate->setRotation(_rotationAngle);
            _stickerButton_scale->setRotation(_rotationAngle);
            _stickerFrame->setRotation(_rotationAngle);
            
            this->updateStickerControls();
        }
    }
    else if(state == GestureStatus::RECOGNIZED)
    {
        filterTouch = true;
    }
}

void StickerPlacementNode::onPan(PanGestureRecognizer *recognizer)
{
    auto state = recognizer->getStatus();
    auto location = recognizer->getGestureLocation();
    
    static bool filterTouch = true;
    if(state == GestureStatus::BEGAN)
    {
        if(_sticker)
        {
            filterTouch = !nodeContainsThePoint(_sticker, location);
        }
    }
    else if(state == GestureStatus::CHANGED)
    {
        if(_sticker && !filterTouch)
        {
            _sticker->setPosition(_sticker->getPosition() + recognizer->getTraslation());
            _stickerFrame->setPosition(_sticker->getPosition());
            
            this->updateStickerControls();
        }
    }
    else if(state == GestureStatus::RECOGNIZED)
    {
        filterTouch = true;
    }
}

void StickerPlacementNode::setTouchListenerEnabled(bool enabled)
{
    _panGesture->setEnabled(enabled);
    _pinchGesture->setEnabled(enabled);
    Azoomee::Application::setMultipleTouchEnabled(enabled);
}

void StickerPlacementNode::setSticker(Sprite *sticker)
{
    if(this->_sticker)
    {
        this->removeChild(this->_sticker);
    }
    this->_sticker = sticker;
    this->addChild(sticker);
    
    _stickerFrame->setContentSize(_sticker->getContentSize() + _stickerButton_rotate->getContentSize());
    _stickerFrame->setPosition(_sticker->getPosition());
    
    updateStickerControls();
}

Sprite* StickerPlacementNode::getSticker()
{
    return _sticker;
}

void StickerPlacementNode::reset()
{
    _rotationAngle = 0;
    _scaleFactor = 1;
    _stickerButton_rotate->setRotation(_rotationAngle);
    _stickerButton_scale->setRotation(_rotationAngle);
    _stickerFrame->setScale(_scaleFactor);
    _stickerFrame->setRotation(_rotationAngle);
}

void StickerPlacementNode::updateStickerControls()
{
    Vec2 offset_rotate = -Vec2((_sticker->getContentSize()/2) * _sticker->getScale()) - _stickerButton_rotate->getContentSize()/2;
    offset_rotate.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-_rotationAngle));
    _stickerButton_rotate->setPosition(_sticker->getPosition() + offset_rotate);
    
    Vec2 offset_scale = ((_sticker->getContentSize()/2) * _sticker->getScale()) + _stickerButton_scale->getContentSize()/2;
    offset_scale.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-_rotationAngle));
    _stickerButton_scale->setPosition(_sticker->getPosition() + offset_scale);
    
    const Vec2& baseFrameSize = _stickerFrame->getContentSize();
    const Vec2& newFrameSize = _stickerButton_rotate->getPosition() - _stickerButton_scale->getPosition();
    _stickerFrame->setScale(newFrameSize.length()/baseFrameSize.length());
    
}

NS_AZOOMEE_AA_END
