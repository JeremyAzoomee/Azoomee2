//
//  MarketingAssetManager.cpp
//  Azoomee
//
//  Created by Macauley on 24/07/2019.
//

#include "MarketingAssetManager.h"
#include <AzoomeeCommon/Utils/StringMgr.h>

NS_AZOOMEE_BEGIN

MarketingAssetRef create()
{
	return std::make_shared<MarketingAsset>();
}
MarketingAssetRef createWithJson(const rapidjson::Value& json)
{
	MarketingAssetRef asset = MarketingAsset::create();
	asset->initWithJson(json);
	return asset;
}

MarketingAsset::MarketingAsset()
{
	
}

void MarketingAsset::initWithJson(const rapidjson::Value& json)
{
	_title = getStringFromJson("title", json);
	_description = getStringFromJson("description", json);
	_location = getStringFromJson("location", json);
	_id = getStringFromJson("id", json);
}
void MarketingAsset::setLocalLocation(const std::string& localLocation)
{
	_localLocation = localLocation;
}

bool MarketingAsset::isDownloaded() const
{
	return _localLocation != "";
}
std::string MarketingAsset::getTitle() const
{
	return _title;
}
std::string MarketingAsset::getDescription() const
{
	return _description;
}
std::string MarketingAsset::getLocation() const
{
	return _location;
}
std::string MarketingAsset::getLocalLocation() const
{
	return _localLocation;
}

static std::auto_ptr<MarketingAssetManager> sMarketingAssetManagerSharedInstance;

MarketingAssetManager* MarketingAssetManager::getInstance()
{
	if(!sMarketingAssetManagerSharedInstance.get())
	{
		sMarketingAssetManagerSharedInstance.reset(new MarketingAssetManager());
	}
	return sMarketingAssetManagerSharedInstance.get();
}

MarketingAssetManager::MarketingAssetManager()
{
	
}

void MarketingAssetManager::downloadMarketingAssets()
{
	/// call get assets enpoint API
}

std::vector<MarketingAssetRef> MarketingAssetManager::getMarketingAssets() const
{
	return _marketingAssets;
}

void MarketingAssetManager::downloadImage(int index)
{
	ImageDownloaderRef downloader = ImageDownloader::create("marketingAssets", ImageDownloader::CacheMode::File);
	downloader->downloadImage(this, _marketingAssets.at(index)->getLocation());
}

//Delegate functions
void MarketingAssetManager::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	// parse data for assets, initiate image download on each
	rapidjson::Document assetData;
	assetData.Parse(body.c_str());
	if(assetData.HasParseError() || !assetData.IsArray())
	{
		return;
	}
	_marketingAssets.clear();
	for(int i = 0; i < assetData.Size(); i++)
	{
		const rapidjson::Value& data = assetData[i];
		MarketingAssetRef asset = MarketingAsset::createWithJson(data);
		_marketingAssets.push_back(asset);
	}
	
	for(int i = 0; i < _marketingAssets.size(); i++)
	{
		downloadImage(i);
	}
}
void MarketingAssetManager::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	
}
void MarketingAssetManager::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
	const std::string& url = downloader->getUrl();
	auto pos = std::find_if(_marketingAssets.begin(), _marketingAssets.end(), [url](const MarketingAssetRef& asset){return asset->getLocation() == url;});
	if(pos != _marketingAssets.end())
	{
		pos->get()->setLocalLocation(downloader->getLocalImagePath());
	}
}
void MarketingAssetManager::onImageDownloadFailed()
{
	
}

NS_AZOOMEE_END
