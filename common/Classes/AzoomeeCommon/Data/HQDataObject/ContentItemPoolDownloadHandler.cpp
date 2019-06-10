//
//  ContentItemPoolDownloadHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 10/05/2018.
//

#include "ContentItemPoolDownloadHandler.h"
#include "../../API/API.h"
#include <cocos/cocos2d.h>
#include "../Json.h"
#include "../../UI/ModalMessages.h"
#include "../Child/ChildManager.h"
#include "ContentItemPool.h"

NS_AZOOMEE_BEGIN

const std::string ContentItemPoolDownloadHandler::kCachePath = "contentCache/";

static std::auto_ptr<ContentItemPoolDownloadHandler> sContentItemPoolDownloadHandlerSharedInstance;

ContentItemPoolDownloadHandler* ContentItemPoolDownloadHandler::getInstance()
{
    if(!sContentItemPoolDownloadHandlerSharedInstance.get())
    {
        sContentItemPoolDownloadHandlerSharedInstance.reset(new ContentItemPoolDownloadHandler());
    }
    const std::string& cachePath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath;
    if(!cocos2d::FileUtils::getInstance()->isDirectoryExist(cachePath))
    {
        cocos2d::FileUtils::getInstance()->createDirectory(cachePath);
    }
    
    return sContentItemPoolDownloadHandlerSharedInstance.get();
}

ContentItemPoolDownloadHandler::~ContentItemPoolDownloadHandler(void)
{
    
}

void ContentItemPoolDownloadHandler::getLatestData(const OnCompleteCallback& callback)
{
	if(callback)
	{
		_callback = callback;
	}
    ModalMessages::getInstance()->startLoading();
	const std::string& childId = ChildManager::getInstance()->getLoggedInChild()->getId();
    HttpRequestCreator* request = API::GetContentPoolRequest(childId, this);
    request->execute();
}

void ContentItemPoolDownloadHandler::loadLocalData()
{
    if(!ContentItemPool::getInstance()->isSameContentPool(getLocalEtag()))
    {
        const std::string& localDataPath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath + "items.json";
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath);
        parseContentItemPool(data);
        ContentItemPool::getInstance()->setPoolEtag(getLocalEtag());
    }
    ModalMessages::getInstance()->stopLoading();
	sendCallback(true);
}

std::string ContentItemPoolDownloadHandler::getCachePath() const
{
	return kCachePath;
}

void ContentItemPoolDownloadHandler::parseContentItemPool(const std::string& contentItemPoolString)
{
	rapidjson::Document contentData;
	contentData.Parse(contentItemPoolString.c_str());
	if(contentData.HasParseError())
	{
		return;
	}
	
	if(contentData.IsObject())
	{
		if(contentData.HasMember("items"))
		{
			const rapidjson::Value& itemList = contentData["items"];
			if(itemList.IsArray())
			{
				for (auto M = itemList.MemberBegin(); M != itemList.MemberEnd(); M++)
				{
					parseContentItem(M->name.GetString(), M->value);
				}
			}
		}
		else
		{
			for (auto M = contentData.MemberBegin(); M != contentData.MemberEnd(); M++)
			{
				parseContentItem(M->name.GetString(), M->value);
			}
		}
	}
	else
	{
		if(contentData.IsArray())
		{
			for (auto M = contentData.MemberBegin(); M != contentData.MemberEnd(); M++)
			{
				parseContentItem(M->name.GetString(), M->value);
			}
		}
	}
}

void ContentItemPoolDownloadHandler::parseContentItem(const std::string &contentId, const rapidjson::Value &contentItemData)
{
	HQContentItemObjectRef contentObject = HQContentItemObject::create();
	
	contentObject->setContentItemId(contentId);
	contentObject->setTitle(getStringFromJson("title", contentItemData));
	contentObject->setDescription(getStringFromJson("description", contentItemData));
	contentObject->setType(getStringFromJson("type", contentItemData));
	contentObject->setUri(getStringFromJson("uri", contentItemData));
	contentObject->setEntitled(getBoolFromJson("entitled", contentItemData));
	
	if(contentItemData.HasMember("tags"))
	{
		contentObject->setTags(getStringArrayFromJson(contentItemData["tags"]));
	}
	
	if(contentItemData.HasMember("images"))
	{
		contentObject->setImages(getStringMapFromJson(contentItemData["images"]));
	}
	
	ContentItemPool::getInstance()->addContentItemToPool(contentObject);
}

//delegate functions
void ContentItemPoolDownloadHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    rapidjson::Document result;
    result.Parse(body.c_str());
    if(result.HasParseError())
    {
        return;
    }
    
    const std::string& zipUrl = getStringFromJson("uri", result);

	downloadFile(zipUrl);

}

void ContentItemPoolDownloadHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    loadLocalData();
}

void ContentItemPoolDownloadHandler::onFileDownloadComplete(const std::string &fileString, const std::string &tag, long responseCode)
{
    if(responseCode == 200)
    {
        setLocalEtag(_fileDownloader->getEtag());
        _fileDownloader = nullptr;
        const std::string& dirPath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath;
        const std::string& zipPath = dirPath + "/contentPool.zip";
        cocos2d::FileUtils::getInstance()->writeStringToFile(fileString, zipPath);
        FileZipUtil::getInstance()->asyncUnzip(zipPath, dirPath, "", this);
    }
    else
    {
        loadLocalData();
    }
}

void ContentItemPoolDownloadHandler::onAsyncUnzipComplete(bool success, const std::string &zipPath, const std::string &dirpath)
{
    cocos2d::FileUtils::getInstance()->removeFile(zipPath);
    if(success)
    {
        loadLocalData();
    }
}

NS_AZOOMEE_END
