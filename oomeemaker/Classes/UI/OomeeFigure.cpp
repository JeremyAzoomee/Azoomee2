//
//  OomeeFigure.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "OomeeFigure.h"
#include "DragAndDropController.h"

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
    static bool removingItem = false;
    static std::string anchorName = "";
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        for(auto it = _accessorySprites.begin(); it != _accessorySprites.end(); ++it)
        {
            const Point& locationInNode = it->second->convertToNodeSpace(touch->getLocation());
            const Size& s = it->second->getBoundingBox().size;//getContentSize();
            const Rect& rect = Rect(0,0,s.width, s.height);
            if(rect.containsPoint(locationInNode))
            {
                DragAndDropController::getInstance()->setItemData(_accessoryData.at(it->first));
                removingItem = true;
                anchorName = it->first;
                return true;
            }
        }
        removingItem = false;
        anchorName = "";
        return false;
    };
    
    _touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        if(removingItem)
        {
            this->removeAccessory(anchorName);
            removingItem = false;
            anchorName = "";
        }
    };
    
    _touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        
    };
    
    _touchListener->onTouchCancelled = _touchListener->onTouchEnded;
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    
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

OomeeRef OomeeFigure::getOomeeData() const
{
    return _oomeeData;
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

Vec2 OomeeFigure::getWorldPositionForAnchorPoint(const std::string &anchorPoint)
{
    return this->getPosition() + getLocalPositionForAnchorPoint(anchorPoint);
}

Vec2 OomeeFigure::getLocalPositionForAnchorPoint(const std::string& anchorPoint)
{
    const Vec2& baseSpriteSize = _baseSprite->getContentSize() * _baseSprite->getScale();
    const Vec2& anchor = _oomeeData->getAnchorPoints()[anchorPoint];
    return Vec2(this->getContentSize().width * _baseSprite->getNormalizedPosition().x, this->getContentSize().height * _baseSprite->getNormalizedPosition().y) + Vec2(baseSpriteSize.x * anchor.x, baseSpriteSize.y * anchor.y) - (baseSpriteSize / 2.0f);
}

NS_AZOOMEE_OM_END
