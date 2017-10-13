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
    _type = PAINTBRUSH;
}

Node* BrushPaintBrush::addDrawNode(const Size& visibleSize)
{
    _brushLayer = Node::create();
    _brushLayer->setContentSize(visibleSize);
    
    return _brushLayer;
}

Node* BrushPaintBrush::getDrawNode()
{
    if(_spriteCount < _kSpriteCountLimit) //If less than sprite limit, just return node holding the sprites, else bake into RenderTexture object
    {
        return _brushLayer;
    }
    const Vec2& visibleOrigin = Director::getInstance()->getVisibleOrigin();
    _paintLayer = RenderTexture::create(_brushLayer->getContentSize().width, _brushLayer->getContentSize().height);
    _paintLayer->setPosition(visibleOrigin + (Director::getInstance()->getVisibleSize()/2));
    _paintLayer->beginWithClear(0, 0, 0, 0);
    _brushLayer->setPosition(_brushLayer->getPosition() - visibleOrigin);
    _brushLayer->visit();
    _paintLayer->end();
    Director::getInstance()->getRenderer()->render();
    
    _brushLayer->getParent()->addChild(_paintLayer);
    _brushLayer->removeFromParent();
    return _paintLayer;

}

void BrushPaintBrush::onTouchBegin(Touch *touch, Event *event)
{
    _lastTouchPos = _brushLayer->convertTouchToNodeSpace(touch);
    _spriteCount = 0;
}

void BrushPaintBrush::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = _brushLayer->convertTouchToNodeSpace(touch);
    float distance = _lastTouchPos.distance(touchPos);
    
    int numSprites = distance/(*_brushRadius*0.15);
    
    for(int i = 0; i < numSprites; i++)
    {
        Sprite* s = Sprite::create(kArtAppAssetLoc + "paintbrush_effect.png");
        float scale = *_brushRadius/(s->getContentSize().width/2);
        s->setScale(scale);
        s->setAnchorPoint(Vec2(0.5,0.5));
        s->setPosition(_lastTouchPos + i * ((touchPos - _lastTouchPos)/numSprites));
        s->setColor(Color3B(*_selectedColour));
        _brushLayer->addChild(s);
    }
    _spriteCount += numSprites;
    
    _lastTouchPos = touchPos;
}

void BrushPaintBrush::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZOOMEE_AA_END
