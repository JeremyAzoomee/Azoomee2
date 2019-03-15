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
        OomeeAccessory* targetSprite = nullptr;
        std::string targetId = "";
        const std::vector<std::string>& lockedAnchors = _oomeeData->getLockedAnchors();
        for(auto it = _accessories.begin(); it != _accessories.end(); ++it)
        {
            if(std::find(lockedAnchors.begin(), lockedAnchors.end(), it->first) != std::end(lockedAnchors))
            {
                continue;
            }
            const Point& locationInNode = it->second->convertToNodeSpace(touch->getLocation());
            const Size& s = it->second->getBoundingBox().size;
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
            DragAndDropController::getInstance()->setItemData(targetSprite->getItemData());
            DragAndDropController::getInstance()->setRemoveItemFromOomee();
            _removingItem = true;
            return true;
        }
        _removingItem = false;
        return false;
    };
    
    _touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        if(_removingItem)
        {
            _undoStack.push_back(getDataSnapshot());
            _removingItem = false;
        }
    };
    
    _touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        if(_removingItem)
        {
            _undoStack.push_back(getDataSnapshot());
            _removingItem = false;
        }
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
    
    //const std::string& colourId = getStringFromJson("colour", data);
    //_colour = OomeeMakerDataStorage::getInstance()->getColourForKey(colourId);
    //if(!_colour)
    //{
    //    return false;
    //}
    
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
    _undoStack.clear();
    _undoStack.push_back(getDataSnapshot());
    
    return true;
}

void OomeeFigure::setOomeeData(const OomeeRef& oomeeData)
{
    auto undoStackSize = _undoStack.size();
    _oomeeData = oomeeData;
    
    if(_baseSprite)
    {
        _baseSprite->removeFromParent();
    }
    
    std::map<std::string, OomeeItemRef> accessoriesHolder;
    
    for(const auto& item : _accessories)
    {
        accessoriesHolder[item.first] = item.second->getItemData();
    }

    _accessories.clear();
    _baseSprite = OomeeBody::create();
    _baseSprite->setOomeeData(oomeeData);
    //if(_colour)
    //{
    //    _baseSprite->setColourData(_colour);
    //}

    _baseSprite->setNormalizedPosition(_oomeeData->getPosition());
    _baseSprite->setScale(_oomeeData->getScale());
    this->addChild(_baseSprite);
    
    for(const std::string& accId : _oomeeData->getDefaultAccessories())
    {
        const OomeeItemRef& item = OomeeMakerDataStorage::getInstance()->getOomeeItemForKey(accId);
        if(item)
        {
            addAccessory(item);
        }
    }
    
    for(const auto& accessoryItem : accessoriesHolder)
    {
        addAccessory(accessoryItem.second);
    }
    
    while(_undoStack.size() > undoStackSize)// oomee change counts as single undoabe action
    {
        _undoStack.pop_back();
    }
    
    _undoStack.push_back(getDataSnapshot());
}

OomeeRef OomeeFigure::getOomeeData() const
{
    return _oomeeData;
}

