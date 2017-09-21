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
    //const Size& winSize =  Director::getInstance()->getWinSize();
    //_paintLayer = RenderTexture::create(winSize.width, winSize.height);
    //_paintLayer->setAnchorPoint(Vec2(0.5,0.5));
    //_paintLayer->setPosition(winSize/2);
    //_paintLayer->clear(0, 0, 0, 0);
    
    //return _paintLayer;
    brushLayer = Node::create();
    brushLayer->setContentSize(visibleSize);
    
    return brushLayer;
}

Node* BrushPaintBrush::getDrawNode()
{
    if(_spriteCount < 1000)
    {
        return brushLayer;
    }
    
    _paintLayer = RenderTexture::create(brushLayer->getContentSize().width, brushLayer->getContentSize().height);
    _paintLayer->setPosition(Director::getInstance()->getVisibleOrigin() + (Director::getInstance()->getVisibleSize()/2));
    _paintLayer->beginWithClear(0, 0, 0, 0);
    brushLayer->setPosition(brushLayer->getPosition() - Director::getInstance()->getVisibleOrigin());
    brushLayer->visit();
    _paintLayer->end();
    Director::getInstance()->getRenderer()->render();
    
    brushLayer->getParent()->addChild(_paintLayer);
    brushLayer->removeFromParent();
    return _paintLayer;

}

void BrushPaintBrush::onTouchBegin(Touch *touch, Event *event)
{
    lastTouchPos = brushLayer->convertTouchToNodeSpace(touch);
    lastAlpha = startingAlpha;
    _spriteCount = 0;
}

void BrushPaintBrush::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = brushLayer->convertTouchToNodeSpace(touch);
    float distance = lastTouchPos.distance(touchPos);
    
    int numSprites = distance/(*brushRadius*0.15);
    //_paintLayer->begin();
    for(int i = 0; i < numSprites; i++)
    {
        Sprite* s = Sprite::create("res/artapp/paintbrush_effect.png");
        float scale = *brushRadius/(s->getContentSize().width/2);
        s->setScale(scale);
        s->setAnchorPoint(Vec2(0.5,0.5));
        s->setPosition(lastTouchPos + i * ((touchPos - lastTouchPos)/numSprites));
        s->setColor(Color3B(*selectedColour));
        brushLayer->addChild(s);
        //s->visit();
    }
    _spriteCount += numSprites;
    //_paintLayer->end();
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
