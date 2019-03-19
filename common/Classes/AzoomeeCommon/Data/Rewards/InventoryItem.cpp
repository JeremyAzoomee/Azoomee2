//
//  InventoryItem.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 18/02/2019.
//

#include "InventoryItem.h"

NS_AZOOMEE_BEGIN

InventoryItemRef InventoryItem::createWithJson(const rapidjson::Value& inventoryItem)
{
	InventoryItemRef item = create();
	item->parseInventoryItemData(inventoryItem);
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
	_meta = getStringFromJson("meta", inventoryItem);
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

NS_AZOOMEE_END

