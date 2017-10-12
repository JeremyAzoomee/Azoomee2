//
//  BrushHighlighter.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#include "BrushHighlighter.h"

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

float BrushHighlighter::alphaLevel = 0.25f;

BrushHighlighter::BrushHighlighter():Brush()
{
    type = HIGHLIGHTER;
}

void BrushHighlighter::onTouchBegin(Touch *touch, Event *event)
{
    lastTouchPos = drawNode->convertTouchToNodeSpace(touch);
    lineStarted = false;
}

void BrushHighlighter::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = drawNode->convertTouchToNodeSpace(touch);
    
    float distance = lastTouchPos.distance(touchPos);
    float brushWidth = *brushRadius;
    if(distance > brushWidth)
    {
        Vec2 directionVector = touchPos - lastTouchPos;
        Vec2 normal = Vec2(-directionVector.y, directionVector.x);
        normal.normalize();
        
        if(normal.y <= 0)
            normal = -normal;
    
    
        if(!lineStarted)
        {
            points[0] = lastTouchPos + (normal*(brushWidth));
            points[1] = lastTouchPos - (normal*(brushWidth));
            points[2] = touchPos - (normal*(brushWidth));
            points[3] = touchPos + (normal*(brushWidth));
            lineStarted = true;
        }
        else
        {
            points[0] = points[3];
            points[1] = points[2];
            points[2] = touchPos - (normal*(brushWidth));
            points[3] = touchPos + (normal*(brushWidth));
        }

    
        Color4F colour = Color4F(selectedColour->r, selectedColour->g, selectedColour->b,alphaLevel);
    
        drawNode->drawSolidPoly(points, 4, colour);
    
        lastTouchPos = touchPos;
    }
}

void BrushHighlighter::onTouchEnded(Touch *touch, Event *event)
{
    lineStarted = false;
}

NS_AZOOMEE_AA_END
