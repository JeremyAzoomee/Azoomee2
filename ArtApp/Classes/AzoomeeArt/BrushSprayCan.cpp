//
//  BrushSprayCan.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 31/05/2017.
//
//

#include "BrushSprayCan.h"

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

//Node* BrushSprayCan::addDrawNode(const Size& visibleSize)
//{
//    DrawNode* node = DrawNode::create();
//    node->setContentSize(visibleSize);
//    node->setPosition(visibleSize/2);
//    node->setAnchorPoint(Vec2(0.5,0.5));
//    _drawNode = node;
    
//    return _drawNode;
//}

//Node* BrushSprayCan::getDrawNode()
//{
    
    /*if(_drawnElementsCount > 10000)
    {
        RenderTexture* sprayCanResult = RenderTexture::create(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height);
        sprayCanResult->setAnchorPoint(Vec2(0.5,0.5));
        sprayCanResult->setPosition(Director::getInstance()->getVisibleOrigin() + (Director::getInstance()->getVisibleSize()/2));
        sprayCanResult->beginWithClear(0, 0, 0, 0);
        _drawNode->setPosition((Vec2)(Director::getInstance()->getVisibleSize()/2) - Director::getInstance()->getVisibleOrigin());
        _drawNode->visit();
        sprayCanResult->end();
        Director::getInstance()->getRenderer()->render();
    
        _drawNode->getParent()->addChild(sprayCanResult);
        _drawNode->removeFromParent();
        return sprayCanResult;
    }
    else
    {
        return _drawNode;
    }*/
//}

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
    
    int sprayPoints = rand()%10 + *_brushRadius*2;
    
    for (int i = 0; i < sprayPoints; ++i)
    {
        float alpha = (rand() % 10)/10.0f;
    
        Color4F colour = Color4F(_selectedColour->r, _selectedColour->g, _selectedColour->b,alpha);
        
        Vec2 direction = Vec2((rand()%200-100)/100.0f, (rand()%200-100)/100.0f);
        direction.normalize();
        int xSpread = rand()%(int)*_brushRadius*4;
        int ySpread = rand()%(int)*_brushRadius*4;
        Vec2 position = Vec2(touchPos.x + direction.x*xSpread, touchPos.y + direction.y*ySpread);
        
        _drawNode->drawDot(position, rand()%6 + 1, colour);
    }
    
    _drawnElementsCount += sprayPoints;
    
    _lastTouchPos = touchPos;
}

void BrushSprayCan::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZOOMEE_AA_END
