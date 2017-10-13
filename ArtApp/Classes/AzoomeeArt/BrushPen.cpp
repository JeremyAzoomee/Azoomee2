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
    _type = PEN;
}

void BrushPen::onTouchBegin(Touch *touch, Event *event)
{
    _lastTouchPos = _drawNode->convertTouchToNodeSpace(touch);
    _drawNode->drawDot(_lastTouchPos, *_brushRadius, *_selectedColour);
}

void BrushPen::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = _drawNode->convertTouchToNodeSpace(touch);
    
    _drawNode->drawSegment(_lastTouchPos, touchPos, *_brushRadius, *_selectedColour);
    
    _lastTouchPos = touchPos;
}

void BrushPen::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZOOMEE_AA_END
