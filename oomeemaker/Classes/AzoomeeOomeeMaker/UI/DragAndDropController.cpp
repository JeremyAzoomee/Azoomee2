//
//  DragAndDropController.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 20/02/2018.
//

#include "DragAndDropController.h"
#include "../DataObjects/OomeeMakerDataHandler.h"

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

static std::auto_ptr<DragAndDropController> sDragAndDropControllerSharedInstance;

DragAndDropController* DragAndDropController::getInstance()
{
    if(!sDragAndDropControllerSharedInstance.get())
    {
        sDragAndDropControllerSharedInstance.reset(new DragAndDropController());
        sDragAndDropControllerSharedInstance->init();
    }
    return sDragAndDropControllerSharedInstance.get();
}

DragAndDropController::~DragAndDropController(void)
{
    
}

void DragAndDropController::init()
{
    _listenerTargetNode = Node::create();
    
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        log("touch began - DNDC");
        if(_itemData && _itemSprite)
        {
            return false;
        }
        return true;
    };
    
    _touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        if(this->_removeItemFromOomee)
        {
            if(_itemData)
            {
                _oomeeFigure->removeAccessory(_itemData->getTargetAnchor());
            }
            this->_removeItemFromOomee = false;
            
        }
        
        if(_itemSprite)
        {
            _itemSprite->setPosition(touch->getLocation());
            
            if(_debugMode)
            {
                const Vec2& dist = (_itemSprite->getPosition() - _anchorPos) / _oomeeFigure->getOomeeData()->getScale();
                _positioningLabel->setString(StringUtils::format("x: %d, y:%d\n%s",(int)dist.x, (int)dist.y, _itemData->getTargetAnchor().c_str()));
                _anchorToSprite->clear();
                _anchorToSprite->drawLine(_anchorPos, _itemSprite->getPosition(), Color4F::WHITE);
            }
        }
    };
    
    _touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        if(_itemSprite && _itemData && _oomeeFigure)
        {
            const Vec2& targetDist = touch->getLocation() - _anchorPos;
            Vec2 absDist = Vec2(targetDist.x * targetDist.x, targetDist.y * targetDist.y);
            const Vec2& range = _itemData->getSnapRange() * _oomeeFigure->getOomeeData()->getScale();
            Vec2 absRange = Vec2(range.x * range.x, range.y * range.y);
            if(absRange.x > absDist.x && absRange.y > absDist.y)
            {
                _oomeeFigure->addAccessory(_itemData);
            }
            _itemData = nullptr;
            _itemSprite->removeFromParent();
            _itemSprite = nullptr;
            
            if(_debugMode)
            {
                _positioningLabel = nullptr;
                _anchorToSprite->removeFromParent();
                _anchorToSprite = nullptr;
            }
        }
        _touchListener->setSwallowTouches(false);
    };
    
    _touchListener->onTouchCancelled = _touchListener->onTouchEnded;
    
}

void DragAndDropController::setItemData(const OomeeItemRef& data)
{
    _itemData = data;
    if(_itemSprite)
    {
        _itemSprite->removeFromParent();
    }
    _itemSprite = SpriteWithHue::create(OomeeMakerDataHandler::getInstance()->getAssetDir() + _itemData->getAssetName());
    _itemSprite->setScale(_itemData->getDragScale());
    _itemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    if(data->isUsingColourHue())
    {
        _itemSprite->setHue(_oomeeFigure->getHue());
    }
    _listenerTargetNode->addChild(_itemSprite);
    _anchorPos = _oomeeFigure->getWorldPositionForAnchorPoint(_itemData->getTargetAnchor());
    
    if(_debugMode)
    {
        const Vec2& dist = (_anchorPos - _itemSprite->getPosition());
        if(_positioningLabel)
        {
            _positioningLabel->removeFromParent();
        }
        _positioningLabel = Label::createWithTTF(StringUtils::format("x: %d, y:%d",(int)dist.x, (int)dist.y), "fonts/arial.ttf", 40);
        _positioningLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _positioningLabel->setPosition(Vec2(_itemSprite->getContentSize().width * 0.5f, 0));
        _itemSprite->addChild(_positioningLabel);
        
        if(_anchorToSprite)
        {
            _anchorToSprite->removeFromParent();
        }
        _anchorToSprite = DrawNode::create();
        _anchorToSprite->drawLine(_anchorPos, _itemSprite->getPosition(), Color4F::WHITE);
        _listenerTargetNode->addChild(_anchorToSprite);
    }
    
    _touchListener->setSwallowTouches(true);
}

void DragAndDropController::setTargetOomee(OomeeFigure* oomeeFigure)
{
    _oomeeFigure = oomeeFigure;
}

void DragAndDropController::attachToScene(cocos2d::Scene *currentScene)
{
    init();
    currentScene->addChild(_listenerTargetNode);
    _listenerTargetNode->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, _listenerTargetNode);
}

void DragAndDropController::setDebugModeEnabled(bool isEnabled)
{
    _debugMode = isEnabled;
}

void DragAndDropController::setRemoveItemFromOomee()
{
    _removeItemFromOomee = true;
}

NS_AZOOMEE_OM_END
