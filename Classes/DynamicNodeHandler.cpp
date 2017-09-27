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
#include "external/unzip/unzip.h"

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
    const std::string CTAPath = FileUtils::getInstance()->getWritablePath() + "DCDECache/";
    if(!FileUtils::getInstance()->isDirectoryExist(CTAPath))
    {
        FileUtils::getInstance()->createDirectory(CTAPath);
    }
    return true;
}

void DynamicNodeHandler::createDynamicNodeById(const std::string& uniqueId)
{
    //local device folder
    std::string CTAPath = FileUtils::getInstance()->getWritablePath() + "DCDECache/";
    std::vector<std::string> folders = getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : folders)
    {
        std::vector<std::string> fileNames = getFilesInDirectory(CTAPath + folder);
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
    CTAPath = FileUtils::getInstance()->fullPathForFilename("res/CTA_Assets/close.png");//android needs a file in the dir to locate it
    CTAPath.substr(0,CTAPath.size() - 9);
    folders = getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : folders)
    {
        std::vector<std::string> fileNames = getFilesInDirectory(CTAPath + folder);
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
    std::string CTAPath = FileUtils::getInstance()->getWritablePath() + "DCDECache/";
    std::vector<std::string> folders = getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : folders)
    {
        if(folder == groupId)
        {
            std::vector<std::string> fileNames = getFilesInDirectory(CTAPath + folder);
            
            Node* CTA = DynamicNodeCreator::getInstance()->createCTAFromFile(CTAPath + folder + "/" + fileNames[rand()%fileNames.size()]);
            Director::getInstance()->getRunningScene()->addChild(CTA);
            return;
                
        }
    }
    
    //res folder fallback
    CTAPath = FileUtils::getInstance()->fullPathForFilename("res/CTA_Assets/close.png");//android needs a file in the dir to locate it
    CTAPath.substr(0,CTAPath.size() - 9);
    folders = getFoldersInDirectory(CTAPath);
    
    for(const std::string& folder : folders)
    {
        if(folder == groupId)
        {
            std::vector<std::string> fileNames = getFilesInDirectory(CTAPath + folder);

            Node* CTA = DynamicNodeCreator::getInstance()->createCTAFromFile(CTAPath + folder + "/" + fileNames[rand()%fileNames.size()]);
            Director::getInstance()->getRunningScene()->addChild(CTA);
            return;
        
        }
    }

}

void DynamicNodeHandler::getCTAFiles()
{
    //getCTAPackageJSON("https://s3-eu-west-1.amazonaws.com/develop4test/package.json");
    //removeCTAFiles();
    //unzipCTAFiles((FileUtils::getInstance()->getWritablePath() + "DCDECache/CTAFiles.zip").c_str(), (FileUtils::getInstance()->getWritablePath() + "DCDECache/").c_str(), nullptr);
    getCTAPackageZip("https://s3-eu-west-1.amazonaws.com/develop4test/CTAFiles.zip");
}

rapidjson::Document DynamicNodeHandler::getLocalCTAPackageJSON()
{
    rapidjson::Document packageJson;
    if(isCTAPackageJSONExist()){
        std::string currentJsonPackage = FileUtils::getInstance()->getStringFromFile(getPackageJsonLocation());
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
        std::vector<char> myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
    }
    
    if(response->getResponseCode() == 200)          //Get content success
    {
        std::string targetPath = getPackageJsonLocation();
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
        std::vector<char> myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
    }
    
    if(response->getResponseCode() == 200)          //Get content success
    {
        std::string basePath = FileUtils::getInstance()->getWritablePath() + "DCDECache/";
        std::string targetPath = basePath + "CTAFiles.zip";
        FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
        removeCTAFiles();
        unzipCTAFiles(targetPath.c_str(), basePath.c_str(), nullptr);
    }
}

std::string DynamicNodeHandler::getPackageJsonLocation()
{
    return FileUtils::getInstance()->getWritablePath() + "DCDECache/package.json";
}

