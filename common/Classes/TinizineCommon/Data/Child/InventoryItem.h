//
//  InventoryItem.h
//  TinizineCommon
//
//  Created by Macauley on 18/02/2019.
//

#ifndef InventoryItem_h
#define InventoryItem_h

#include "../../Tinizine.h"
#include <string>
#include <memory>
#include "../Json.h"

NS_TZ_BEGIN

class InventoryItem;
typedef std::shared_ptr<InventoryItem> InventoryItemRef;

class InventoryItem
{
private:
	int _itemId;
	std::string _type;
	std::string _name;
	std::string _uri;
	std::string _meta;
	
	InventoryItem();
public:
	
	static InventoryItemRef createWithJson(const rapidjson::Value& InventoryItem);
    static InventoryItemRef createCoin();
	static InventoryItemRef create();
	
	void parseInventoryItemData(const rapidjson::Value& InventoryItem);
	
	std::string getType() const;
	int getItemId() const;
	std::string getUri() const;
	std::string getName() const;
	std::string getMeta() const;
	
	static const std::string kAccessoryType;
};

NS_TZ_END

#endif /* InventoryItem_h */
