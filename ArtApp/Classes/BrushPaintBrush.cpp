//
//  BrushPaintBrush.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#include "BrushPaintBrush.h"

using namespace cocos2d;

float BrushPaintBrush::startingAlpha = 0.1f;

BrushPaintBrush::BrushPaintBrush():Brush()
{
    
}

void BrushPaintBrush::onTouchBegin(Touch *touch, Event *event)
{
    lastTouchPos = drawNode->convertTouchToNodeSpace(touch);
    lastAlpha = startingAlpha;
}

void BrushPaintBrush::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = drawNode->convertTouchToNodeSpace(touch);
    float distance = lastTouchPos.distance(touchPos);
    distance /= 4096.0f;
    float alpha = lastAlpha + distance;
    if(alpha > 1)
        alpha = 1;
    
    Color4F colour = Color4F(selectedColour->r, selectedColour->g, selectedColour->b,alpha);
    
    drawNode->drawSegment(lastTouchPos, touchPos, *brushRadius, colour);
    
    lastTouchPos = touchPos;
    lastAlpha = alpha;
}

void BrushPaintBrush::onTouchEnded(Touch *touch, Event *event)
{
    
}
