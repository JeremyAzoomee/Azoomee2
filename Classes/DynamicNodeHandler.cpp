//
//  DynamicNodeHandler.cpp
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#include "DynamicNodeHandler.h"
#include <dirent.h>
#include <AzoomeeCommon/Data/Json.h>
#include <AzoomeeCommon/Data/Cookie/CookieDataProvider.h>
#include <AzoomeeCommon/Utils/FileZipUtil.h>
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <AzoomeeCommon/Utils/VersionChecker.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

//#define USING_LOCAL_CTA_ASSETS YES

using namespace cocos2d;
NS_AZOOMEE_BEGIN
//-----start Popup group names
const std::string DynamicNodeHandler::kUpgradeGroup = "upgrade";

//-----end popup group names

static std::auto_ptr<DynamicNodeHandler> sDynamicNodeHandlerSharedInstance;

DynamicNodeHandler* DynamicNodeHandler::getInstance()
{
    if(!sDynamicNodeHandlerSharedInstance.get())
    {
        sDynamicNodeHandlerSharedInstance.reset(new DynamicNodeHandler());
        sDynamicNodeHandlerSharedInstance->init();
    }
    return sDynamicNodeHandlerSharedInstance.get();
}

DynamicNodeHandler::~DynamicNodeHandler(void)
{
    
}

bool DynamicNodeHandler::init(void)
{
    const std::string& ctaPath = getCTADirectoryPath();
    if(!FileUtils::getInstance()->isDirectoryExist(ctaPath))
    {
        FileUtils::getInstance()->createDirectory(ctaPath);
    }
    return true;
}

void DynamicNodeHandler::createDynamicNodeById(const std::string& uniqueId)
{
    //local device folder
    const std::string& ctaPath = getCTADirectoryPath();
    const std::vector<std::string>& folders = DirectorySearcher::getInstance()->getFoldersInDirectory(ctaPath);
    
    for(const std::string& folder : folders)
    {
        const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getJsonFilesInDirectory(ctaPath + folder);
        for(const std::string& file : fileNames)
        {
            if(file == uniqueId)
            {
                AnalyticsSingleton::getInstance()->ctaWindowAppeared("N/A", uniqueId);
                createDynamicNodeFromFile(ctaPath + folder + "/" + file);
                return;
            }
        }
    }
    
}

void DynamicNodeHandler::createDynamicNodeByGroupId(const std::string& groupId)
{
    //local device folder
    const std::string& ctaPath = getCTADirectoryPath();
    const std::vector<std::string>& folders = DirectorySearcher::getInstance()->getFoldersInDirectory(ctaPath);
    
    for(const std::string& folder : folders)
    {
        if(folder == groupId)
        {
            const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getJsonFilesInDirectory(ctaPath + folder);
            
            int randomFileNameIndex = rand()%fileNames.size();
            AnalyticsSingleton::getInstance()->ctaWindowAppeared(groupId, fileNames[randomFileNameIndex]);
            createDynamicNodeFromFile(ctaPath + folder + "/" + fileNames[randomFileNameIndex]);
            return;
                
        }
    }

}

void DynamicNodeHandler::createDynamicNodeByIdWithParams(const std::string& uniqueId, const std::string& params)
{
    //local device folder
    const std::string& ctaPath = getCTADirectoryPath();
    const std::vector<std::string>& folders = DirectorySearcher::getInstance()->getFoldersInDirectory(ctaPath);
    
    for(const std::string& folder : folders)
    {
        const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getJsonFilesInDirectory(ctaPath + folder);
        for(const std::string& file : fileNames)
        {
            if(file == uniqueId)
            {
                AnalyticsSingleton::getInstance()->ctaWindowAppeared("N/A", uniqueId);
                createDynamicNodeFromFileWithParams(ctaPath + folder + "/" + file, params);
                return;
            }
        }
    }
    
}