bool DynamicNodeHandler::unzipCTAFiles(const char *zipPath, const char *dirpath, const char *passwd)
{
    static unsigned long  _maxUnzipBufSize = 0x500000;
    CCLOG("unzip fullpath =%s",zipPath);
    unzFile pFile = unzOpen(zipPath);
    if(!pFile)
    {
        return false;
    }
    int err = unzGoToFirstFile(pFile);
    bool ret = true;
    while (err == UNZ_OK)
    {
        int nRet = 0;
        int openRet = 0;
        do
        {
            if(passwd)
            {
                openRet = unzOpenCurrentFilePassword( pFile,passwd);
                CCLOG("openRet %d",openRet);
            }
            else
            {
                openRet = unzOpenCurrentFile(pFile);
            }
            CC_BREAK_IF(UNZ_OK != openRet);
            unz_file_info FileInfo;
            char szFilePathA[260];
            nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
            CC_BREAK_IF(UNZ_OK != nRet);
            std::string newName = std::string(dirpath)+"/"+szFilePathA;
            if (newName[newName.length()-1]=='/')
            {
                FileUtils::getInstance()->createDirectory(newName.c_str());
                continue;
            }
            
            //if (newName.find("package.json") != newName.npos || newName.find("feedData.json") != newName.npos) continue;
            
            FILE* pFile2 = fopen(newName.c_str(), "w");
            
            if(!pFile2)
            {
                unzClose(pFile);
                CCLOG("unzip can not create file");
                return false;
            }
            unsigned long savedSize = 0;
            while(pFile2 != NULL && FileInfo.uncompressed_size > savedSize)
            {
                unsigned char *pBuffer = NULL;
                unsigned long once = FileInfo.uncompressed_size - savedSize;
                if(once > _maxUnzipBufSize)
                {
                    once = _maxUnzipBufSize;
                    pBuffer = new unsigned char[once];
                }
                else
                {
                    pBuffer = new unsigned char[once];
                }
                int nSize = unzReadCurrentFile(pFile, pBuffer, (int)once);
                fwrite(pBuffer, once, 1, pFile2);
                
                savedSize += nSize;
                delete []pBuffer;
            }
            if (pFile2)
            {
                fclose(pFile2);
            }
            
        } while (0);
        if(nRet != UNZ_OK)
        {
            unzClose(pFile);
            return false;
        }
        else
        {
            unzCloseCurrentFile(pFile);
        }
        err = unzGoToNextFile(pFile);
    }
    
    if(err != UNZ_END_OF_LIST_OF_FILE)
    {
        unzClose(pFile);
        return false;
    }
    unzClose(pFile);
    
    FileUtils::getInstance()->removeFile(FileUtils::getInstance()->getWritablePath() + "DCDECache/CTAFiles.zip");
    
    return ret;
}

bool DynamicNodeHandler::removeCTAFiles()
{
    std::string baseLocation = FileUtils::getInstance()->getWritablePath() + "DCDECache/";
    std::vector<std::string> CTAFolders = getFoldersInDirectory(baseLocation);
    for(const std::string& folder : CTAFolders)
    {
        if(folder.size() > 2)
        {
            return FileUtils::getInstance()->removeDirectory(baseLocation + folder);
        }
    }
    
    return false;
}

std::vector<std::string> DynamicNodeHandler::getFilesInDirectory(const std::string& path)
{
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if(ent->d_type == DT_REG)
            {
                fileNames.push_back(ent->d_name);
            }
        }
        closedir (dir);
        return fileNames;
    }
    else
    {
        perror ("");
        return fileNames;
    }
}

std::vector<std::string> DynamicNodeHandler::getFoldersInDirectory(const std::string& path)
{
    std::vector<std::string> dirNames;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if(ent->d_type == DT_DIR)
            {
                dirNames.push_back(ent->d_name);
            }
        }
        closedir (dir);
        return dirNames;
    }
    else
    {
        perror ("");
        return dirNames;
    }
}


NS_AZOOMEE_END
