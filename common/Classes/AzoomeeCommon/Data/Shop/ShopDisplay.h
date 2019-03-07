//
//  ShopDisplay.h
//  AzoomeeCommon
//
//  Created by Macauley on 07/03/2019.
//

#ifndef ShopDisplay_h
#define ShopDisplay_h

#include "../../Azoomee.h"
#include <string>
#include <memory>
#include "../Json.h"
#include "ShopDisplayItem.h"

NS_AZOOMEE_BEGIN

class ShopDisplay;
typedef std::shared_ptr<ShopDisplay> ShopDisplayRef;

class ShopDisplay
{
protected:
	
	std::string _id;
	std::string _name;
	std::vector<ShopDisplayItemRef> _shopDisplayItems;
	
	ShopDisplay();
public:
	
	static ShopDisplayRef createWithJson(const rapidjson::Value& shopDisplayData);
	static ShopDisplayRef create();
	
	void parseShopDisplayData(const rapidjson::Value& shopDisplayData);
	
	std::string getId() const;
	std::string getName() const;
	std::vector<ShopDisplayItemRef> getDisplayItems() const;
	
};

NS_AZOOMEE_END


#endif /* ShopDisplay_h */
