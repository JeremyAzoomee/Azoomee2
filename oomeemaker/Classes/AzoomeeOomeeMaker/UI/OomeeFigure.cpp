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
#include <cocos/editor-support/spine/AttachmentVertices.h>

using namespace cocos2d;

static unsigned short quadTriangles[6] = {0, 1, 2, 2, 3, 0};

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
            /*const Point& locationInNode = it->second->convertToNodeSpace(touch->getLocation());
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
            }*/
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
	if(_spineTest)
	{
		_spineTest->removeFromParent();
	}
    
    std::map<std::string, OomeeItemRef> accessoriesHolder;
    
    for(const auto& item : _accessories)
    {
        accessoriesHolder[item.first] = item.second;
    }

    _accessories.clear();
    _baseSprite = OomeeBody::create();
    _baseSprite->setOomeeData(oomeeData);

    _baseSprite->setNormalizedPosition(_oomeeData->getPosition());
    _baseSprite->setScale(_oomeeData->getScale());
    this->addChild(_baseSprite);
	_baseSprite->setVisible(false);
	
	_spineTest = spine::SkeletonAnimation::createWithJsonFile("res/spineTest/skeleton.json", "res/spineTest/skeleton.atlas");
	_spineTest->setAnimation(0, "fly", false);
	_spineTest->setOpacityModifyRGB(true);
	//_spineTest->setNormalizedPosition(_oomeeData->getPosition());
	_spineTest->setPosition(Vec2(_oomeeData->getPosition().x * this->getContentSize().width, _oomeeData->getPosition().y * this->getContentSize().height));
	_spineTest->setScale(_oomeeData->getScale());
	_spineTest->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//_spineTest->setDebugSlotsEnabled(true);
	//_spineTest->setDebugBonesEnabled(true);
	
	for(auto layer : _oomeeData->getColour()->getColours())
	{
		auto slotName = layer.first;
		//spSkeleton_setAttachment(_spineTest->getSkeleton(), slotName.c_str(), "");
		spSlot* slot = spSkeleton_findSlot(_spineTest->getSkeleton(), slotName.c_str());
		if(slot)
		{
			spSlot_setAttachment(slot, nullptr);
		}
	}
	
	for(auto asset : _oomeeData->getAssetSet())
	{
		auto colourName = asset.getTag();
		auto colour = _oomeeData->getColour()->getColours().at(colourName);
		spSkeleton* skel = _spineTest->getSkeleton();
		int targetSlot = spSkeleton_findSlotIndex(skel, colourName.c_str());
		spRegionAttachment* attachment = (spRegionAttachment*)spSkeleton_getAttachmentForSlotName(skel, colourName.c_str(), colourName.c_str());
		//spRegionAttachment* attachment = (spRegionAttachment*)spSkin_getAttachment(skel->data->defaultSkin, targetSlot, colourName.c_str());
		if(!attachment)
		{
			attachment = spRegionAttachment_create(colourName.c_str());
		}
		
		Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(OomeeMakerDataHandler::getInstance()->getAssetDir() + asset.getLocation());
		cocos2d::Size sz = texture->getContentSizeInPixels();
		texture->retain();
		
		attachment->regionOffsetX = 0;
		attachment->regionOffsetY = 0;
		attachment->regionWidth = sz.width;
		attachment->regionHeight = sz.height;
		attachment->regionOriginalWidth = attachment->regionWidth; // same if not doing whitespace stripping
		attachment->regionOriginalHeight = attachment->regionHeight;
		
		
		//attachment->x = 0;
		//attachment->y = 0;
		//attachment->scaleX = 1.0f;
		//attachment->scaleY = 1.0f;
		//attachment->rotation = 0;
		attachment->width = sz.width;
		attachment->height = sz.height;
		
		if(colourName != "none")
		{
			attachment->r = colour.r / 255.0f;
			attachment->g = colour.g / 255.0f;
			attachment->b = colour.b / 255.0f;
		}
		attachment->a = 1;
		spRegionAttachment_setUVs(attachment, 0, 0, 1, 1, 0);
		
		//unsigned short quadTriangles[6] = {0, 1, 2, 2, 3, 0};
		//spine::AttachmentVertices* attachmentVertices = new spine::AttachmentVertices(texture, 4, quadTriangles, 6);
		spine::AttachmentVertices* attachmentVertices = (spine::AttachmentVertices*)attachment->rendererObject;
		V3F_C4B_T2F* vertices = attachmentVertices->_triangles->verts;
		for (int i = 0, ii = 0; i < 4; ++i, ii += 2) {
			vertices[i].texCoords.u = attachment->uvs[ii];
			vertices[i].texCoords.v = attachment->uvs[ii + 1];
		}
		//attachmentVertices->_texture->release();
		attachmentVertices->_texture = texture;
		attachment->rendererObject = attachmentVertices;
		spRegionAttachment_updateOffset(attachment);
		//spSlot_setAttachment(spSkeleton_findSlot(skel, colourName.c_str()), (spAttachment*)attachment);
		_spineTest->setAttachment(colourName, colourName);
	}
	
	//spRegionAttachment* attachment = (spRegionAttachment*)spSkeleton_getAttachmentForSlotName(_spineTest->getSkeleton(), "accessories/Toy1.5", "accessories/Toy1.5");
	//attachment->a = 0;
	//spine::AttachmentVertices* attachmentVertices = (spine::AttachmentVertices*)attachment->rendererObject;
	//attachmentVertices->_texture->retain();
	
	//const Size& baseSpriteSize = _baseSprite->getContentSize();
	//Vec2 anchorPoint = _oomeeData->getAnchorPoints().at("armLeft");
	//_spineTest->setPosition(Vec2(baseSpriteSize.width * anchorPoint.x - 25, baseSpriteSize.height * anchorPoint.y));
	//_baseSprite->addChild(_spineTest, _baseSprite->transformZOrder(0));
	_spineTest->scheduleUpdate();
	this->addChild(_spineTest);
	
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
        ids.push_back(accessory.second->getId());
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
           OomeeItemRef accessory = _accessories.at(oomeeItem->getTargetAnchor());
            if(accessory->getId() == oomeeItem->getId()) // accessory is already there
            {
                return;
            }
        }
        
        removeAccessory(oomeeItem->getTargetAnchor());
		
		/*if(oomeeItem->getTargetAnchor() == "toy")
		{
			const Size& baseSpriteSize = _baseSprite->getContentSize();
			Vec2 offsetNor = _oomeeData->getAnchorPoints().at("armLeft") - _oomeeData->getAnchorPoints().at("toy");
			Vec2 offset = Vec2(baseSpriteSize.width * offsetNor.x, baseSpriteSize.height * offsetNor.y);
			
			Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(OomeeMakerDataHandler::getInstance()->getAssetDir() + oomeeItem->getAssetSet().at(0).getLocation());
			cocos2d::Size sz = texture->getContentSizeInPixels();
			texture->retain();
			
			_spineTest->setAttachment("accessories/Toy1.5", "accessories/Toy1.5");
			
			spRegionAttachment* attachment = (spRegionAttachment*)spSkeleton_getAttachmentForSlotName(_spineTest->getSkeleton(), "accessories/Toy1.5", "accessories/Toy1.5");
			attachment->regionWidth = sz.width;
			attachment->regionHeight = sz.height;
			//attachment->x = offset.x + oomeeItem->getOffset().x;
			//attachment->y = offset.y + oomeeItem->getOffset().y;
			attachment->a = 1;
			spRegionAttachment_setUVs(attachment, 0, 0, 1, 1, 0);
			spRegionAttachment_updateOffset(attachment);
			spine::AttachmentVertices* attachmentVertices = (spine::AttachmentVertices*)attachment->rendererObject;
			V3F_C4B_T2F* vertices = attachmentVertices->_triangles->verts;
			for (int i = 0, ii = 0; i < 4; ++i, ii += 2) {
				vertices[i].texCoords.u = attachment->uvs[ii];
				vertices[i].texCoords.v = attachment->uvs[ii + 1];
			}
			attachmentVertices->_texture->release();
			attachmentVertices->_texture = texture;
			
			_spineTest->setAnimation(0, "flip", false);
			return;
		}*/
		
		//for(auto layer : _oomeeData->getColour()->getColours())
		for(int i = 0; i < _oomeeData->getColour()->getColours().size(); i++)
		{
			auto slotName = oomeeItem->getTargetAnchor() + "_" + StringUtils::format("%d",i);//layer.first;
			//spSkeleton_setAttachment(_spineTest->getSkeleton(), slotName.c_str(), "");
			spSlot* slot = spSkeleton_findSlot(_spineTest->getSkeleton(), slotName.c_str());
			if(slot)
			{
				spSlot_setAttachment(slot, nullptr);
			}
		}
		
		for(auto asset : oomeeItem->getAssetSet())
		{
			auto colourName = asset.getTag();
			auto colour = _oomeeData->getColour()->getColours().at(colourName);
			auto attachName = oomeeItem->getTargetAnchor() + "_" + StringUtils::format("%d",asset.getZOrder());//colourName;
			spSkeleton* skel = _spineTest->getSkeleton();
			int targetSlot = spSkeleton_findSlotIndex(skel, attachName.c_str());
			
			spRegionAttachment* attachment = (spRegionAttachment*)spSkeleton_getAttachmentForSlotName(skel, attachName.c_str(), attachName.c_str());//(spRegionAttachment*)spSkin_getAttachment(skel->data->defaultSkin, targetSlot, attachName.c_str());
			bool newAtt = false;
			if(!attachment)
			{
				attachment = spRegionAttachment_create(attachName.c_str());
				spSkin_addAttachment(skel->data->defaultSkin, targetSlot, attachName.c_str(), (spAttachment*)attachment);
				newAtt = true;
			}
			Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(OomeeMakerDataHandler::getInstance()->getAssetDir() + asset.getLocation());
			cocos2d::Size sz = texture->getContentSizeInPixels();
			texture->retain();
			
			attachment->regionOffsetX = 0;
			attachment->regionOffsetY = 0;
			attachment->regionWidth = sz.width;
			attachment->regionHeight = sz.height;
			attachment->regionOriginalWidth = attachment->regionWidth; // same if not doing whitespace stripping
			attachment->regionOriginalHeight = attachment->regionHeight;
			
			
			attachment->x = oomeeItem->getOffset().x;
			attachment->y = oomeeItem->getOffset().y;
			attachment->scaleX = 1.0f;
			attachment->scaleY = 1.0f;
			attachment->rotation = 0;
			attachment->width = sz.width;
			attachment->height = sz.height;
			
			if(colourName != "none" && oomeeItem->isUsingColourHue())
			{
				attachment->r = colour.r / 255.0f;
				attachment->g = colour.g / 255.0f;
				attachment->b = colour.b / 255.0f;
			}
			else
			{
				attachment->r = 1;
				attachment->g = 1;
				attachment->b = 1;
			}
			attachment->a = 1;
			spRegionAttachment_setUVs(attachment, 0, 0, 1, 1, 0);
			spine::AttachmentVertices* attachmentVertices = nullptr;
			if(newAtt)
			{
				attachmentVertices = new spine::AttachmentVertices(texture, 4, quadTriangles, 6);
			}
			else
			{
				attachmentVertices = (spine::AttachmentVertices*)attachment->rendererObject;
			}
			V3F_C4B_T2F* vertices = attachmentVertices->_triangles->verts;
			for (int i = 0, ii = 0; i < 4; ++i, ii += 2) {
				vertices[i].texCoords.u = attachment->uvs[ii];
				vertices[i].texCoords.v = attachment->uvs[ii + 1];
			}
			if(!newAtt)
			{
				attachmentVertices->_texture->release();
			}
			attachmentVertices->_texture = texture;
			attachment->rendererObject = attachmentVertices;
			spRegionAttachment_updateOffset(attachment);
			//spSlot_setAttachment(spSkeleton_findSlot(skel, attachName.c_str()), (spAttachment*)attachment);
			_spineTest->setAttachment(attachName, attachName);
		}
		if(oomeeItem->getTargetAnchor() == "toy" || oomeeItem->getTargetAnchor() == "head")
		{
			_spineTest->setAnimation(0, oomeeItem->getTargetAnchor(), false);
		}
		
		
        /*OomeeAccessory* accessory = OomeeAccessory::create();
        accessory->setItemData(oomeeItem);

		accessory->setColourData(_oomeeData->getColour());
		
        const Size& baseSpriteSize = _baseSprite->getContentSize();
        Vec2 anchorPoint = _oomeeData->getAnchorPoints().at(oomeeItem->getTargetAnchor()); // dont const& - unstable on android, caused many tears
        accessory->setPosition(Vec2(baseSpriteSize.width * anchorPoint.x, baseSpriteSize.height * anchorPoint.y) + oomeeItem->getOffset());
        accessory->setScale(oomeeItem->getTargetScale());
        _baseSprite->addChild(accessory, _baseSprite->transformZOrder(oomeeItem->getZOrder()));*/
        _accessories[oomeeItem->getTargetAnchor()] = oomeeItem;
        
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
        //_accessories.at(anchorPoint)->removeFromParent();
		for(int i = 0; i < _oomeeData->getColour()->getColours().size(); i++)
		{
			auto slotName = anchorPoint + "_" + StringUtils::format("%d",i);//layer.first;
			//spSkeleton_setAttachment(_spineTest->getSkeleton(), slotName.c_str(), "");
			spSlot* slot = spSkeleton_findSlot(_spineTest->getSkeleton(), slotName.c_str());
			if(slot)
			{
				spSlot_setAttachment(slot, nullptr);
			}
		}
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
        const OomeeItemRef& itemData = acc.second;
        for(const auto& dependancyId : itemData->getDependancies())
        {
            const OomeeItemRef& dependancyData = OomeeMakerDataStorage::getInstance()->getOomeeItemForKey(dependancyId);
            if(dependancyData && _accessories.find(dependancyData->getTargetAnchor()) != _accessories.end())
            {
                if(_accessories.at(dependancyData->getTargetAnchor())->getId() != dependancyId)
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
        snapshot._accessoryData.push_back(acc.second);
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
