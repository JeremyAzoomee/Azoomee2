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

Node* BrushSprayCan::addDrawNode(Size visibleSize)
{
    DrawNode* node = DrawNode::create();
    node->setContentSize(visibleSize);
    node->setPosition(visibleSize/2);
    node->setAnchorPoint(Vec2(0.5,0.5));
    drawNode = node;
    
    return drawNode;
}

Node* BrushSprayCan::getDrawNode()
{
    
    if(drawnElementsCount > 10000)
    {
        RenderTexture* sprayCanResult = RenderTexture::create(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height);
        sprayCanResult->setAnchorPoint(Vec2(0.5,0.5));
        sprayCanResult->setPosition(Director::getInstance()->getVisibleOrigin() + (Director::getInstance()->getVisibleSize()/2));
        sprayCanResult->beginWithClear(0, 0, 0, 0);
        drawNode->setPosition((Vec2)(Director::getInstance()->getVisibleSize()/2) - Director::getInstance()->getVisibleOrigin());
        drawNode->visit();
        sprayCanResult->end();
        Director::getInstance()->getRenderer()->render();
    
        drawNode->getParent()->addChild(sprayCanResult);
        drawNode->removeFromParent();
        return sprayCanResult;
    }
    else
    {
        return drawNode;
    }
}

BrushSprayCan::BrushSprayCan():Brush()
{
    
}

void BrushSprayCan::onTouchBegin(Touch *touch, Event *event)
{
    lastTouchPos = drawNode->convertTouchToNodeSpace(touch);
    srand((int)time(NULL));
    
    drawnElementsCount = 0;
    
}

void BrushSprayCan::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = drawNode->convertTouchToNodeSpace(touch);
    
    int sprayPoints = rand()%10 + *brushRadius*2;
    
    for (int i = 0; i < sprayPoints; ++i)
    {
        float alpha = (rand() % 10)/10.0f;
    
        Color4F colour = Color4F(selectedColour->r, selectedColour->g, selectedColour->b,alpha);
        
        Vec2 direction = Vec2((rand()%200-100)/100.0f, (rand()%200-100)/100.0f);
        direction.normalize();
        int xSpread = rand()%(int)*brushRadius*4;
        int ySpread = rand()%(int)*brushRadius*4;
        Vec2 position = Vec2(touchPos.x + direction.x*xSpread, touchPos.y + direction.y*ySpread);
        
        drawNode->drawDot(position, rand()%6 + 1, colour);
    }
    
    drawnElementsCount += sprayPoints;
    
    lastTouchPos = touchPos;
}

void BrushSprayCan::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZOOMEE_AA_END
