//
//  DragAndDropController.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 20/02/2018.
//

#include "DragAndDropController.h"

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
    //_listenerTargetNode->retain();
    _touchListener = EventListenerTouchOneByOne::create();
    
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        if(_itemData && _itemSprite)
        {
            return false;
        }
        return true;
    };
    
    _touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        if(_itemSprite)
        {
            _itemSprite->setPosition(touch->getLocation());
        }
    };
    
    _touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        if(_itemSprite && _itemData && _oomeeFigure)
        {
            const Vec2& oomeePos = _oomeeFigure->getWorldPositionForAnchorPoint(_itemData->getTargetAnchor());
            const Vec2& targetDist = touch->getLocation() - oomeePos;
            Vec2 absDist = Vec2(targetDist.x * targetDist.x, targetDist.y * targetDist.y);
            const Vec2& range = _itemData->getSnapRange();
            Vec2 absRange = Vec2(range.x * range.x, range.y * range.y);
            if(absRange.x > absDist.x && absRange.y > absDist.y)
            {
                _oomeeFigure->addAccessory(_itemData);
            }
            _itemData = nullptr;
            _itemSprite->removeFromParent();
            _itemSprite = nullptr;
        }
    };
    
    _touchListener->onTouchCancelled = _touchListener->onTouchEnded;
    
    //_touchListener->retain();
    
}

void DragAndDropController::setItemData(const OomeeItemRef& data)
{
    _itemData = data;
    if(_itemSprite)
    {
        _itemSprite->removeFromParent();
    }
    _itemSprite = Sprite::create(_itemData->getAssetName());
    _itemSprite->setScale(_itemData->getDragScale());
    _itemSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _listenerTargetNode->addChild(_itemSprite);
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

NS_AZOOMEE_OM_END
