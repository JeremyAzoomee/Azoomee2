//
//  BrushEraser.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 02/06/2017.
//
//

#include "BrushEraser.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

BrushEraser::BrushEraser():Brush()
{
    _type = ERASER;
}

Node* BrushEraser::addDrawNode(const Size& visibleSize)
{
    _drawNode = DrawNode::create();
    _drawNode->setContentSize(visibleSize);
    _maskingNode = ClippingNode::create(_drawNode);
    
    return _maskingNode;
}

Node* BrushEraser::getDrawNode()
{
    return _maskingNode;
}

void BrushEraser::setBgImageFile(const std::string& filename)
{
    _bgImageFile = filename;
}

void BrushEraser::onTouchBegin(Touch *touch, Event *event)
{
    _lastTouchPos = _drawNode->convertTouchToNodeSpace(touch);
    Sprite* background = Sprite::create(_bgImageFile);
    background->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize()/2);
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setScale(Director::getInstance()->getVisibleSize().width/background->getContentSize().width);
    background->setColor(Color3B(*_selectedColour));
    _maskingNode->addChild(background);
}

void BrushEraser::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = touch->getLocation();
    
    float distance = _lastTouchPos.distance(touchPos);
    
    int numSprites = distance/(*_brushRadius*0.15);
    
    for(int i = 0; i < numSprites; i++)
    {
        _drawNode->drawSolidCircle(_lastTouchPos + i * ((touchPos - _lastTouchPos)/numSprites), *_brushRadius, 0, 16, Color4F::BLACK);
    }
    _lastTouchPos = touchPos;
}

void BrushEraser::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZOOMEE_AA_END
