//
//  BrushEraser.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 02/06/2017.
//
//

#include "BrushEraser.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

BrushEraser::BrushEraser():Brush()
{
    
}

void BrushEraser::onTouchBegin(Touch *touch, Event *event)
{
    lastTouchPos = drawNode->convertTouchToNodeSpace(touch);
}

void BrushEraser::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = drawNode->convertTouchToNodeSpace(touch);
    
    drawNode->drawSegment(lastTouchPos, touchPos, *brushRadius, Style::Color_4F::white);
    
    lastTouchPos = touchPos;
}

void BrushEraser::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZOOMEE_AA_END