void DynamicNodeHandler::getCTAFiles()
{
#ifdef USING_LOCAL_CTA_ASSETS
    removeCTAFiles();
    unzipBundleCTAFiles();
#else
    checkIfVersionChangedFromLastCTAPull();
    getCTAPackageJSON(ConfigStorage::getInstance()->getCTAPackageJsonURL());
#endif
}

rapidjson::Document DynamicNodeHandler::getLocalCTAPackageJSON()
{
    rapidjson::Document packageJson;
    if(isCTAPackageJSONExist())
    {
        const std::string& currentJsonPackage = FileUtils::getInstance()->getStringFromFile(getPackageJsonLocation());
        packageJson.Parse(currentJsonPackage.c_str());
    }
    return packageJson;
}

bool DynamicNodeHandler::isCTAPackageJSONExist()
{
    return FileUtils::getInstance()->isFileExist(getPackageJsonLocation());
}

void DynamicNodeHandler::checkIfVersionChangedFromLastCTAPull()
{
    const std::string& lastPullAppVersionFile = getLastPullAppVersionFilePath();
    if(!FileUtils::getInstance()->isFileExist(lastPullAppVersionFile))
    {
        FileUtils::getInstance()->writeStringToFile(ConfigStorage::getInstance()->getVersionNumber(), lastPullAppVersionFile);
        return;
    }
    
    const std::string& lastPullAppVersion = FileUtils::getInstance()->getStringFromFile(lastPullAppVersionFile);
    
    if(!azoomeeVersionEqualsVersionNumber(lastPullAppVersion))
    {
        removeCTAFiles();
        FileUtils::getInstance()->removeFile(getPackageJsonLocation());
    }
}

void DynamicNodeHandler::getCTAPackageJSON(const std::string& url)
{
    auto jsonRequest = new network::HttpRequest();
    jsonRequest->setRequestType(network::HttpRequest::Type::GET);
    jsonRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers
    {
        StringUtils::format("Cookie: %s", CookieDataProvider::getInstance()->getCookiesForRequest(url).c_str())
    };
    jsonRequest->setHeaders(headers);
    
    jsonRequest->setResponseCallback(CC_CALLBACK_2(DynamicNodeHandler::onGetCTAPackageJSONAnswerReceived , this));
    network::HttpClient::getInstance()->setTimeoutForConnect(2);
    network::HttpClient::getInstance()->setTimeoutForRead(2);
    network::HttpClient::getInstance()->sendImmediate(jsonRequest);
}

void DynamicNodeHandler::onGetCTAPackageJSONAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response && response->getResponseData() && response->getResponseCode() == 200)
    {
        const std::vector<char>& myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
        const std::string& targetPath = getPackageJsonLocation();
        rapidjson::Document newPackageJSON;
        newPackageJSON.Parse(responseString.c_str());
        
        if(!newPackageJSON.HasParseError())
        {
            if(isCTAPackageJSONExist())
            {
                rapidjson::Document oldPackageJSON = getLocalCTAPackageJSON(); //dont need to error check as wont make it on to device if invalid
                
                if(oldPackageJSON["currentVersion"].GetInt() < newPackageJSON["currentVersion"].GetInt() && azoomeeMeetsVersionRequirement(newPackageJSON["minAzoomeeVersion"].GetString()))
                {
                    FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
                    getCTAPackageZip(getStringFromJson("uri", newPackageJSON));
                    return;
                }
            }
            else
            {
                if(azoomeeMeetsVersionRequirement(newPackageJSON["minAzoomeeVersion"].GetString()))
                {
                    FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
                    getCTAPackageZip(getStringFromJson("uri", newPackageJSON));
                    return;
                }
                else
                {
                    unzipBundleCTAFiles();
                    return;
                }
            }
        }
    }
    else
    {
        if(!isCTAPackageJSONExist())
        {
            unzipBundleCTAFiles();
        }
    }
}

