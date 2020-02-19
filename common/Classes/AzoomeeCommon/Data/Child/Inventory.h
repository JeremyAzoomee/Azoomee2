//
//  Inventory.h
//  AzoomeeCommon
//
//  Created by Macauley on 18/02/2019.
//

#ifndef Inventory_h
#define Inventory_h

#include "../../Tinizine.h"
#include <string>
#include <memory>
#include "../Json.h"
#include "InventoryItem.h"

NS_TZ_BEGIN

class Inventory;
typedef std::shared_ptr<Inventory> InventoryRef;

class Inventory
{
private:
	std::vector<InventoryItemRef> _items;
	int _coins = 0;
	
	Inventory();
public:
	
	static InventoryRef createWithJson(const rapidjson::Value& inventoryData);
	static InventoryRef create();
	
	void parseInventoryData(const rapidjson::Value& inventoryData);
	
	std::vector<InventoryItemRef> getItems() const;
	int getCoins() const;
	
	bool hasOomeeAccessory(const std::string& accessoryId) const;
	
};

NS_TZ_END

#endif /* Inventory_h */
