//
//  ShopDataDownloadHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 07/03/2019.
//

#ifndef ShopDataDownloadHandler_h
#define ShopDataDownloadHandler_h

#include "../../Tinizine.h"
#include "../../API/API.h"
#include <memory>
#include "../Json.h"
#include "Shop.h"
#include "../../Utils/FileDownloader.h"
#include "../DataDownloadHandler.h"

NS_TZ_BEGIN

class ShopDataDownloadHandler : public DataDownloadHandler
{
private:
	static const std::string kCachePath;
	
	ShopRef _shop = nullptr;
	
	void loadLocalData();
	
	std::string getCachePath() const override;
	
	ShopDataDownloadHandler();
	
public:
	static ShopDataDownloadHandler* getInstance();
	~ShopDataDownloadHandler();
	
	ShopRef getShop();
	
	void getLatestData(const OnCompleteCallback& callback = nullptr) override;
	
	//delegate functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
	void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode) override;
};

NS_TZ_END

#endif /* ShopDataDownloadHandler_h */
