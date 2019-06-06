//
//  ContentItemPoolHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 10/05/2018.
//

#include "ContentItemPoolHandler.h"
#include "ContentItemPoolParser.h"
#include "../../API/API.h"
#include <cocos/cocos2d.h>
#include "../Json.h"
#include "../../UI/ModalMessages.h"
#include "../Parent/ParentDataProvider.h"
#include "../Child/ChildManager.h"
#include "ContentItemPool.h"

NS_AZOOMEE_BEGIN

const std::string ContentItemPoolHandler::kCachePath = "contentCache/";

static std::auto_ptr<ContentItemPoolHandler> sContentItemPoolHandlerSharedInstance;

ContentItemPoolHandler* ContentItemPoolHandler::getInstance()
{
    if(!sContentItemPoolHandlerSharedInstance.get())
    {
        sContentItemPoolHandlerSharedInstance.reset(new ContentItemPoolHandler());
    }
    const std::string& cachePath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath;
    if(!cocos2d::FileUtils::getInstance()->isDirectoryExist(cachePath))
    {
        cocos2d::FileUtils::getInstance()->createDirectory(cachePath);
    }
    
    return sContentItemPoolHandlerSharedInstance.get();
}

ContentItemPoolHandler::~ContentItemPoolHandler(void)
{
    
}

void ContentItemPoolHandler::getLatestData(const OnCompleteCallback& callback)
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

void ContentItemPoolHandler::loadLocalData()
{
    if(!ContentItemPool::getInstance()->isSameContentPool(getLocalEtag()))
    {
        const std::string& localDataPath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath + "items.json";
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath);
        ContentItemPoolParser::getInstance()->parseContentItemPool(data);
        ContentItemPool::getInstance()->setPoolEtag(getLocalEtag());
    }
    ModalMessages::getInstance()->stopLoading();
	sendCallback(true);
}

std::string ContentItemPoolHandler::getCachePath() const
{
	return kCachePath;
}
//delegate functions
void ContentItemPoolHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
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

void ContentItemPoolHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    loadLocalData();
}

void ContentItemPoolHandler::onFileDownloadComplete(const std::string &fileString, const std::string &tag, long responseCode)
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

void ContentItemPoolHandler::onAsyncUnzipComplete(bool success, const std::string &zipPath, const std::string &dirpath)
{
    cocos2d::FileUtils::getInstance()->removeFile(zipPath);
    if(success)
    {
        loadLocalData();
    }
}

NS_AZOOMEE_END
