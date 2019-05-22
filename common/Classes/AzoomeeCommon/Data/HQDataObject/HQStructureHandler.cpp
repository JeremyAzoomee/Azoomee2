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
#include "../Child/ChildManager.h"
#include "../../Utils/ChildManager.h"
#include "../ConfigStorage.h"
#include "../../Utils/StringFunctions.h"
#include "HQDataObjectStorage.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string HQStructureHandler::kCachePath = "feedsCache/";
const std::string HQStructureHandler::kZipName = "feeds.zip";

static std::auto_ptr<HQStructureHandler> sHQStructureHandlerSharedInstance;

HQStructureHandler* HQStructureHandler::getInstance()
{
    if(!sHQStructureHandlerSharedInstance.get())
    {
        sHQStructureHandlerSharedInstance.reset(new HQStructureHandler());
    }
    const std::string& cachePath = DirUtil::getCachesPath() + kCachePath;
    if(!cocos2d::FileUtils::getInstance()->isDirectoryExist(cachePath))
    {
        cocos2d::FileUtils::getInstance()->createDirectory(cachePath);
    }
    
    return sHQStructureHandlerSharedInstance.get();
}

HQStructureHandler::~HQStructureHandler(void)
{
    
}

void HQStructureHandler::getLatestData(const OnCompleteCallback& callback)
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

void HQStructureHandler::loadLocalData()
{
    if(!HQDataObjectStorage::getInstance()->isSameHQData(getLocalEtag()))
    {
        const std::string& localDataPath = DirUtil::getCachesPath() + kCachePath + _feedPath + "/";
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath + "entitlements.json");
        HQStructureParser::getInstance()->parseEntitlementData(data);
        
        const auto& feedsFolders = DirUtil::getFoldersInDirectory(localDataPath);
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
	sendCallback(true);
}

void HQStructureHandler::loadGroupHQData(const std::string &groupIdPath)
{
    const std::string& dataPath = DirUtil::getCachesPath() + kCachePath + _feedPath + "/" + groupIdPath;
    if(FileUtils::getInstance()->isFileExist(dataPath))
    {
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(dataPath);
        HQStructureParser::getInstance()->parseHQStructureData(data, ConfigStorage::kGroupHQName);
    }
}

std::string HQStructureHandler::getCachePath() const
{
	return kCachePath + _feedPath;
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
        const auto& value = result["navigation"][i];
        const std::string& hqName = getStringFromJson("name", value);
        hqNames.push_back(hqName);
        if(getBoolFromJson("default", value, false))
        {
			ConfigStorage::getInstance()->setDefaultHQ(hqNames.back());
        }
        if(value.HasMember("available"))
        {
            HQDataObjectRef dataObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName);
            dataObject->setHqEntitlement(getBoolFromJson("available", value));
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
        const std::string& dirPath = DirUtil::getCachesPath() + kCachePath + _feedPath;
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

void HQStructureHandler::onAsyncUnzipComplete(bool success, const std::string &zipPath, const std::string &dirpath)
{
    cocos2d::FileUtils::getInstance()->removeFile(zipPath);
    if(success)
    {
        loadLocalData();
    }
}

NS_AZOOMEE_END