void DynamicNodeHandler::getCTAPackageZip(const std::string& url)
{
    auto zipRequest = new network::HttpRequest();
    zipRequest->setRequestType(network::HttpRequest::Type::GET);
    zipRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers
    {
        StringUtils::format("Cookie: %s", CookieDataProvider::getInstance()->getCookiesForRequest(url).c_str())
    };

    zipRequest->setHeaders(headers);
    
    zipRequest->setResponseCallback(CC_CALLBACK_2(DynamicNodeHandler::onGetCTAPackageZipAnswerReceived, this));
    network::HttpClient::getInstance()->setTimeoutForConnect(2);
    network::HttpClient::getInstance()->setTimeoutForRead(2);
    network::HttpClient::getInstance()->sendImmediate(zipRequest);
}

void DynamicNodeHandler::onGetCTAPackageZipAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response && response->getResponseData() && response->getResponseCode() == 200)
    {
        const std::vector<char>& myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
        const std::string& basePath = getCTADirectoryPath();
        const std::string& targetPath = basePath + "CTAFiles.zip";
        FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
        removeCTAFiles();
        unzipCTAFiles(targetPath.c_str(), basePath.c_str(), nullptr);
        FileUtils::getInstance()->writeStringToFile(ConfigStorage::getInstance()->getVersionNumber(), getLastPullAppVersionFilePath());
    }
    else
    {
        unzipBundleCTAFiles();
    }
}

std::string DynamicNodeHandler::getPackageJsonLocation() const
{
    return getCTADirectoryPath() + "package.json";
}

std::string DynamicNodeHandler::getCTADirectoryPath() const
{
    return FileUtils::getInstance()->getWritablePath() + "DCDECache/";
}

std::string DynamicNodeHandler::getBundledAssetsPath() const
{
    return "res/cta_assets/CTAFiles.zip";
}

std::string DynamicNodeHandler::getLastPullAppVersionFilePath() const
{
    return getCTADirectoryPath() + "lastPullAppVersion.txt";
}

bool DynamicNodeHandler::unzipCTAFiles(const char *zipPath, const char *dirpath, const char *passwd)
{
    if(FileZipUtil::getInstance()->unzip(zipPath,dirpath,passwd))
    {
        FileUtils::getInstance()->removeFile(getCTADirectoryPath() + "CTAFiles.zip");
        return true;
    }
    else
    {
        //unable to unzip file
        FileUtils::getInstance()->removeFile(getCTADirectoryPath() + "CTAFiles.zip");
        return false;
    }
}

bool DynamicNodeHandler::unzipBundleCTAFiles()
{
    const std::string& basePath = getCTADirectoryPath();
    const std::string& targetPath = getBundledAssetsPath();
    const std::string& destinationPath = basePath + "CTAFiles.zip";
    const std::string& zipFile = FileUtils::getInstance()->getStringFromFile(targetPath);
    
    FileUtils::getInstance()->writeStringToFile(zipFile, destinationPath);

    if(FileZipUtil::getInstance()->unzip(destinationPath.c_str(),basePath.c_str(),nullptr))
    {
        FileUtils::getInstance()->removeFile(destinationPath);
        return true;
    }
    else
    {
        //unable to unzip file
        FileUtils::getInstance()->removeFile(destinationPath);
        return false;
    }
}

void DynamicNodeHandler::removeCTAFiles()
{
    const std::string& baseLocation = getCTADirectoryPath();
    const std::vector<std::string>& CTAFolders = DirectorySearcher::getInstance()->getFoldersInDirectory(baseLocation);
    for(const std::string& folder : CTAFolders)
    {
        if(folder.size() > 2)
        {
            FileUtils::getInstance()->removeDirectory(baseLocation + folder);
        }
    }
    
}

void DynamicNodeHandler::createDynamicNodeFromFile(const std::string &file)
{
    Node* cta = DynamicNodeCreator::getInstance()->createCTAFromFile(file);
    Director::getInstance()->getRunningScene()->addChild(cta);
}

void DynamicNodeHandler::createDynamicNodeFromFileWithParams(const std::string &file, const std::string& params)
{
    Node* cta = DynamicNodeCreator::getInstance()->createCTAFromFileWithParams(file, params);
    Director::getInstance()->getRunningScene()->addChild(cta);
}


NS_AZOOMEE_END
