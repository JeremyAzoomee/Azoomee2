//
//  InventoryItem.cpp
//  TinizineCommon
//
//  Created by Macauley on 18/02/2019.
//

#include "InventoryItem.h"

NS_TZ_BEGIN

const std::string InventoryItem::kAccessoryType = "ACCESSORIES";

InventoryItemRef InventoryItem::createWithJson(const rapidjson::Value& inventoryItem)
{
	InventoryItemRef item = create();
	item->parseInventoryItemData(inventoryItem);
	return item;
}

InventoryItemRef InventoryItem::createCoin()
{
    InventoryItemRef item = create();
    item->_type = "COIN";
    return item;
}

InventoryItemRef InventoryItem::create()
{
	return InventoryItemRef(new InventoryItem);
}

InventoryItem::InventoryItem()
{
	
}

void InventoryItem::parseInventoryItemData(const rapidjson::Value& inventoryItem)
{
	_itemId = getIntFromJson("id", inventoryItem);
	_type = getStringFromJson("type", inventoryItem);
	_uri = getStringFromJson("uri", inventoryItem);
	_name = getStringFromJson("name", inventoryItem);
	_meta = getStringFromJson("oomeeMakerItemMetaId", inventoryItem);
}

std::string InventoryItem::getType() const
{
	return _type;
}
int InventoryItem::getItemId() const
{
	return _itemId;
}
std::string InventoryItem::getUri() const
{
	return _uri;
}
std::string InventoryItem::getName() const
{
	return _name;
}
std::string InventoryItem::getMeta() const
{
	return _meta;
}

NS_TZ_END

