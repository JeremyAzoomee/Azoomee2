//
//  ShopDataHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 07/03/2019.
//

#ifndef ShopDataHandler_h
#define ShopDataHandler_h

#include "../Azoomee.h"
#include "API.h"
#include <memory>
#include "../Json.h"
#include "Shop.h"

NS_AZOOMEE_BEGIN

class ShopDataHandler : public HttpRequestCreatorResponseDelegate
{
private:
	ShopRef _shop = nullptr;
	
	ShopDataHandler();
	
public:
	static ShopDataHandler* getInstance();
	virtual ~ShopDataHandler();
	
	ShopRef getShop();
	
	//delegate functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
	void onHttpRequestFailed(const std::string& requestTag, long errorCode);
};

NS_AZOOMEE_END

#endif /* ShopDataHandler_h */
