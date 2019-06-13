//
//  ShopDataHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 12/03/2019.
//

#include "ShopDataHandler.h"
#include "../../Utils/DirUtil.h"

NS_AZOOMEE_BEGIN



const std::string ShopDataHandler::kCachePath = "shopCache/";

static std::auto_ptr<ShopDataHandler> sShopDataHandlerSharedInstance;

ShopDataHandler* ShopDataHandler::getInstance()
{
	if(!sShopDataHandlerSharedInstance.get())
	{
		sShopDataHandlerSharedInstance.reset(new ShopDataHandler());
	}
	const std::string& cachePath = DirUtil::getCachesPath() + kCachePath;
	if(!cocos2d::FileUtils::getInstance()->isDirectoryExist(cachePath))
	{
		cocos2d::FileUtils::getInstance()->createDirectory(cachePath);
	}
	
	return sShopDataHandlerSharedInstance.get();
}

ShopDataHandler::~ShopDataHandler()
{
	
}
ShopDataHandler::ShopDataHandler()
{
	
}

ShopRef ShopDataHandler::getShop()
{
	return _shop;
}

void ShopDataHandler::getLatestData(const OnCompleteCallback& callback)
{
	if(callback)
	{
		_callback = callback;
	}
	HttpRequestCreator* request = API::GetShopFeed(this);
	request->execute();
}

std::string ShopDataHandler::getCachePath() const
{
	return kCachePath;
}

void ShopDataHandler::loadLocalData()
{
	/*
	const std::string& shopString = cocos2d::FileUtils::getInstance()->getStringFromFile("res/shop/testShop.json");
	rapidjson::Document shopJson;
	shopJson.Parse(shopString.c_str());
	ShopRef shop = nullptr;
	if(!shopJson.HasParseError())
	{
		_shop = Shop::createWithJson(shopJson);
	}
	*/
	const std::string& filePath = DirUtil::getCachesPath() + kCachePath + "shop.json";
	const std::string shopDataStr = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);
	if(shopDataStr == "")
	{
		sendCallback(false);
		return;
	}
	rapidjson::Document shopJson;
	shopJson.Parse(shopDataStr.c_str());
	if(shopJson.HasParseError())
	{
		sendCallback(false);
		return;
	}
	_shop = Shop::createWithJson(shopJson);
	sendCallback(true);
}

// delegate functions
void ShopDataHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	rapidjson::Document result;
	result.Parse(body.c_str());
	if(result.HasParseError())
	{
		return;
	}
	
	const std::string& url = getStringFromJson("uri", result);
	
	downloadFile(url);
}
void ShopDataHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	loadLocalData();
}
void ShopDataHandler::onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode)
{
	if(responseCode == 200)
	{
		setLocalEtag(_fileDownloader->getEtag());
		_fileDownloader = nullptr;
		const std::string& filePath = DirUtil::getCachesPath() + kCachePath + "shop.json";
		cocos2d::FileUtils::getInstance()->writeStringToFile(fileString, filePath);
	}
	
	loadLocalData();
	
}


NS_AZOOMEE_END
