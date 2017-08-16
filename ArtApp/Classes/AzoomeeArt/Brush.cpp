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
    drawNode = DrawNode::create();
    
    return true;
}

void Brush::setupDrawNode(const Size& visibleSize)
{
    drawNode->setContentSize(visibleSize);
}

void Brush::setSelectedColour(Color4F *selectedColour)
{
    this->selectedColour = selectedColour;
}

void Brush::setBrushRadius(float *brushRadius)
{
    this->brushRadius = brushRadius;
}

Node* Brush::addDrawNode(const Size& visibleSize)
{
    DrawNode* node = DrawNode::create();
    node->setContentSize(visibleSize);
    drawNode = node;
    return node;
}

Node* Brush::getDrawNode()
{
    return this->drawNode;
}

void Brush::clearBrushDrawing()
{
    drawNode->clear();
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
