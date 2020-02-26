//
//  BrushHighlighter.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#include "BrushHighlighter.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_ART_BEGIN

float BrushHighlighter::_alphaLevel = 0.25f;

BrushHighlighter::BrushHighlighter():Brush()
{
    _type = HIGHLIGHTER;
}

Node* BrushHighlighter::addDrawNode(const Size& visibleSize)
{
    if(_brushConfig->getSelectedPatternTransparant() != "")
    {
        _drawNode = DrawNode::create();
        _drawNode->setContentSize(visibleSize);
        _maskingNode = ClippingNode::create(_drawNode);
        Sprite* background = Sprite::create(_brushConfig->getSelectedPatternTransparant());
        background->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize()/2);
        background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        background->setScale(Director::getInstance()->getVisibleSize().width/background->getContentSize().width);
        _maskingNode->addChild(background);
        
        return _maskingNode;
    }
    else
    {
        _drawNode = DrawNode::create();
        _drawNode->setContentSize(visibleSize);
        return _drawNode;
    }
}

Node* BrushHighlighter::getDrawNode()
{
    if(_brushConfig->getSelectedPatternTransparant() != "")
    {
        return _maskingNode;
    }
    else
    {
        return _drawNode;
    }
}

void BrushHighlighter::onTouchBegin(Touch *touch, Event *event)
{
    _lastTouchPos = _drawNode->convertTouchToNodeSpace(touch);
    _lineStarted = false;
}

void BrushHighlighter::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = _drawNode->convertTouchToNodeSpace(touch);
    
    float distance = _lastTouchPos.distance(touchPos);
    float brushWidth = _brushConfig->getBrushRadius();
    if(distance > brushWidth)
    {
        Vec2 directionVector = touchPos - _lastTouchPos;
        Vec2 normal = Vec2(-directionVector.y, directionVector.x);
        normal.normalize();
        
        if(normal.y <= 0)
        {
            normal = -normal;
        }
    
    
        if(!_lineStarted)
        {
            _points[0] = _lastTouchPos + (normal*(brushWidth));
            _points[1] = _lastTouchPos - (normal*(brushWidth));
            _points[2] = touchPos - (normal*(brushWidth));
            _points[3] = touchPos + (normal*(brushWidth));
            _lineStarted = true;
        }
        else
        {
            _points[0] = _points[3];
            _points[1] = _points[2];
            _points[2] = touchPos - (normal*(brushWidth));
            _points[3] = touchPos + (normal*(brushWidth));
        }

        
        Color4F colour = _brushConfig->getSelectedColour();
        colour.a = _alphaLevel;
    
        _drawNode->drawSolidPoly(_points, 4, colour);
    
        _lastTouchPos = touchPos;
    }
}

void BrushHighlighter::onTouchEnded(Touch *touch, Event *event)
{
    _lineStarted = false;
}

NS_AZ_ART_END
