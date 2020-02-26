//
//  BrushSprayCan.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 31/05/2017.
//
//

#include "BrushSprayCan.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_ART_BEGIN

BrushSprayCan::BrushSprayCan():Brush()
{
    _type = SPRAYCAN;
}

void BrushSprayCan::onTouchBegin(Touch *touch, Event *event)
{
    _lastTouchPos = _drawNode->convertTouchToNodeSpace(touch);
    srand((int)time(NULL));
    
    _drawnElementsCount = 0;
    
}

void BrushSprayCan::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = _drawNode->convertTouchToNodeSpace(touch);
    
    const float brushRad = _brushConfig->getBrushRadius();
    
    int sprayPoints = rand()%10 + brushRad*2;
    
    for (int i = 0; i < sprayPoints; ++i)
    {
        float alpha = (rand() % 10)/10.0f;
        
        Color4F colour = _brushConfig->getSelectedColour();
        colour.a = alpha;
        
        Vec2 direction = Vec2((rand()%200-100)/100.0f, (rand()%200-100)/100.0f);
        direction.normalize();
        int xSpread = rand()%(int)brushRad*4;
        int ySpread = rand()%(int)brushRad*4;
        Vec2 position = Vec2(touchPos.x + direction.x*xSpread, touchPos.y + direction.y*ySpread);
        
        _drawNode->drawDot(position, rand()%6 + 1, colour);
    }
    
    _drawnElementsCount += sprayPoints;
    
    _lastTouchPos = touchPos;
}

void BrushSprayCan::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZ_ART_END
