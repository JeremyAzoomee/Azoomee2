//
//  MarketingAssetManager.cpp
//  Azoomee
//
//  Created by Macauley on 24/07/2019.
//

#include "MarketingAssetManager.h"
#include <AzoomeeCommon/Utils/StringMgr.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>

NS_AZOOMEE_BEGIN

MarketingAssetRef MarketingAsset::create()
{
	return std::make_shared<MarketingAsset>();
}
MarketingAssetRef MarketingAsset::createWithJson(const rapidjson::Value& json)
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
    _title.clear();
    _description.clear();
	if(json.HasMember("titleSet") && json["titleSet"].IsArray())
	{
        const rapidjson::Value& jsonValue = json["titleSet"];
        for (int i = 0; i < jsonValue.Size(); i++)
        {
            const std::string& key = getStringFromJson("language", jsonValue[i]);
            const std::string& value = getStringFromJson("text", jsonValue[i]);
            if(key != "")
            {
                _title[key] = value;
            }
        }
	}
	if(json.HasMember("textSet") && json["textSet"].IsArray());
	{
        const rapidjson::Value& jsonValue = json["textSet"];
        for (int i = 0; i < jsonValue.Size(); i++)
        {
            const std::string& key = getStringFromJson("language", jsonValue[i]);
            const std::string& value = getStringFromJson("text", jsonValue[i]);
            if(key != "")
            {
                _description[key] = value;
            }
        }
	}
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
std::string MarketingAsset::getTitle(const std::string& language) const
{
	if(_title.find(language) != _title.end())
	{
		return _title.at(language);
	}
    return _title.at(StringMgr::kDefaultLanguageIdentifier);
}
std::string MarketingAsset::getDescription(const std::string& language) const
{
	if(_description.find(language) != _description.end())
	{
		return _description.at(language);
	}
    return _description.at(StringMgr::kDefaultLanguageIdentifier);
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
	_marketingAssets.clear();
    
    HttpRequestCreator* request = API::GetMarketingAssets(ParentManager::getInstance()->getLoggedInParentCountryCode() ,this);
    request->execute();
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

void MarketingAssetManager::loadLocalAssets()
{
    _marketingAssets.clear();
    
    const std::string& config = cocos2d::FileUtils::getInstance()->getStringFromFile("res/onboarding/marketingConfig.json");
    rapidjson::Document marketingData;
    marketingData.Parse(config.c_str());
    if(marketingData.HasParseError() || !marketingData.IsArray())
    {
        CCASSERT(true,"local marketing data is invalid");
        return;
    }
    for(int i = 0; i < marketingData.Size(); i++)
    {
        const rapidjson::Value& dataEntry = marketingData[i];;
        MarketingAssetRef data = MarketingAsset::createWithJson(dataEntry);
        data->setLocalLocation(data->getLocation());
        _marketingAssets.push_back(data);
    }
}

//Delegate functions
void MarketingAssetManager::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	// parse data for assets, initiate image download on each
	rapidjson::Document assetData;
	assetData.Parse(body.c_str());
	if(assetData.HasParseError() || !assetData.IsArray())
	{
        loadLocalAssets();
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
    loadLocalAssets();
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
