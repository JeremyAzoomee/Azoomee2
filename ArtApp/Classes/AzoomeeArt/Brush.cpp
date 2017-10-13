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

Node* Brush::addDrawNode(const Size& visibleSize)
{
    DrawNode* node = DrawNode::create();
    node->setContentSize(visibleSize);
    _drawNode = node;
    return node;
}

Node* Brush::getDrawNode()
{
    return _drawNode;
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
