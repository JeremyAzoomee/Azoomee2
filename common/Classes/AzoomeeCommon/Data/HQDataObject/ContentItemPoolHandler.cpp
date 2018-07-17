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
#include "../Child/ChildDataProvider.h"

NS_AZOOMEE_BEGIN

static std::auto_ptr<ContentItemPoolHandler> sContentItemPoolHandlerSharedInstance;

ContentItemPoolHandler* ContentItemPoolHandler::getInstance()
{
    if(!sContentItemPoolHandlerSharedInstance.get())
    {
        sContentItemPoolHandlerSharedInstance.reset(new ContentItemPoolHandler());
    }
    const std::string& cachePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "contentCache/";
    if(!cocos2d::FileUtils::getInstance()->isDirectoryExist(cachePath))
    {
        cocos2d::FileUtils::getInstance()->createDirectory(cachePath);
    }
    
    return sContentItemPoolHandlerSharedInstance.get();
}

ContentItemPoolHandler::~ContentItemPoolHandler(void)
{
    
}

void ContentItemPoolHandler::getLatestContentPool()
{
    const std::string& childId = ParentDataProvider::getInstance()->isLoggedInParentAnonymous() ? "anonymous" : ChildDataProvider::getInstance()->getParentOrChildId();
    HttpRequestCreator* request = API::GetContentPoolRequest(childId, this);
    request->execute();
}

void ContentItemPoolHandler::loadLocalData()
{
    const std::string& localDataPath = cocos2d::FileUtils::getInstance()->getWritablePath() + "contentCache/items.json";
    const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath);
    ContentItemPoolParser::getInstance()->parseContentItemPool(data);
}

bool ContentItemPoolHandler::isNewerVersion(time_t timestamp)
{
    const std::string& timestampPath = cocos2d::FileUtils::getInstance()->getWritablePath() + "contentCache/lastModified.time.stmp";
    const time_t oldTimestamp = atoll(cocos2d::FileUtils::getInstance()->getStringFromFile(timestampPath).c_str());
    return timestamp > oldTimestamp;
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
    time_t lastModified = result["lastModified"].GetUint64();
    if(isNewerVersion(lastModified))
    {
        const std::string& zipUrl = getStringFromJson("uri", result);
        const std::string& timestampPath = cocos2d::FileUtils::getInstance()->getWritablePath() + "contentCache/lastModified.time.stmp";
        std::stringstream ss;
        ss << lastModified;
        cocos2d::FileUtils::getInstance()->writeStringToFile(ss.str(), timestampPath);
        _fileDownloader = FileDownloader::create();
        _fileDownloader->setDelegate(this);
        _fileDownloader->downloadFileFromServer(zipUrl);
    }
    else
    {
        loadLocalData();
    }

}

void ContentItemPoolHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    loadLocalData();
}

void ContentItemPoolHandler::onFileDownloadComplete(const std::string &fileString, const std::string &tag, long responseCode)
{
    _fileDownloader = nullptr;
    const std::string& dirPath = cocos2d::FileUtils::getInstance()->getWritablePath() + "contentCache/";
    const std::string& zipPath = dirPath + "contentPool.zip";
    cocos2d::FileUtils::getInstance()->writeStringToFile(fileString, zipPath);
    FileZipUtil::getInstance()->asyncUnzip(zipPath, dirPath, "", this);
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
