//
//  BrushPen.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#include "BrushPen.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_AA_BEGIN

BrushPen::BrushPen()
{
    _type = PEN;
}

void BrushPen::onTouchBegin(Touch *touch, Event *event)
{
    _lastTouchPos = _drawNode->convertTouchToNodeSpace(touch);
    if(_brushConfig->getSelectedPattern() != "")
    {
        _drawNode->drawSolidCircle(_lastTouchPos, _brushConfig->getBrushRadius(), 0, 16, Color4F::BLACK);
    }
    else
    {
        _drawNode->drawDot(_lastTouchPos, _brushConfig->getBrushRadius(), _brushConfig->getSelectedColour());
    }
    
}

void BrushPen::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = _drawNode->convertTouchToNodeSpace(touch);
    
    if(_brushConfig->getSelectedPattern() != "")
    {
        float distance = _lastTouchPos.distance(touchPos);
        
        int numSprites = distance/(_brushConfig->getBrushRadius()*0.15);
        
        for(int i = 0; i < numSprites; i++)
        {
            _drawNode->drawSolidCircle(_lastTouchPos + i * ((touchPos - _lastTouchPos)/numSprites), _brushConfig->getBrushRadius(), 0, 16, Color4F::BLACK);
        }

    }
    else
    {
        _drawNode->drawSegment(_lastTouchPos, touchPos, _brushConfig->getBrushRadius(), _brushConfig->getSelectedColour());
    }
    
    _lastTouchPos = touchPos;
}

void BrushPen::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZ_AA_END
