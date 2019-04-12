//
//  ShopDisplayItem.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 07/03/2019.
//

#include "ShopDisplayItem.h"

NS_AZOOMEE_BEGIN

ShopDisplayItemRef ShopDisplayItem::createWithJson(const rapidjson::Value& shopDisplayItemData)
{
	ShopDisplayItemRef item = ShopDisplayItem::create();
	item->parseShopDisplayItemData(shopDisplayItemData);
	return item;
}
ShopDisplayItemRef ShopDisplayItem::create()
{
	return ShopDisplayItemRef(new ShopDisplayItem());
}

void ShopDisplayItem::parseShopDisplayItemData(const rapidjson::Value& shopDisplayItemData)
{
	_shape = getStringFromJson("shape", shopDisplayItemData);
	_startDate = getIntFromJson("startDate", shopDisplayItemData);
	_entitlement = getStringFromJson("entitlement", shopDisplayItemData);
	_onPeriodDays = getIntFromJson("onPeriodDays", shopDisplayItemData);
	_offPeriodDays = getIntFromJson("offPeriodDays", shopDisplayItemData);
	_repeatable = getBoolFromJson("repeatable", shopDisplayItemData);
	_price = getIntFromJson("price", shopDisplayItemData);
	_purchaseUrl = getStringFromJson("purchaseUrl", shopDisplayItemData);
	if(shopDisplayItemData.HasMember("item"))
	{
		_inventoryItem = InventoryItem::createWithJson(shopDisplayItemData["item"]);
	}
	if(shopDisplayItemData.HasMember("tags"))
	{
		_tags = getStringArrayFromJson(shopDisplayItemData["tags"]);
	}
}

ShopDisplayItem::ShopDisplayItem()
{
	
}

InventoryItemRef ShopDisplayItem::getInventoryItem() const
{
	return _inventoryItem;
}
std::string ShopDisplayItem::getShape() const
{
	return _shape;
}
std::vector<std::string> ShopDisplayItem::getTags() const
{
	return _tags;
}
long ShopDisplayItem::getStartDate() const
{
	return _startDate;
}
int ShopDisplayItem::getOnPeriodDays() const
{
	return _onPeriodDays;
}
int ShopDisplayItem::getOffperiodDays() const
{
	return _offPeriodDays;
}
bool ShopDisplayItem::isRepeatable() const
{
	return _repeatable;
}
std::string ShopDisplayItem::getEntitlement() const
{
	return _entitlement;
}
int ShopDisplayItem::getPrice() const
{
	return _price;
}
std::string ShopDisplayItem::getPurchaseUrl() const
{
	return _purchaseUrl;
}

NS_AZOOMEE_END
