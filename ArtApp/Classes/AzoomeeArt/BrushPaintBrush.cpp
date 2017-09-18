//
//  BrushPaintBrush.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#include "BrushPaintBrush.h"

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

float BrushPaintBrush::startingAlpha = 0.1f;

BrushPaintBrush::BrushPaintBrush():Brush()
{
    
}

Node* BrushPaintBrush::addDrawNode(const Size& visibleSize)
{
    brushLayer = Node::create();
    brushLayer->setContentSize(visibleSize);
    
    return brushLayer;
}

Node* BrushPaintBrush::getDrawNode()
{
    return brushLayer;
}

void BrushPaintBrush::onTouchBegin(Touch *touch, Event *event)
{
    lastTouchPos = brushLayer->convertTouchToNodeSpace(touch);
    lastAlpha = startingAlpha;
}

void BrushPaintBrush::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = brushLayer->convertTouchToNodeSpace(touch);
    float distance = lastTouchPos.distance(touchPos);
    
    int numSprites = distance/(*brushRadius*0.15);
    
    for(int i = 0; i < numSprites; i++)
    {
        Sprite* s = Sprite::create("res/artapp/paintbrush_effect.png");
        float scale = *brushRadius/(s->getContentSize().width/2);
        s->setScale(scale);
        s->setAnchorPoint(Vec2(0.5,0.5));
        s->setPosition(lastTouchPos + i * ((touchPos - lastTouchPos)/numSprites));
        s->setColor(Color3B(*selectedColour));
        brushLayer->addChild(s);
    }
    
    //distance /= 4096.0f;
    //float alpha = lastAlpha + distance;
    //if(alpha > 1)
    //    alpha = 1;
    
    //Color4F colour = Color4F(selectedColour->r, selectedColour->g, selectedColour->b,alpha);
    
    //drawNode->drawSegment(lastTouchPos, touchPos, *brushRadius, colour);
    
    lastTouchPos = touchPos;
    //lastAlpha = alpha;
}

void BrushPaintBrush::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZOOMEE_AA_END
