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
    type = ERASER;
}

Node* BrushEraser::addDrawNode(const Size& visibleSize)
{
    drawNode = DrawNode::create();
    drawNode->setContentSize(visibleSize);
    maskingNode = ClippingNode::create(drawNode);
    Sprite* background = Sprite::create(bgImageFile);
    background->setPosition(Director::getInstance()->getVisibleOrigin() + visibleSize/2);
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setScale(visibleSize.width/background->getContentSize().width);
    maskingNode->addChild(background);
    return maskingNode;
}

Node* BrushEraser::getDrawNode()
{
    return maskingNode;
}

void BrushEraser::setBgImageFile(const std::string& filename)
{
    bgImageFile = filename;
}

void BrushEraser::onTouchBegin(Touch *touch, Event *event)
{
    lastTouchPos = drawNode->convertTouchToNodeSpace(touch);
    
    
}

void BrushEraser::onTouchMoved(Touch *touch, Event *event)
{
    Vec2 touchPos = touch->getLocation();//drawNode->convertTouchToNodeSpace(touch);
    
    float distance = lastTouchPos.distance(touchPos);
    
    int numSprites = distance/(*brushRadius*0.15);
    
    for(int i = 0; i < numSprites; i++)
    {
        drawNode->drawSolidCircle(lastTouchPos + i * ((touchPos - lastTouchPos)/numSprites), *brushRadius, 0, 16, Color4F::BLACK);
    }
    lastTouchPos = touchPos;
}

void BrushEraser::onTouchEnded(Touch *touch, Event *event)
{
    
}

NS_AZOOMEE_AA_END
