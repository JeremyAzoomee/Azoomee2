//
//  MarketingAssetManager.h
//  Azoomee
//
//  Created by Macauley on 24/07/2019.
//

#ifndef MarketingAssetManager_h
#define MarketingAssetManager_h

#include <AzoomeeCommon/Tinizine.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <AzoomeeCommon/Data/Json.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class MarketingAsset;
typedef std::shared_ptr<MarketingAsset> MarketingAssetRef;

class MarketingAsset
{
private:
	std::string _id;
	std::map<std::string,std::string> _title;
	std::map<std::string,std::string> _description;
	std::string _location;
	std::string _localLocation;
	
public:
	static MarketingAssetRef create();
	static MarketingAssetRef createWithJson(const rapidjson::Value& json);
	
	MarketingAsset();
	
	void initWithJson(const rapidjson::Value& json);
	void setLocalLocation(const std::string& localLocation);
	
	bool isDownloaded() const;
	std::string getTitle(const std::string& language) const;
	std::string getDescription(const std::string& language) const;
	std::string getLocation() const;
	std::string getLocalLocation() const;
	
};

class MarketingAssetManager : public HttpRequestCreatorResponseDelegate, ImageDownloaderDelegate
{
private:
	
	std::vector<MarketingAssetRef> _marketingAssets;
	
	void downloadImage(int index);
    void loadLocalAssets();
    
	MarketingAssetManager();
public:
	
	static MarketingAssetManager* getInstance();
	
	void downloadMarketingAssets();
	
	std::vector<MarketingAssetRef> getMarketingAssets() const;
	
	//Delegate functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
	void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
	void onImageDownloadFailed() override;
	
};

NS_AZ_END

#endif /* MarketingAssetManager_h */
