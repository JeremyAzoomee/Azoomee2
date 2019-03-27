//
//  ShopDataHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 07/03/2019.
//

#ifndef ShopDataHandler_h
#define ShopDataHandler_h

#include "../../Azoomee.h"
#include "../../API/API.h"
#include <memory>
#include "../Json.h"
#include "Shop.h"
#include "../../Utils/FileDownloader.h"
#include "../DataDownloadHandler.h"

NS_AZOOMEE_BEGIN

class ShopDataHandler : public DataDownloadHandler
{
private:
	static const std::string kCachePath;
	
	ShopRef _shop = nullptr;
	
	void loadLocalData();
	
	std::string getCachePath() const override;
	
	ShopDataHandler();
	
public:
	static ShopDataHandler* getInstance();
	~ShopDataHandler();
	
	ShopRef getShop();
	
	void getLatestData(const OnCompleteCallback& callback = nullptr) override;
	
	//delegate functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
	void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode) override;
};

NS_AZOOMEE_END

#endif /* ShopDataHandler_h */
