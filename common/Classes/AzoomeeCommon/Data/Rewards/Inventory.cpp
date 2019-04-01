//
//  Inventory.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 18/02/2019.
//

#include "Inventory.h"

NS_AZOOMEE_BEGIN

InventoryRef Inventory::createWithJson(const rapidjson::Value& inventoryData)
{
	InventoryRef inventory = Inventory::create();
	inventory->parseInventoryData(inventoryData);
	return inventory;
}
InventoryRef Inventory::create()
{
	return InventoryRef(new Inventory);
}

Inventory::Inventory()
{
	
}

void Inventory::parseInventoryData(const rapidjson::Value& inventoryData)
{
	_items.clear();
	_coins = getIntFromJson("coins", inventoryData);
	if(inventoryData.HasMember("items") && inventoryData["items"].IsArray())
	{
		const rapidjson::Value& itemsData = inventoryData["items"];
		for(int i = 0; i < itemsData.Size(); i++)
		{
			InventoryItemRef item = InventoryItem::createWithJson(itemsData[i]);
			_items.push_back(item);
		}
	}
}

std::vector<InventoryItemRef> Inventory::getItems() const
{
	return _items;
}
int Inventory::getCoins() const
{
	return _coins;
}

bool Inventory::hasOomeeAccessory(const std::string& accessoryId) const
{
	for(const auto& item : _items)
	{
		if(item->getType() == InventoryItem::kAccessoryType && item->getMeta() == accessoryId)
		{
			return true;
		}
	}
	return false;
}

NS_AZOOMEE_END
