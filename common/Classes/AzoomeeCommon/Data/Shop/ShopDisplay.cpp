//
//  ShopDisplay.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 07/03/2019.
//

#include "ShopDisplay.h"

NS_AZOOMEE_BEGIN

ShopDisplayRef ShopDisplay::createWithJson(const rapidjson::Value& shopDisplayData)
{
	ShopDisplayRef shopDisplay = ShopDisplay::create();
	shopDisplay->parseShopDisplayData(shopDisplayData);
	return shopDisplay;
}
ShopDisplayRef ShopDisplay::create()
{
	return ShopDisplayRef(new ShopDisplay());
}

ShopDisplay::ShopDisplay()
{
	
}

void ShopDisplay::parseShopDisplayData(const rapidjson::Value& shopDisplayData)
{
	_id = getStringFromJson("id", shopDisplayData);
	_name = getStringFromJson("name", shopDisplayData);
	if(shopDisplayData.HasMember("items") && shopDisplayData["items"].IsArray())
	{
		const rapidjson::Value& itemData = shopDisplayData["items"];
		for(int i = 0; i < itemData.Size(); i++)
		{
			ShopDisplayItemRef item = ShopDisplayItem::createWithJson(itemData[i]);
			_shopDisplayItems.push_back(item);
		}
	}
}

std::string ShopDisplay::getId() const
{
	return _id;
}
std::string ShopDisplay::getName() const
{
	return _name;
}
std::vector<ShopDisplayItemRef> ShopDisplay::getDisplayItems() const
{
	return _shopDisplayItems;
}

NS_AZOOMEE_END
