//
//  InventoryItem.h
//  AzoomeeCommon
//
//  Created by Macauley on 18/02/2019.
//

#ifndef InventoryItem_h
#define InventoryItem_h

#include "../../Azoomee.h"
#include <string>
#include <memory>
#include "../Json.h"

NS_AZOOMEE_BEGIN

class InventoryItem;
typedef std::shared_ptr<InventoryItem> InventoryItemRef;

class InventoryItem
{
private:
	std::string _itemId;
	std::string _type;
	std::string _name;
	std::string _uri;
	
	InventoryItem();
public:
	
	static InventoryItemRef createWithJson(const rapidjson::Value& InventoryItem);
	static InventoryItemRef create();
	
	void parseInventoryItemData(const rapidjson::Value& InventoryItem);
	
	std::string getType() const;
	std::string getItemId() const;
	std::string getUri() const;
	std::string getName() const;
	
};

NS_AZOOMEE_END

#endif /* InventoryItem_h */
