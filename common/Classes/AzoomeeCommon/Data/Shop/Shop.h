//
//  Shop.h
//  AzoomeeCommon
//
//  Created by Macauley on 07/03/2019.
//

#ifndef Shop_h
#define Shop_h

#include "../../Tinizine.h"
#include <string>
#include <memory>
#include "../Json.h"
#include "ShopDisplay.h"

NS_TZ_BEGIN

class Shop;
typedef std::shared_ptr<Shop> ShopRef;

class Shop
{
protected:
	
	std::string _id;
	std::string _name;
	std::vector<ShopDisplayRef> _shopDisplays;
	
	Shop();
public:
	
	static ShopRef createWithJson(const rapidjson::Value& shopData);
	static ShopRef create();
	
	void parseShopData(const rapidjson::Value& shopData);
	
	std::string getId() const;
	std::string getName() const;
	std::vector<ShopDisplayRef> getDisplays() const;
	
};


NS_TZ_END


#endif /* Shop_h */
