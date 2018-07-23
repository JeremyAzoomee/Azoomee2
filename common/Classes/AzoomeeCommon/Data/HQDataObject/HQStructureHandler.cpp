//
//  HQStructureHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 10/05/2018.
//

#include "HQStructureHandler.h"
#include "HQStructureParser.h"
#include <cocos/cocos2d.h>
#include "../../API/API.h"
#include "../../Utils/FileZipUtil.h"
#include "../Json.h"
#include "../../UI/ModalMessages.h"
#include "../Parent/ParentDataProvider.h"
#include "../Child/ChildDataProvider.h"
#include "../../Utils/DirectorySearcher.h"
#include "../ConfigStorage.h"
#include "../../Utils/StringFunctions.h"
#include "HQDataObjectStorage.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string HQStructureHandler::kCachePath = "feedsCache/";

static std::auto_ptr<HQStructureHandler> sHQStructureHandlerSharedInstance;

HQStructureHandler* HQStructureHandler::getInstance()
{
    if(!sHQStructureHandlerSharedInstance.get())
    {
        sHQStructureHandlerSharedInstance.reset(new HQStructureHandler());
    }
    const std::string& cachePath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath;
    if(!cocos2d::FileUtils::getInstance()->isDirectoryExist(cachePath))
    {
        cocos2d::FileUtils::getInstance()->createDirectory(cachePath);
    }
    
    return sHQStructureHandlerSharedInstance.get();
}

HQStructureHandler::~HQStructureHandler(void)
{
    
}

void HQStructureHandler::setHQFeedDelegate(HQFeedDelegate* delegate)
{
    _delegate = delegate;
}

void HQStructureHandler::getLatestHQStructureFeed()
{
    ModalMessages::getInstance()->startLoading();
    const std::string& childId = ParentDataProvider::getInstance()->isLoggedInParentAnonymous() ? "anonymous" : ChildDataProvider::getInstance()->getParentOrChildId();
    HttpRequestCreator* request = API::GetHQStructureDataRequest(childId, this);
    request->execute();
}

void HQStructureHandler::loadLocalData()
{
    if(!HQDataObjectStorage::getInstance()->isSameHQData(getLocalEtag()))
    {
        const std::string& localDataPath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath + _feedPath + "/";
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath + "entitlements.json");
        HQStructureParser::getInstance()->parseEntitlementData(data);
        
        const auto& feedsFolders = DirectorySearcher::getInstance()->getFoldersInDirectory(localDataPath);
        for(const auto& folder : feedsFolders)
        {
            if(folder == "groups")
            {
                continue;
            }
            const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath + folder + "/feed.json");
            HQStructureParser::getInstance()->parseHQStructureData(data, convertToHQNameString(folder));
        }
        HQDataObjectStorage::getInstance()->setHQDataEtag(getLocalEtag());
    }
    ModalMessages::getInstance()->stopLoading();
    if(_delegate)
    {
        _delegate->onFeedDownloadComplete();
    }
}

void HQStructureHandler::loadGroupHQData(const std::string &groupIdPath)
{
    const std::string& dataPath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath + _feedPath + "/" + groupIdPath;
    if(FileUtils::getInstance()->isFileExist(dataPath))
    {
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(dataPath);
        HQStructureParser::getInstance()->parseHQStructureData(data, ConfigStorage::kGroupHQName);
    }
}

std::string HQStructureHandler::getLocalEtag()
{
    const std::string& etagFilePath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath + _feedPath + "/etag.txt";
    if(cocos2d::FileUtils::getInstance()->isFileExist(etagFilePath))
    {
        return cocos2d::FileUtils::getInstance()->getStringFromFile(etagFilePath);
    }
    return "";
}
void HQStructureHandler::setLocalEtag(const std::string& etag)
{
    const std::string& etagFilePath = cocos2d::FileUtils::getInstance()->getWritablePath() + kCachePath + _feedPath + "/etag.txt";
    cocos2d::FileUtils::getInstance()->writeStringToFile(etag, etagFilePath);
}

void HQStructureHandler::parseNavigationData(const std::string &data)
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
        hqNames.push_back(getStringFromJson("name", result["navigation"][i]));
        if(getBoolFromJson("default", result["navigation"][i], false))
        {
            ConfigStorage::getInstance()->setDefaultHQ(hqNames.back());
        }
    }
    ConfigStorage::getInstance()->setNavigationHQs(hqNames);
}

//delegate functions
void HQStructureHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
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

void HQStructureHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    loadLocalData();
}

void HQStructureHandler::onFileDownloadComplete(const std::string &fileString, const std::string &tag, long responseCode)
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
        const std::string& zipPath = dirPath + "/feeds.zip";
        cocos2d::FileUtils::getInstance()->writeStringToFile(fileString, zipPath);
        FileZipUtil::getInstance()->asyncUnzip(zipPath, dirPath, "", this);
    }
    else
    {
        loadLocalData();
    }
}

void HQStructureHandler::onAsyncUnzipComplete(bool success, const std::string &zipPath, const std::string &dirpath)
{
    cocos2d::FileUtils::getInstance()->removeFile(zipPath);
    if(success)
    {
        loadLocalData();
    }
}

NS_AZOOMEE_END
