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
        //SpriteWithHue* targetSprite = nullptr;
        OomeeAccessory* targetSprite = nullptr;
        std::string targetId = "";
        const std::vector<std::string>& lockedAnchors = _oomeeData->getLockedAnchors();
        //for(auto it = _accessorySprites.begin(); it != _accessorySprites.end(); ++it)
        for(auto it = _accessories.begin(); it != _accessories.end(); ++it)
        {
            if(std::find(lockedAnchors.begin(), lockedAnchors.end(), it->first) != std::end(lockedAnchors))
            {
                continue;
            }
            const Point& locationInNode = it->second->convertToNodeSpace(touch->getLocation());
            const Size& s = it->second->getBoundingBox().size;//getContentSize();
            const Rect& rect = Rect(0,0,s.width, s.height);
            if(rect.containsPoint(locationInNode) && touchOnSpritePixelPerfect(touch, it->second->getBaseSprite()))
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
            //DragAndDropController::getInstance()->setItemData(_accessoryData.at(targetId));
            DragAndDropController::getInstance()->setItemData(targetSprite->getItemData());
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
    
    const std::string& colourId = getStringFromJson("colour", data);
    _colour = OomeeMakerDataStorage::getInstance()->getColourForKey(colourId);
    if(!_colour)
    {
        return false;
    }
    
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
    
    if(_baseSprite)
    {
        _baseSprite->removeFromParent();
    }
    
    std::map<std::string, OomeeItemRef> accessoriesHolder;
    
    for(auto item : _accessories)
    {
        accessoriesHolder[item.first] = item.second->getItemData();
    }
    //_accessoryData.clear();
    //_accessorySprites.clear();
    _accessories.clear();
    _baseSprite = OomeeBody::create();
    _baseSprite->setOomeeData(oomeeData);
    if(_colour)
    {
        _baseSprite->setColourData(_colour);
    }
    //_baseSprite = SpriteWithHue::create(OomeeMakerDataHandler::getInstance()->getAssetDir() + _oomeeData->getAssetName());
    _baseSprite->setNormalizedPosition(_oomeeData->getPosition());
    //_baseSprite->setPosition(Vec2(this->getContentSize().width * _oomeeData->getPosition().x, this->getContentSize().height * oomeeData->getPosition().y) - _baseSprite->getContentSize()/2);
    _baseSprite->setScale(_oomeeData->getScale());
    //_baseSprite->setHue(_hue);
    this->addChild(_baseSprite);
    
    for(const std::string& accId : _oomeeData->getDefaultAccessories())
    {
        const OomeeItemRef& item = OomeeMakerDataStorage::getInstance()->getOomeeItemForKey(accId);
        if(item)
        {
            addAccessory(item);
        }
    }
    
    for(auto accessoryItem : accessoriesHolder)
    {
        addAccessory(accessoryItem.second);
    }
}

OomeeRef OomeeFigure::getOomeeData() const
{
    return _oomeeData;
}

std::vector<std::string> OomeeFigure::getAccessoryIds() const
{
    std::vector<std::string> ids;
    for(auto accessory : _accessories)
    {
        ids.push_back(accessory.second->getItemId());
    }
    return ids;
}

void OomeeFigure::addAccessory(const OomeeItemRef& oomeeItem)
{
    if(oomeeItem)
    {
        const auto& incompatAccList = _oomeeData->getIncompatableAccessories();
        if(std::find(incompatAccList.begin(),incompatAccList.end(), oomeeItem->getId()) != incompatAccList.end())
        {
            return;
        }
    }
    else
    {
        return;
    }
    
    if(_baseSprite && (_oomeeData->getAnchorPoints().find(oomeeItem->getTargetAnchor()) != _oomeeData->getAnchorPoints().end()))
    {
        if(_accessories.find(oomeeItem->getTargetAnchor()) != _accessories.end())
        {
            OomeeAccessory* accessory = _accessories.at(oomeeItem->getTargetAnchor());
            if(accessory->getItemId() == oomeeItem->getId()) // accessory is already there
            {
                return;
            }
        }
        
        removeAccessory(oomeeItem->getTargetAnchor());
        OomeeAccessory* accessory = OomeeAccessory::create();
        accessory->setItemData(oomeeItem);
        if(_colour)
        {
            accessory->setColourData(_colour);
        }
        const Size& baseSpriteSize = _baseSprite->getContentSize();
        Vec2 anchorPoint = _oomeeData->getAnchorPoints().at(oomeeItem->getTargetAnchor()); // dont const& - unstable on android, caused many tears
        accessory->setPosition(Vec2(baseSpriteSize.width * anchorPoint.x, baseSpriteSize.height * anchorPoint.y) + oomeeItem->getOffset());
        accessory->setScale(oomeeItem->getTargetScale());
        _baseSprite->addChild(accessory, _baseSprite->transformZOrder(oomeeItem->getZOrder()));
        _accessories[oomeeItem->getTargetAnchor()] = accessory;
        
        for(const std::string& id : oomeeItem->getDependancies())
        {
            addAccessory(OomeeMakerDataStorage::getInstance()->getOomeeItemForKey(id));
        }
        
        dependancyCheck();
    }
}

