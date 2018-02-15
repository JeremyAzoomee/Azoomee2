//
//  OomeeItem.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 15/02/2018.
//

#include "OomeeItem.h"

NS_AZOOMEE_OM_BEGIN

OomeeItemRef OomeeItem::create()
{
    return std::make_shared<OomeeItem>();
}

OomeeItemRef OomeeItem::createWithData(const rapidjson::Value& itemConfig)
{
    OomeeItemRef item = std::make_shared<OomeeItem>();
    item->initWithData(itemConfig);
    return item;
}

OomeeItem::OomeeItem()
{
    
}

void OomeeItem::initWithData(const rapidjson::Value& itemConfig)
{
    setId(getStringFromJson("id", itemConfig));
    setTargetAnchor(getStringFromJson("targetAnchor", itemConfig));
    setOffset(getVec2FromJson("offset", itemConfig, cocos2d::Vec2(0,0)));
    setSnapRange(getVec2FromJson("snapRange", itemConfig, cocos2d::Vec2(500,500)));
    setMenuScale(getFloatFromJson("menuScale", itemConfig, 1.0f));
    setDragScale(getFloatFromJson("dragScale", itemConfig, 1.0f));
    setTargetScale(getFloatFromJson("targetScale", itemConfig, 1.0f));
    setZOrder(getIntFromJson("zOrder", itemConfig, 0));
    setCategoryId(getStringFromJson("categoryId", itemConfig));
    setAssetFilename(getStringFromJson("assetFilename", itemConfig));
}

// GETTERS AND SETTERS

void OomeeItem::setId(const std::string& id)
{
    _id = id;
}
std::string OomeeItem::getId() const
{
    return _id;
}

void OomeeItem::setTargetAnchor(const std::string& targetAnchor)
{
    _targetAnchor = targetAnchor;
}
std::string OomeeItem::getTargetAnchor() const
{
    return _targetAnchor;
}

void OomeeItem::setOffset(const cocos2d::Vec2& offset)
{
    _offset = offset;
}
cocos2d::Vec2 OomeeItem::getOffset() const
{
    return _offset;
}

void OomeeItem::setSnapRange(const cocos2d::Vec2& snapRange)
{
    _snapRange = snapRange;
}
cocos2d::Vec2 OomeeItem::getSnapRange() const
{
    return _snapRange;
}

void OomeeItem::setMenuScale(float menuScale)
{
    _menuScale = menuScale;
}
float OomeeItem::getMenuScale() const
{
    return _menuScale;
}

void OomeeItem::setDragScale(float dragScale)
{
    _dragScale = dragScale;
}
float OomeeItem::getDragScale() const
{
    return _dragScale;
}

void OomeeItem::setTargetScale(float targetScale)
{
    _targetScale = targetScale;
}
float OomeeItem::getTargetScale() const
{
    return _targetScale;
}

void OomeeItem::setZOrder(int zOrder)
{
    _zOrder = zOrder;
}
int OomeeItem::getZOrder() const
{
    return _zOrder;
}

void OomeeItem::setCategoryId(const std::string& categoryId)
{
    _categoryId = categoryId;
}
std::string OomeeItem::getCategoryId() const
{
    return _categoryId;
}

void OomeeItem::setAssetFilename(const std::string& assetName)
{
    _assetFilename = assetName;
}
std::string OomeeItem::getAssetName() const
{
    return _assetFilename;
}

NS_AZOOMEE_OM_END
