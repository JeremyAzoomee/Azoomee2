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
    if(_spriteCount < _kSpriteCountLimit) //If less than sprite limit, just return node holding the sprites, else bake into RenderTexture object
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
    _spriteCount = 0;
}

void BrushPaintBrush::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = brushLayer->convertTouchToNodeSpace(touch);
    float distance = lastTouchPos.distance(touchPos);
    
    int numSprites = distance/(*brushRadius*0.15);
    
    for(int i = 0; i < numSprites; i++)
    {
        Sprite* s = Sprite::create(ArtAppAssetLoc + "paintbrush_effect.png");
        float scale = *brushRadius/(s->getContentSize().width/2);
        s->setScale(scale);
        s->setAnchorPoint(Vec2(0.5,0.5));
        s->setPosition(lastTouchPos + i * ((touchPos - lastTouchPos)/numSprites));
        s->setColor(Color3B(*selectedColour));
        brushLayer->addChild(s);
    }
    _spriteCount += numSprites;
    
    lastTouchPos = touchPos;
}

void BrushPaintBrush::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZOOMEE_AA_END
