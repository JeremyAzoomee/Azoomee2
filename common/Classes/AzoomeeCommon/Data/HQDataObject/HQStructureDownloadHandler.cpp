//
//  HQStructureDownloadHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 10/05/2018.
//

#include "HQStructureDownloadHandler.h"
#include "HQStructureDownloadHandler.h"
#include <cocos/cocos2d.h>
#include "../../API/API.h"
#include "../../Utils/FileZipUtil.h"
#include "../Json.h"
#include "../../UI/ModalMessages.h"
#include "../Child/ChildManager.h"
#include "../../Utils/DirectorySearcher.h"
#include "../ConfigStorage.h"
#include "../../Utils/StringFunctions.h"
#include "HQDataObjectManager.h"
#include "ContentItemPool.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string HQStructureDownloadHandler::kCachePath = "feedsCache/";
const std::string HQStructureDownloadHandler::kZipName = "feeds.zip";

static std::auto_ptr<HQStructureDownloadHandler> sHQStructureDownloadHandlerSharedInstance;

HQStructureDownloadHandler* HQStructureDownloadHandler::getInstance()
{
    if(!sHQStructureDownloadHandlerSharedInstance.get())
    {
        sHQStructureDownloadHandlerSharedInstance.reset(new HQStructureDownloadHandler());
    }
    const std::string& cachePath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath;
    if(!cocos2d::FileUtils::getInstance()->isDirectoryExist(cachePath))
    {
        cocos2d::FileUtils::getInstance()->createDirectory(cachePath);
    }
    
    return sHQStructureDownloadHandlerSharedInstance.get();
}

HQStructureDownloadHandler::~HQStructureDownloadHandler(void)
{
    
}

void HQStructureDownloadHandler::getLatestData(const OnCompleteCallback& callback)
{
	if(callback)
	{
		_callback = callback;
	}
    ModalMessages::getInstance()->startLoading();
	const std::string& childId = ChildManager::getInstance()->ChildManager::getInstance()->getLoggedInChild()->getId();
    HttpRequestCreator* request = API::GetHQStructureDataRequest(childId, this);
    request->execute();
}

void HQStructureDownloadHandler::loadLocalData()
{
    if(!HQDataObjectManager::getInstance()->isSameHQData(getLocalEtag()))
    {
        const std::string& localDataPath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath + _feedPath + "/";
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath + "entitlements.json");
        parseEntitlementData(data);
        
        const auto& feedsFolders = DirectorySearcher::getInstance()->getFoldersInDirectory(localDataPath);
        for(const auto& folder : feedsFolders)
        {
            if(folder == "groups")
            {
                continue;
            }
            const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath + folder + "/feed.json");
            parseHQStructureData(data, convertToHQNameString(folder));
        }
        HQDataObjectManager::getInstance()->setHQDataEtag(getLocalEtag());
    }
    ModalMessages::getInstance()->stopLoading();
	sendCallback(true);
}

void HQStructureDownloadHandler::loadGroupHQData(const std::string &groupIdPath)
{
    const std::string& dataPath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath + _feedPath + "/" + groupIdPath;
    if(FileUtils::getInstance()->isFileExist(dataPath))
    {
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(dataPath);
        parseHQStructureData(data, ConfigStorage::kGroupHQName);
    }
}

std::string HQStructureDownloadHandler::getCachePath() const
{
	return kCachePath + _feedPath;
}

void HQStructureDownloadHandler::parseNavigationData(const std::string &data)
{
    rapidjson::Document result;
    result.Parse(data.c_str());
    if(result.HasParseError() || !result.HasMember("navigation"))
    {
        return;
    }
    
    std::vector<std::string> hqNames;
    for(int i = 0; i < result["navigation"].Size(); i++)
    {
        const auto& value = result["navigation"][i];
        const std::string& hqName = getStringFromJson("name", value);
        hqNames.push_back(hqName);
        if(getBoolFromJson("default", value, false))
        {
			ConfigStorage::getInstance()->setDefaultHQ(hqNames.back());
        }
        if(value.HasMember("available"))
        {
            HQDataObjectRef dataObject = HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqName);
            dataObject->setHqEntitlement(getBoolFromJson("available", value));
        }
        
    }
    ConfigStorage::getInstance()->setNavigationHQs(hqNames);
}

