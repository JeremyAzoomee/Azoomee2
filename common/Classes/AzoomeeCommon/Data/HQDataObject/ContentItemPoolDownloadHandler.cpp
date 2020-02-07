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
#include "../Child/ChildManager.h"
#include "ContentItemManager.h"
#include "../../Utils/DirUtil.h"

NS_AZOOMEE_BEGIN

const std::string ContentItemPoolDownloadHandler::kCachePath = "contentCache/";

static std::auto_ptr<ContentItemPoolDownloadHandler> sContentItemPoolDownloadHandlerSharedInstance;

ContentItemPoolDownloadHandler* ContentItemPoolDownloadHandler::getInstance()
{
    if(!sContentItemPoolDownloadHandlerSharedInstance.get())
    {
        sContentItemPoolDownloadHandlerSharedInstance.reset(new ContentItemPoolDownloadHandler());
    }
    const std::string& cachePath = DirUtil::getCachesPath() + kCachePath;
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
	const std::string& childId = ChildManager::getInstance()->getLoggedInChild()->getId();
    HttpRequestCreator* request = API::GetContentPoolRequest(childId, this);
    request->execute();
}

void ContentItemPoolDownloadHandler::loadLocalData()
{
    if(!ContentItemManager::getInstance()->isSameContentPool(getLocalEtag()))
    {
        const std::string& localDataPath = DirUtil::getCachesPath() + kCachePath + "items.json";
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath);
        ContentItemManager::getInstance()->parseContentItemPool(data);
        ContentItemManager::getInstance()->setPoolEtag(getLocalEtag());
    }
	sendCallback(true);
}

std::string ContentItemPoolDownloadHandler::getCachePath() const
{
	return kCachePath;
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
        const std::string& dirPath = DirUtil::getCachesPath() + kCachePath;
        const std::string& zipPath = dirPath + "contentPool.zip";
        bool success = cocos2d::FileUtils::getInstance()->writeStringToFile(fileString, zipPath);
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