void OomeeFigure::removeAccessory(const std::string anchorPoint)
{
    if(_accessories.find(anchorPoint) != _accessories.end())
    {
        _accessories.at(anchorPoint)->removeFromParent();
        _accessories.erase(_accessories.find(anchorPoint));
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
    Sprite* target = Sprite::create("res/oomeeMaker/1_Oomee_Reference.png");
    // render twice, first in 2x size canvas to make sure accessories dont get culled by the renderer, then again to get normal size image
    _baseSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _baseSprite->setPosition(target->getContentSize());
    _baseSprite->setScale(target->getContentSize().height / _baseSprite->getContentSize().height);
    RenderTexture* renderTex = RenderTexture::create(target->getContentSize().width*2, target->getContentSize().height*2 );
    renderTex->beginWithClear(0, 0, 0, 0);
    _baseSprite->visit();
    renderTex->end();
    Director::getInstance()->getRenderer()->render();
    
    Sprite* temp = Sprite::createWithTexture(renderTex->getSprite()->getTexture());
    temp->setFlippedY(true);
    temp->setPosition(target->getContentSize() / 2.0f);
    RenderTexture* finalTex = RenderTexture::create(target->getContentSize().width, target->getContentSize().height);
    finalTex->beginWithClear(0, 0, 0, 0);
    temp->visit();
    finalTex->end();
    finalTex->saveToFile(filepath, Image::Format::PNG);
    Director::getInstance()->getRenderer()->render();
}

OomeeColourRef OomeeFigure::getColour() const
{
    return _colour;
}

void OomeeFigure::setColour(const OomeeColourRef &colour)
{
    _colour = colour;
    if(_baseSprite)
    {
        _baseSprite->setColourData(_colour);
    }
    
    for(auto accessory : _accessories)
    {
        accessory.second->setColourData(_colour);
    }
    
}

float OomeeFigure::getHue() const
{
    return _hue;
}

void OomeeFigure::setHue(float hue)
{
    _hue = hue;
    /*if(_baseSprite)
    {
        _baseSprite->setHue(_hue);
    }
    
    for(auto accessory : _accessorySprites)
    {
        if(_accessoryData.at(accessory.first)->isUsingColourHue())
        {
            accessory.second->setHue(_hue);
        }
    }*/
}

void OomeeFigure::addDefaultAccessories()
{
    for(const std::string& itemName : kDefaultAccessories)
    {
        addAccessory(OomeeMakerDataStorage::getInstance()->getOomeeItemForKey(itemName));
    }
}

void OomeeFigure::dependancyCheck()
{
    std::vector<std::string> removeAccsAnchors;
    for(const auto& acc : _accessories)
    {
        const OomeeItemRef& itemData = acc.second->getItemData();
        for(const auto& dependancyId : itemData->getDependancies())
        {
            const OomeeItemRef& dependancyData = OomeeMakerDataStorage::getInstance()->getOomeeItemForKey(dependancyId);
            if(dependancyData && _accessories.find(dependancyData->getTargetAnchor()) != _accessories.end())
            {
                if(_accessories.at(dependancyData->getTargetAnchor())->getItemId() != dependancyId)
                {
                    removeAccsAnchors.push_back(acc.first);
                    break;
                }
            }
        }
    }
    
    for(const auto& removeAnchor : removeAccsAnchors)
    {
        removeAccessory(removeAnchor);
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