void HQStructureDownloadHandler::parseHQStructureData(const std::string& hqStuctureData, const std::string& hqName)
{
	// stucture
	/*
	 {
	 "items":{
	 "1111111-11111-1111-111111":{"entitled":true},
	 "1111111-22222-1111-111111":{"entitled":true}
	 etc.
	 },
	 "rows": [
	 {
	 "contentIds": [
	 "1111111-11111-1111-111111",
	 etc.
	 ],
	 "title": "favs",
	 "images": {
	 "icon": "icon url"
	 },
	 "shapes": [
	 [1,1],
	 etc
	 ]
	 }
	 }
	 */
	
	rapidjson::Document hqData;
	hqData.Parse(hqStuctureData.c_str());
	
	if (hqData.HasParseError())
	{
		return; //JSON HAS ERRORS IN IT
	}
	
	if(hqData.HasMember("items"))
	{
		const rapidjson::Value& items = hqData["items"];
		for (auto M = items.MemberBegin(); M != items.MemberEnd(); M++)
		{
			auto item = ContentItemPool::getInstance()->getContentItemForId(M->name.GetString());
			if(item)
			{
				item->setUri(getStringFromJson("location", M->value, item->getUri()));
			}
		}
	}
	if(!hqData.HasMember("rows"))
	{
		return;
	}
	
	HQDataObjectRef hq = HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqName);
	
	hq->clearData();
	
	for (int rowNumber = 0; rowNumber < hqData["rows"].Size(); rowNumber++)
	{
		const rapidjson::Value& rowData = hqData["rows"][rowNumber];
		HQCarouselObjectRef carouselObject = HQCarouselObject::create();
		
		carouselObject->setTitle(getStringFromJson("title", rowData));
		
		if(rowData.HasMember("images"))
		{
			carouselObject->setIcon(getStringFromJson("icon", rowData["images"])); //parsing carousel main icon if present
		}
		
		if(rowData.HasMember("contentIds") && rowData["contentIds"].IsArray() && rowData["contentIds"].Size() != 0)
		{
			const rapidjson::Value& contentIds = rowData["contentIds"];
			for(int elementIndex = 0; elementIndex < contentIds.Size(); elementIndex++)
			{
				if(!contentIds[elementIndex].IsString())
				{
					continue;
				}
				const std::string &contentId = contentIds[elementIndex].GetString();
				
				const HQContentItemObjectRef &pointerToContentItem = ContentItemPool::getInstance()->getContentItemForId(contentId);
				if(pointerToContentItem == nullptr)
				{
					continue;
				}
				
				Vec2 contentItemHighlight = Vec2(1,1);
				if(rowData.HasMember("shapes") && rowData["shapes"].IsArray() && rowData["shapes"].Size() > elementIndex)
				{
					const rapidjson::Value& elementShapeData = rowData["shapes"][elementIndex];
					if(elementShapeData.IsArray() && elementShapeData.Size() == 2 && elementShapeData[0].IsInt() && elementShapeData[1].IsInt())
					{
						contentItemHighlight = Vec2(elementShapeData[0].GetInt(), elementShapeData[1].GetInt());
					}
				}
				
				carouselObject->addContentItemToCarousel(pointerToContentItem);
				carouselObject->addContentItemHighlight(contentItemHighlight);
			}
		}
		
		hq->addCarouselToHq(carouselObject);
		hq->setHqType(hqName);
	}
	
	if(hqData.HasMember("images"))
	{
		hq->setImages(getStringMapFromJson(hqData["images"]));
	}
	
}

void HQStructureDownloadHandler::parseEntitlementData(const std::string &entitlementData)
{
	rapidjson::Document data;
	data.Parse(entitlementData.c_str());
	
	if (data.HasParseError())
	{
		return; //JSON HAS ERRORS IN IT
	}
	
	for (auto M = data.MemberBegin(); M != data.MemberEnd(); M++)
	{
		auto item = ContentItemPool::getInstance()->getContentItemForId(M->name.GetString());
		if(item)
		{
			item->setEntitled(M->value.GetBool());
		}
	}
}
//delegate functions
void HQStructureDownloadHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    rapidjson::Document result;
    result.Parse(body.c_str());
    if(result.HasParseError())
    {
        return;
    }
    
    const std::string& zipUrl = getStringFromJson("uri", result);
    _feedPath = getStringFromJson("id", result);
    
    parseNavigationData(body);
    
    _fileDownloader = FileDownloader::create();
    _fileDownloader->setDelegate(this);
    _fileDownloader->setEtag(getLocalEtag());
    _fileDownloader->downloadFileFromServer(zipUrl);
}

void HQStructureDownloadHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    loadLocalData();
}

void HQStructureDownloadHandler::onFileDownloadComplete(const std::string &fileString, const std::string &tag, long responseCode)
{
    if(responseCode == 200)
    {
        const std::string& dirPath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath + _feedPath;
        if(FileUtils::getInstance()->isDirectoryExist(dirPath))
        {
            FileUtils::getInstance()->removeDirectory(dirPath);
        }
        FileUtils::getInstance()->createDirectory(dirPath);
        setLocalEtag(_fileDownloader->getEtag());
        _fileDownloader = nullptr;
        const std::string& zipPath = dirPath + kZipName;
        cocos2d::FileUtils::getInstance()->writeStringToFile(fileString, zipPath);
        FileZipUtil::getInstance()->asyncUnzip(zipPath, dirPath, "", this);
    }
    else
    {
        loadLocalData();
    }
}

void HQStructureDownloadHandler::onAsyncUnzipComplete(bool success, const std::string &zipPath, const std::string &dirpath)
{
    cocos2d::FileUtils::getInstance()->removeFile(zipPath);
    if(success)
    {
        loadLocalData();
    }
}

NS_AZOOMEE_END
