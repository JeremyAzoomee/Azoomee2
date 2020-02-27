//
//  Shop.cpp
//  TinizineCommon
//
//  Created by Macauley on 07/03/2019.
//

#include "Shop.h"

NS_TZ_BEGIN

ShopRef Shop::createWithJson(const rapidjson::Value& shopData)
{
	ShopRef shop = Shop::create();
	shop->parseShopData(shopData);
	return shop;
}
ShopRef Shop::create()
{
	return ShopRef(new Shop());
}

Shop::Shop()
{
	
}

void Shop::parseShopData(const rapidjson::Value& shopData)
{
	_id = getStringFromJson("id", shopData);
	_name = getStringFromJson("name", shopData);
	if(shopData.HasMember("displays") && shopData["displays"].IsArray())
	{
		const rapidjson::Value& displayData = shopData["displays"];
		for(int i = 0; i < displayData.Size(); i++)
		{
			ShopDisplayRef display = ShopDisplay::createWithJson(displayData[i]);
			_shopDisplays.push_back(display);
		}
	}
}

std::string Shop::getId() const
{
	return _id;
}
std::string Shop::getName() const
{
	return _name;
}
std::vector<ShopDisplayRef> Shop::getDisplays() const
{
	return _shopDisplays;
}

NS_TZ_END

