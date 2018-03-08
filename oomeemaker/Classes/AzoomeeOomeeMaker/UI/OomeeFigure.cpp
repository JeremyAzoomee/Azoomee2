//
//  OomeeFigure.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 19/02/2018.
//

#include "OomeeFigure.h"
#include "DragAndDropController.h"
#include "../DataObjects/OomeeMakerDataStorage.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include <AzoomeeCommon/Utils/SpriteUtils.h>

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

bool OomeeFigure::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        log("touch Began OF");
        SpriteWithHue* targetSprite = nullptr;
        std::string targetId = "";
        for(auto it = _accessorySprites.begin(); it != _accessorySprites.end(); ++it)
        {
            const Point& locationInNode = it->second->convertToNodeSpace(touch->getLocation());
            const Size& s = it->second->getBoundingBox().size;//getContentSize();
            const Rect& rect = Rect(0,0,s.width, s.height);
            if(rect.containsPoint(locationInNode) && touchOnSpritePixelPerfect(touch, it->second))
            {
                if(!targetSprite)
                {
                    targetSprite = it->second;
                    targetId = it->first;
                }
                else
                {
                    if(targetSprite->getLocalZOrder() < it->second->getLocalZOrder())
                    {
                        targetSprite = it->second;
                        targetId = it->first;
                    }
                }
            }
        }
        
        if(targetSprite)
        {
            DragAndDropController::getInstance()->setItemData(_accessoryData.at(targetId));
            DragAndDropController::getInstance()->setRemoveItemFromOomee();
            return true;
        }
        
        return false;
    };
    
    _touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        
    };
    
    _touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        
    };
    
    _touchListener->onTouchCancelled = _touchListener->onTouchEnded;
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    
    return true;
}

void OomeeFigure::onEnter()
{
    if(_touchListener)
    {
        _touchListener->setEnabled(_isEditable);
    }
    
    Super::onEnter();
}

bool OomeeFigure::initWithOomeeFigureData(const rapidjson::Document &data)
{
    if(data.HasParseError())
    {
        return false;
    }
    
    const std::string& oomeeKey = getStringFromJson("oomee", data);
    
    _hue = getFloatFromJson("colourHue", data);
    
    const OomeeRef& oomee = OomeeMakerDataStorage::getInstance()->getOomeeForKey(oomeeKey);
    if(oomee)
    {
        setOomeeData(oomee);
    }
    else
    {
        return false;
    }
    
    if(!_baseSprite)
    {
        return false;
    }
    
    if(data.HasMember("oomeeItems"))
    {
        const std::vector<std::string>& itemKeys = getStringArrayFromJson(data["oomeeItems"]);
        for(const std::string& key : itemKeys)
        {
            const OomeeItemRef& item = OomeeMakerDataStorage::getInstance()->getOomeeItemForKey(key);
            if(item)
            {
                addAccessory(item);
            }
        }
    }
    
    return true;
}

void OomeeFigure::setOomeeData(const OomeeRef& oomeeData)
{
    _oomeeData = oomeeData;
    
    _accessoryData.clear();
    _accessorySprites.clear();
    
    if(_baseSprite)
    {
        //TODO - process transition of accessories here
        
        _baseSprite->removeFromParent();
    }
    
    _baseSprite = SpriteWithHue::create(OomeeMakerDataHandler::getInstance()->getAssetDir() + _oomeeData->getAssetName());
    _baseSprite->setNormalizedPosition(_oomeeData->getPosition());
    _baseSprite->setScale(_oomeeData->getScale());
    _baseSprite->setHue(_hue);
    this->addChild(_baseSprite);
}

OomeeRef OomeeFigure::getOomeeData() const
{
    return _oomeeData;
}

std::vector<std::string> OomeeFigure::getAccessoryIds() const
{
    std::vector<std::string> ids;
    for(auto accessory : _accessoryData)
    {
        ids.push_back(accessory.second->getId());
    }
    return ids;
}

void OomeeFigure::addAccessory(const OomeeItemRef& oomeeItem)
{
    if(_baseSprite && (_oomeeData->getAnchorPoints().find(oomeeItem->getTargetAnchor()) != _oomeeData->getAnchorPoints().end()) )
    {
        removeAccessory(oomeeItem->getTargetAnchor());
        _accessoryData[oomeeItem->getTargetAnchor()] = oomeeItem;
        SpriteWithHue* item = SpriteWithHue::create(OomeeMakerDataHandler::getInstance()->getAssetDir() + oomeeItem->getAssetName());
        const Size& baseSpriteSize = _baseSprite->getContentSize();
        Vec2 anchorPoint = _oomeeData->getAnchorPoints().at(oomeeItem->getTargetAnchor()); // dont const& - unstable on android, caused many tears
        item->setPosition(Vec2(baseSpriteSize.width * anchorPoint.x, baseSpriteSize.height * anchorPoint.y) + oomeeItem->getOffset());
        item->setScale(oomeeItem->getTargetScale());
        if(oomeeItem->isUsingColourHue())
        {
            item->setHue(_hue);
        }
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

void OomeeFigure::setEditable(bool isEditable)
{
    _isEditable = isEditable;
    if(_touchListener)
    {
        _touchListener->setEnabled(_isEditable);
    }
}

void OomeeFigure::saveSnapshotImage(const std::string &filepath)
{
    _baseSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _baseSprite->setPosition(_baseSprite->getContentSize());
    _baseSprite->setScale(1);
    RenderTexture* renderTex = RenderTexture::create(_baseSprite->getContentSize().width*2, _baseSprite->getContentSize().height*2 );
    renderTex->beginWithClear(0, 0, 0, 0);
    _baseSprite->visit();
    renderTex->end();
    Director::getInstance()->getRenderer()->render();
    
    Sprite* temp = Sprite::createWithTexture(renderTex->getSprite()->getTexture());
    temp->setFlippedY(true);
    temp->setPosition(_baseSprite->getContentSize() / 2.0f);
    RenderTexture* finalTex = RenderTexture::create(_baseSprite->getContentSize().width, _baseSprite->getContentSize().height);
    finalTex->beginWithClear(0, 0, 0, 0);
    temp->visit();
    finalTex->end();
    finalTex->saveToFile(filepath, Image::Format::PNG);
    Director::getInstance()->getRenderer()->render();
}

float OomeeFigure::getHue() const
{
    return _hue;
}

void OomeeFigure::setHue(float hue)
{
    _hue = hue;
    if(_baseSprite)
    {
        _baseSprite->setHue(_hue);
    }
    
    for(auto accessory : _accessorySprites)
    {
        if(_accessoryData.at(accessory.first)->isUsingColourHue())
        {
            accessory.second->setHue(_hue);
        }
    }
}

Vec2 OomeeFigure::getWorldPositionForAnchorPoint(const std::string &anchorPoint)
{
    return this->getParent()->convertToWorldSpace(this->getPosition()) + getLocalPositionForAnchorPoint(anchorPoint);
}

Vec2 OomeeFigure::getLocalPositionForAnchorPoint(const std::string& anchorPoint)
{
    const Vec2& baseSpriteSize = _baseSprite->getContentSize() * _baseSprite->getScale();
    Vec2 anchor = _oomeeData->getAnchorPoints().at(anchorPoint); // dont const& - unstable on android, caused many tears
    return Vec2(this->getContentSize().width * _baseSprite->getNormalizedPosition().x, this->getContentSize().height * _baseSprite->getNormalizedPosition().y) + Vec2(baseSpriteSize.x * anchor.x, baseSpriteSize.y * anchor.y) - (baseSpriteSize / 2.0f);
}

NS_AZOOMEE_OM_END