std::vector<std::string> OomeeFigure::getAccessoryIds() const
{
    std::vector<std::string> ids;
    for(const auto& accessory : _accessories)
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
        //if(_colour)
        //{
        //    accessory->setColourData(_colour);
        //}
		accessory->setColourData(_oomeeData->getColour());
		
        const Size& baseSpriteSize = _baseSprite->getContentSize();
        Vec2 anchorPoint = _oomeeData->getAnchorPoints().at(oomeeItem->getTargetAnchor()); // dont const& - unstable on android, caused many tears
        accessory->setPosition(Vec2(baseSpriteSize.width * anchorPoint.x, baseSpriteSize.height * anchorPoint.y) + oomeeItem->getOffset());
        accessory->setScale(oomeeItem->getTargetScale());
        _baseSprite->addChild(accessory, _baseSprite->transformZOrder(oomeeItem->getZOrder()));
        _accessories[oomeeItem->getTargetAnchor()] = accessory;
        
        for(const std::string& id : oomeeItem->getDependancies())
        {
            auto undoStackSize = _undoStack.size();
            addAccessory(OomeeMakerDataStorage::getInstance()->getOomeeItemForKey(id));
            while(_undoStack.size() > undoStackSize) // might not have added accessory / action to stack, so check if size has changed
            {
                _undoStack.pop_back(); // dont count recursive additions as unique actions
            }
        }
        
        dependancyCheck();
        _undoStack.push_back(getDataSnapshot()); // add to undo stack after recursive accessory addition and dependancy cleanup
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

void OomeeFigure::resetOomee()
{
    std::vector<std::string> anchorNames;
    for(auto item : _accessories)
    {
        anchorNames.push_back(item.first);
    }
    for(auto anchor : anchorNames)
    {
        removeAccessory(anchor);
    }
    setOomeeData(_oomeeData);

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
    auto prevPos = _baseSprite->getPosition();
    auto prevScale = _baseSprite->getScale();
    auto prevAnchor = _baseSprite->getAnchorPoint();
    
    Sprite* target = Sprite::create("res/oomeeMaker/1_Oomee_Reference.png");
    
    const Size& targetSize = target->getContentSize();
    
    _baseSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _baseSprite->setPosition(Vec2(targetSize.width/2,0));
    _baseSprite->setScale(0.75);
    RenderTexture* renderTex = RenderTexture::create(targetSize.width, targetSize.height);
    renderTex->beginWithClear(0, 0, 0, 0);
    _baseSprite->visit();
    renderTex->end();
    renderTex->saveToFile(filepath, Image::Format::PNG);
    Director::getInstance()->getRenderer()->render();
    
    _baseSprite->setPosition(prevPos);
    _baseSprite->setAnchorPoint(prevAnchor);
    _baseSprite->setScale(prevScale);
}

OomeeColourRef OomeeFigure::getColour() const
{
    return _oomeeData->getColour();
}

float OomeeFigure::getHue() const
{
    return _hue;
}

void OomeeFigure::setHue(float hue)
{
    _hue = hue;
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

OomeeDataSnapshot OomeeFigure::getDataSnapshot()
{
    OomeeDataSnapshot snapshot;
    snapshot._oomeeData = _oomeeData;
    for(const auto& acc : _accessories )
    {
        snapshot._accessoryData.push_back(acc.second->getItemData());
    }
    return snapshot;
}

void OomeeFigure::loadDataSnapshot(const OomeeDataSnapshot &dataSnapshot)
{
    auto undoStackSize = _undoStack.size();
    setOomeeData(dataSnapshot._oomeeData);
    for(const auto& anchorPoint : _oomeeData->getAnchorPoints())
    {
        removeAccessory(anchorPoint.first);
    }
    for(const auto& acc : dataSnapshot._accessoryData)
    {
        addAccessory(acc);
    }
    
    while(_undoStack.size() > undoStackSize) // reverse adding snapshots back onto the undo stack
    {
        _undoStack.pop_back();
    }
    _undoStack.push_back(getDataSnapshot()); // reset last snapshot to current state
}

void OomeeFigure::undoLastAction()
{
    // snapshot at top of stack is current state
    if(_undoStack.size() >= 2)
    {
        _undoStack.pop_back(); //remove current state, this is updated to new state after loading old snapshot
        auto targetState = _undoStack.back(); //get previous state
        _undoStack.pop_back(); // remove previous state from stack
        loadDataSnapshot(targetState); // load previous state
    }
}

int OomeeFigure::getUndoStackSize() const
{
    return (int)_undoStack.size();
}

Vec2 OomeeFigure::getWorldPositionForAnchorPoint(const std::string &anchorPoint)
{
    return getParent()->convertToWorldSpace(getPosition()) + getLocalPositionForAnchorPoint(anchorPoint);
}

Vec2 OomeeFigure::getLocalPositionForAnchorPoint(const std::string& anchorPoint)
{
    const Size& layerSize = getContentSize();
    const Vec2& baseSpriteSize = _baseSprite->getContentSize() * _baseSprite->getScale();
    Vec2 anchor = _oomeeData->getAnchorPoints().at(anchorPoint); // dont const& - unstable on android, caused many tears
    return Vec2(layerSize.width * _baseSprite->getNormalizedPosition().x, layerSize.height * _baseSprite->getNormalizedPosition().y) + Vec2(baseSpriteSize.x * anchor.x, baseSpriteSize.y * anchor.y) - (baseSpriteSize / 2.0f);
}

NS_AZOOMEE_OM_END
