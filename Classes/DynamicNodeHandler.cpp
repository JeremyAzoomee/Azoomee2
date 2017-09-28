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


using namespace cocos2d;
NS_AZOOMEE_BEGIN

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
    const std::string CTAPath = getCTADirectoryPath();
    if(!FileUtils::getInstance()->isDirectoryExist(CTAPath))
    {
        FileUtils::getInstance()->createDirectory(CTAPath);
    }
    return true;
}

void DynamicNodeHandler::createDynamicNodeById(const std::string& uniqueId)
{
    //local device folder
    const std::string& CTAPath = getCTADirectoryPath();
    const std::vector<std::string>& folders = DirectorySearcher::getInstance()->getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : folders)
    {
        const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getFilesInDirectory(CTAPath + folder);
        for(const std::string& file : fileNames)
        {
            if(file == uniqueId)
            {
                Node* CTA = DynamicNodeCreator::getInstance()->createCTAFromFile(CTAPath + folder + "/" + file);
                Director::getInstance()->getRunningScene()->addChild(CTA);
                return;
            }
        }
    }
    
    //res folder fallback
    std::string CTAPathFallBack = FileUtils::getInstance()->fullPathForFilename("res/CTA_Assets/close.png");//android needs a file in the dir to locate it
    CTAPathFallBack.substr(0,CTAPathFallBack.size() - 9);
    const std::vector<std::string>& foldersFallBack = DirectorySearcher::getInstance()->getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : foldersFallBack)
    {
        const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getFilesInDirectory(CTAPath + folder);
        for(const std::string& file : fileNames)
        {
            if(file == uniqueId)
            {
                Node* CTA = DynamicNodeCreator::getInstance()->createCTAFromFile(CTAPath + folder + "/" + file);
                Director::getInstance()->getRunningScene()->addChild(CTA);
                return;
            }
        }
    }
    
}

void DynamicNodeHandler::createDynamicNodeByGroupId(const std::string& groupId)
{
    //local device folder
    const std::string& CTAPath = getCTADirectoryPath();
    const std::vector<std::string>& folders = DirectorySearcher::getInstance()->getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : folders)
    {
        if(folder == groupId)
        {
            const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getFilesInDirectory(CTAPath + folder);
            
            Node* CTA = DynamicNodeCreator::getInstance()->createCTAFromFile(CTAPath + folder + "/" + fileNames[rand()%fileNames.size()]);
            Director::getInstance()->getRunningScene()->addChild(CTA);
            return;
                
        }
    }
    
    //res folder fallback
    const std::string& CTAPathFallBack = FileUtils::getInstance()->fullPathForFilename("res/CTA_Assets/close.png");//android needs a file in the dir to locate it
    CTAPathFallBack.substr(0,CTAPath.size() - 9);
    const std::vector<std::string>& foldersFallBack = DirectorySearcher::getInstance()->getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : foldersFallBack)
    {
        if(folder == groupId)
        {
            const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getFilesInDirectory(CTAPath + folder);

            Node* CTA = DynamicNodeCreator::getInstance()->createCTAFromFile(CTAPath + folder + "/" + fileNames[rand()%fileNames.size()]);
            Director::getInstance()->getRunningScene()->addChild(CTA);
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
    if(isCTAPackageJSONExist()){
        const std::string& currentJsonPackage = FileUtils::getInstance()->getStringFromFile(getPackageJsonLocation());
        packageJson.Parse(currentJsonPackage.c_str());
    }
    return packageJson;
}

bool DynamicNodeHandler::isCTAPackageJSONExist()
{
    return FileUtils::getInstance()->isFileExist(getPackageJsonLocation());
}

void DynamicNodeHandler::getCTAPackageJSON(std::string url)
{
    auto jsonRequest = new network::HttpRequest();
    jsonRequest->setRequestType(network::HttpRequest::Type::GET);
    jsonRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", CookieDataProvider::getInstance()->getCookiesForRequest(url).c_str()));
    jsonRequest->setHeaders(headers);
    
    CCLOG("Cookies being used are: %s", headers.at(0).c_str());
    
    jsonRequest->setResponseCallback(CC_CALLBACK_2(DynamicNodeHandler::onGetCTAPackageJSONAnswerReceived , this));
    network::HttpClient::getInstance()->setTimeoutForConnect(2);
    network::HttpClient::getInstance()->setTimeoutForRead(2);
    network::HttpClient::getInstance()->sendImmediate(jsonRequest);
}

void DynamicNodeHandler::onGetCTAPackageJSONAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response && response->getResponseData())
    {
        const std::vector<char>& myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
    }
    
    if(response->getResponseCode() == 200)          //Get content success
    {
        const std::string& targetPath = getPackageJsonLocation();
        rapidjson::Document newPackageJSON;
        newPackageJSON.Parse(responseString.c_str());
        
        if(isCTAPackageJSONExist())
        {
            rapidjson::Document oldPackageJSON = getLocalCTAPackageJSON();
            
            if(oldPackageJSON["currentVersion"].GetInt() < newPackageJSON["currentVersion"].GetInt() && oldPackageJSON["minVersion"].GetInt() >= newPackageJSON["minVersion"].GetInt())
            {
                FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
                getCTAPackageZip(newPackageJSON["uri"].GetString());
            }
        }
        else
        {
            FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
            getCTAPackageZip(newPackageJSON["uri"].GetString());
        }
        
    }
    else
    {
        
    }
}

void DynamicNodeHandler::getCTAPackageZip(std::string url)
{
    auto zipRequest = new network::HttpRequest();
    zipRequest->setRequestType(network::HttpRequest::Type::GET);
    zipRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", CookieDataProvider::getInstance()->getCookiesForRequest(url).c_str()));
    zipRequest->setHeaders(headers);
    
    CCLOG("Cookies being used are: %s", headers.at(0).c_str());
    
    zipRequest->setResponseCallback(CC_CALLBACK_2(DynamicNodeHandler::onGetCTAPackageZipAnswerReceived, this));
    network::HttpClient::getInstance()->setTimeoutForConnect(2);
    network::HttpClient::getInstance()->setTimeoutForRead(2);
    network::HttpClient::getInstance()->sendImmediate(zipRequest);
}

void DynamicNodeHandler::onGetCTAPackageZipAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response && response->getResponseData())
    {
        const std::vector<char>& myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
    }
    
    if(response->getResponseCode() == 200)          //Get content success
    {
        const std::string& basePath = getCTADirectoryPath();
        const std::string& targetPath = basePath + "CTAFiles.zip";
        FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
        removeCTAFiles();
        unzipCTAFiles(targetPath.c_str(), basePath.c_str(), nullptr);
    }
}

std::string DynamicNodeHandler::getPackageJsonLocation()
{
    return getCTADirectoryPath() + "package.json";
}

std::string DynamicNodeHandler::getCTADirectoryPath()
{
    return FileUtils::getInstance()->getWritablePath() + "DCDECache/";
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

bool DynamicNodeHandler::removeCTAFiles()
{
    const std::string& baseLocation = getCTADirectoryPath();
    const std::vector<std::string>& CTAFolders = DirectorySearcher::getInstance()->getFoldersInDirectory(baseLocation);
    for(const std::string& folder : CTAFolders)
    {
        if(folder.size() > 2)
        {
            return FileUtils::getInstance()->removeDirectory(baseLocation + folder);
        }
    }
    
    return false;
}


NS_AZOOMEE_END
