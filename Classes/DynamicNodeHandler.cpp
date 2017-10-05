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
        const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getFilesInDirectory(ctaPath + folder);
        for(const std::string& file : fileNames)
        {
            if(file == uniqueId)
            {
                createDynamicNodeFromFile(ctaPath + folder + "/" + file);
                return;
            }
        }
    }
    
    //res folder fallback
    const std::string& ctaPathFallBack = getResCTADirectoryPath();
    const std::vector<std::string>& foldersFallBack = DirectorySearcher::getInstance()->getFoldersInDirectory(ctaPathFallBack);
    
    for(const std::string& folder : foldersFallBack)
    {
        const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getFilesInDirectory(ctaPathFallBack + folder);
        for(const std::string& file : fileNames)
        {
            if(file == uniqueId)
            {
                createDynamicNodeFromFile(ctaPathFallBack + folder + "/" + file);
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
            const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getFilesInDirectory(ctaPath + folder);
            
            createDynamicNodeFromFile(ctaPath + folder + "/" + fileNames[rand()%fileNames.size()]);
            return;
                
        }
    }
    
    //res folder fallback
    const std::string& ctaPathFallBack = getResCTADirectoryPath();
    const std::vector<std::string>& foldersFallBack = DirectorySearcher::getInstance()->getFoldersInDirectory(ctaPathFallBack);
    
    for(const std::string& folder : foldersFallBack)
    {
        if(folder == groupId)
        {
            const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getFilesInDirectory(ctaPathFallBack + folder);
            
            createDynamicNodeFromFile(ctaPathFallBack + folder + "/" + fileNames[rand()%fileNames.size()]);
            return;
        
        }
    }

}

void DynamicNodeHandler::getCTAFiles()
{
    getCTAPackageJSON(_kCTAPackageJSONURL);
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
                }
            }
            else
            {
                if(azoomeeMeetsVersionRequirement(newPackageJSON["minAzoomeeVersion"].GetString()))
                {
                    FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
                    getCTAPackageZip(getStringFromJson("uri", newPackageJSON));
                }
            }
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

std::string DynamicNodeHandler::getResCTADirectoryPath() const
{
    const std::string& localFile = "close.png";
    std::string resDirPath = FileUtils::getInstance()->fullPathForFilename("res/cta_assets/" + localFile);//android needs a file in the dir to locate it
    resDirPath = resDirPath.substr(0,resDirPath.size() - localFile.length());
    return resDirPath;
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


NS_AZOOMEE_END
