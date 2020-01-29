//
//  HQStructureDownloadHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 10/05/2018.
//

#include "HQStructureDownloadHandler.h"
#include <cocos/cocos2d.h>
#include "../../API/API.h"
#include "../../Utils/FileZipUtil.h"
#include "../../Utils/DirUtil.h"
#include "../Json.h"
#include "../../UI/ModalMessages.h"
#include "../Child/ChildManager.h"
#include "../ConfigStorage.h"
#include "../../Utils/StringFunctions.h"
#include "HQDataObjectManager.h"
#include "ContentItemManager.h"

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
    const std::string& cachePath = DirUtil::getCachesPath() + kCachePath;
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
        const std::string& localDataPath = DirUtil::getCachesPath() + kCachePath + _feedPath + "/";
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath + "entitlements.json");
		ContentItemManager::getInstance()->parseEntitlementData(data);
        
        const auto& feedsFolders = DirUtil::getFoldersInDirectory(localDataPath);
        for(const auto& folder : feedsFolders)
        {
            if(folder == "groups")
            {
                continue;
            }
            const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(localDataPath + folder + "/feed.json");
            HQDataObjectManager::getInstance()->parseHQStructureData(data, StringFunctions::convertToHQNameString(folder));
        }
        HQDataObjectManager::getInstance()->setHQDataEtag(getLocalEtag());
    }
    ModalMessages::getInstance()->stopLoading();
	sendCallback(true);
}

void HQStructureDownloadHandler::loadGroupHQData(const std::string &groupIdPath)
{
    const std::string& dataPath = DirUtil::getCachesPath() + kCachePath + _feedPath + "/" + groupIdPath;
    if(FileUtils::getInstance()->isFileExist(dataPath))
    {
        const std::string& data = cocos2d::FileUtils::getInstance()->getStringFromFile(dataPath);
        HQDataObjectManager::getInstance()->parseHQStructureData(data, ConfigStorage::kGroupHQName);
    }
}

std::string HQStructureDownloadHandler::getCachePath() const
{
	return kCachePath + _feedPath;
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
    
	HQDataObjectManager::getInstance()->parseNavigationData(body);
    
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

void HQStructureDownloadHandler::onAsyncUnzipComplete(bool success, const std::string &zipPath, const std::string &dirpath)
{
    cocos2d::FileUtils::getInstance()->removeFile(zipPath);
    if(success)
    {
        loadLocalData();
    }
}

NS_AZOOMEE_END
