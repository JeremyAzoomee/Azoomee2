//
//  ShopDisplayItem.h
//  AzoomeeCommon
//
//  Created by Macauley on 07/03/2019.
//

#ifndef ShopDisplayItem_h
#define ShopDisplayItem_h

#include "../../Azoomee.h"
#include <string>
#include <memory>
#include "../Json.h"
#include "../Rewards/InventoryItem.h"

NS_AZOOMEE_BEGIN

class ShopDisplayItem;
typedef std::shared_ptr<ShopDisplayItem> ShopDisplayItemRef;

class ShopDisplayItem
{
protected:
	
	InventoryItemRef _inventoryItem = nullptr;
	std::string _shape;
	std::vector<std::string> _tags;
	long _startDate;
	int _onPeriodDays;
	int _offPeriodDays;
	std::string _entitlement;
	bool _repeatable;
	int _price;
	
	ShopDisplayItem();
public:
	
	static ShopDisplayItemRef createWithJson(const rapidjson::Value& shopDisplayItemData);
	static ShopDisplayItemRef create();
	
	void parseShopDisplayItemData(const rapidjson::Value& shopDisplayItemData);
	
	InventoryItemRef getInventoryItem() const;
	std::string getShape() const;
	std::vector<std::string> getTags() const;
	long getStartDate() const;
	int getOnPeriodDays() const;
	int getOffperiodDays() const;
	bool isRepeatable() const;
	std::string getEntitlement() const;
	int getPrice() const;
	
};

NS_AZOOMEE_END

#endif /* ShopDisplayItem_h */
