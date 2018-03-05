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

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

bool OomeeFigure::init()
{
    if(!Super::init())
    {
        return false;
    }
    
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
    
    const OomeeRef& oomee = OomeeMakerDataStorage::getInstance()->getOomeeForKey(oomeeKey);
    if(oomee)
    {
        setOomeeData(oomee);
    }
    else
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
        //process transition of accessories here
        
        _baseSprite->removeFromParent();
    }
    
    _baseSprite = Sprite::create(OomeeMakerDataHandler::getInstance()->getAssetDir() + _oomeeData->getAssetName());
    _baseSprite->setNormalizedPosition(_oomeeData->getPosition());
    _baseSprite->setScale(_oomeeData->getScale());
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
        Sprite* item = Sprite::create(OomeeMakerDataHandler::getInstance()->getAssetDir() + oomeeItem->getAssetName());
        const Size& baseSpriteSize = _baseSprite->getContentSize();
        const Vec2& anchorPoint = _oomeeData->getAnchorPoints()[oomeeItem->getTargetAnchor()];
        item->setPosition(Vec2(baseSpriteSize.width * anchorPoint.x, baseSpriteSize.height * anchorPoint.y) + oomeeItem->getOffset() * _oomeeData->getScale());
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

Vec2 OomeeFigure::getWorldPositionForAnchorPoint(const std::string &anchorPoint)
{
    return this->getParent()->convertToWorldSpace(this->getPosition()) + getLocalPositionForAnchorPoint(anchorPoint);
}

Vec2 OomeeFigure::getLocalPositionForAnchorPoint(const std::string& anchorPoint)
{
    const Vec2& baseSpriteSize = _baseSprite->getContentSize() * _baseSprite->getScale();
    const Vec2& anchor = _oomeeData->getAnchorPoints()[anchorPoint];
    return Vec2(this->getContentSize().width * _baseSprite->getNormalizedPosition().x, this->getContentSize().height * _baseSprite->getNormalizedPosition().y) + Vec2(baseSpriteSize.x * anchor.x, baseSpriteSize.y * anchor.y) - (baseSpriteSize / 2.0f);
}

NS_AZOOMEE_OM_END
