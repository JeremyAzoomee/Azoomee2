//
//  BrushPen.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#include "BrushPen.h"

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

BrushPen::BrushPen()
{
    
}

void BrushPen::onTouchBegin(Touch *touch, Event *event)
{
    lastTouchPos = drawNode->convertTouchToNodeSpace(touch);
    drawNode->drawDot(lastTouchPos, *brushRadius, *selectedColour);
}

void BrushPen::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = drawNode->convertTouchToNodeSpace(touch);
    
    drawNode->drawSegment(lastTouchPos, touchPos, *brushRadius, *selectedColour);
    
    lastTouchPos = touchPos;
}

void BrushPen::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZOOMEE_AA_END
