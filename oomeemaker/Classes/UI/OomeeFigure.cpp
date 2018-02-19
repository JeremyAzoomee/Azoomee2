//
//  OomeeFigure.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "OomeeFigure.h"

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

bool OomeeFigure::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void OomeeFigure::onEnter()
{
    Super::onEnter();
}

void OomeeFigure::setOomeeData(const OomeeRef& oomeeData)
{
    _oomeeData = oomeeData;
    
    _accessoryData.clear();
    _accessorySprites.clear();
    
    if(_baseSprite)
    {
        _baseSprite->removeFromParent();
    }
    
    _baseSprite = Sprite::create(_oomeeData->getAssetName());
    _baseSprite->setNormalizedPosition(_oomeeData->getPosition());
    _baseSprite->setScale(_oomeeData->getScale());
    this->addChild(_baseSprite);
}

void OomeeFigure::addAccessory(const OomeeItemRef& oomeeItem)
{
    if(_baseSprite && (_oomeeData->getAnchorPoints().find(oomeeItem->getTargetAnchor()) != _oomeeData->getAnchorPoints().end()) )
    {
        removeAccessory(oomeeItem->getTargetAnchor());
        _accessoryData[oomeeItem->getTargetAnchor()] = oomeeItem;
        Sprite* item = Sprite::create(oomeeItem->getAssetName());
        const Size& baseSpriteSize = _baseSprite->getContentSize();
        const Vec2& anchorPoint = _oomeeData->getAnchorPoints()[oomeeItem->getTargetAnchor()];
        item->setPosition(Vec2(baseSpriteSize.width * anchorPoint.x, baseSpriteSize.height * anchorPoint.y) + oomeeItem->getOffset());
        item->setScale(oomeeItem->getTargetScale());
        _baseSprite->addChild(item, oomeeItem->getZOrder());
        _accessorySprites[oomeeItem->getTargetAnchor()] = item;
    }
}

void OomeeFigure::removeAccessory(const std::string anchorPoint)
{
    if(_accessorySprites.find(anchorPoint) != _accessorySprites.end())
    {
        _accessorySprites.at(anchorPoint)->removeFromParent();
        _accessorySprites.erase(_accessorySprites.find(anchorPoint));
    }
    
    if(_accessoryData.find(anchorPoint) != _accessoryData.end())
    {
        _accessoryData.erase(_accessoryData.find(anchorPoint));
    }
}

NS_AZOOMEE_OM_END
