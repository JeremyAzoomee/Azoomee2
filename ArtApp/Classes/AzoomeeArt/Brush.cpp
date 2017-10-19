//
//  Brush.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#include "Brush.h"

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

Brush::Brush()
{
    
}

bool Brush::init()
{
    _drawNode = DrawNode::create();
    
    return true;
}

void Brush::setupDrawNode(const Size& visibleSize)
{
    _drawNode->setContentSize(visibleSize);
}

void Brush::setSelectedColour(Color4F *selectedColour)
{
    _selectedColour = selectedColour;
}

void Brush::setBrushRadius(float *brushRadius)
{
    _brushRadius = brushRadius;
}

void Brush::setBgImageFile(std::string* filename)
{
    _bgImageFile = filename;
}

Node* Brush::addDrawNode(const Size& visibleSize)
{
    if(*_bgImageFile != "")
    {
        _drawNode = DrawNode::create();
        _drawNode->setContentSize(visibleSize);
        _maskingNode = ClippingNode::create(_drawNode);
        Sprite* background = Sprite::create(*_bgImageFile);
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

Node* Brush::getDrawNode()
{
    if(*_bgImageFile != "")
    {
        return _maskingNode;
    }
    else
    {
        return _drawNode;
    }
}

void Brush::clearBrushDrawing()
{
    _drawNode->clear();
}

void Brush::onTouchBegin(Touch *touch, Event *event)
{
    //override in child classes
}

void Brush::onTouchMoved(Touch *touch, Event *event)
{
    //override in child classes
}

void Brush::onTouchEnded(Touch *touch, Event *event)
{
    //override in child classes
}

NS_AZOOMEE_AA_END
