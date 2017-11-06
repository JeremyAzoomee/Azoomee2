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
    
    setupTouchHandling();
}

void StickerPlacementNode::onExit()
{
    Super::onExit();
}

void StickerPlacementNode::setupTouchHandling()
{
    static bool touchDetected = false;
    static ControlMode mode = NONE;
    
    _touchListener = EventListenerTouchOneByOne::create();
    
    _touchListener->onTouchBegan = [=](Touch* touch, Event* event)
    {
        if(touchDetected)
        {
            return false;
        }
        
        if(_stickerButton_rotate->getBoundingBox().containsPoint(touch->getLocation()))
        {
            mode = ROTATE;
        }
        else if(_stickerButton_scale->getBoundingBox().containsPoint(touch->getLocation()))
        {
            mode = SCALE;
        }
        else if(_sticker->getBoundingBox().containsPoint(touch->getLocation()))
        {
            mode = MOVE;
        }
        else
        {
            return false;
        }
        
        touchDetected = true;
        
        return true;
    };
    
    _touchListener->onTouchMoved = [=](Touch* touch, Event* event)
    {
        
        switch(mode)
        {
            case MOVE:
            {
                const Vec2& moveVec = touch->getLocation() - touch->getPreviousLocation();
                _sticker->setPosition(_sticker->getPosition() + moveVec);
                _stickerFrame->setPosition(_sticker->getPosition());
                break;
            }
            case ROTATE:
            {
                const Vec2& touchVec = _sticker->getPosition() - touch->getLocation();
                _rotationAngle = CC_RADIANS_TO_DEGREES(-touchVec.getAngle()) + 45;
                _sticker->setRotation(_rotationAngle);
                _stickerButton_rotate->setRotation(_rotationAngle);
                _stickerButton_scale->setRotation(_rotationAngle);
                _stickerFrame->setRotation(_rotationAngle);
                break;
            }
                
            case SCALE:
            {
                const Vec2& baseStickerSize = _sticker->getContentSize()/2;
                const Vec2& midToTouchLength = touch->getLocation() - _sticker->getPosition();
                _scaleFactor = midToTouchLength.length()/baseStickerSize.length();
                _sticker->setScale(_scaleFactor);
                break;
            }
            case NONE:
            {
                break;
            }
        }
        
        this->updateStickerControls();
        
    };
    
    _touchListener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        
        touchDetected = false;
        
    };
    
    _touchListener->onTouchCancelled = _touchListener->onTouchEnded;
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    
}

void StickerPlacementNode::setTouchListenerEnabled(bool enabled)
{
    _touchListener->setEnabled(enabled);
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
