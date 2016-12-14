#include "GameDataManager.h"

#include "network/HttpClient.h"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"

USING_NS_CC;

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "external/json/prettywriter.h"

#include "external/unzip/unzip.h"

#include "DataStorage.h"
#include "WebViewSelector.h"


using namespace network;
using namespace cocos2d;

static GameDataManager *_sharedGameDataManager = NULL;

GameDataManager* GameDataManager::getInstance()
{
    if (! _sharedGameDataManager)
    {
        _sharedGameDataManager = new GameDataManager();
        _sharedGameDataManager->init();
    }
    
    return _sharedGameDataManager;
}

GameDataManager::~GameDataManager(void)
{
}

bool GameDataManager::init(void)
{
    return true;
}

void GameDataManager::startProcessingGame(std::string url, std::string itemId)
{
    std::string basePath = FileUtils::getInstance()->getWritablePath() + itemId + "/";
    
    std::string fileName = getFileNameFromUrl(url);
    if(checkIfFileExists(basePath + fileName))
    {
        std::string startFile = getStartFileFromJson(basePath + fileName);
        
        if(checkIfFileExists(basePath + startFile))
        {
            startGame(basePath + startFile);
        }
        else
        {
            std::string downloadUrl = getDownloadUrlForGame(basePath + fileName);
            getGameZipFile(downloadUrl, itemId); //getGameZipFile will call unzipGame and startGame
        }
    }
    else
    {
        FileUtils::getInstance()->createDirectory(basePath);
        getJSONGameData(url, itemId); //the callback of this method will fire up getDownloadUrlForGame, getGameZipFile, unzipGame and startGame
    }
}

std::string GameDataManager::getFileNameFromUrl(std::string url)
{
    int startPoint = (int)url.find_last_of("/") + 1;
    
    int endPoint = (int)url.length();
    if(url.find("?", 0) != url.npos) endPoint = (int)url.find("?", 0);
    int subLength = endPoint - startPoint;
    
    return url.substr(startPoint, subLength);
}

void GameDataManager::getJSONGameData(std::string url, std::string itemId)
{
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", DataStorage::getInstance()->dataDownloadCookiesForCpp.c_str()));
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(GameDataManager::onGetJSONGameDataAnswerReceived, this));
    request->setTag(itemId);
    HttpClient::getInstance()->send(request);
}

void GameDataManager::onGetJSONGameDataAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
        CCLOG("get game json data: %s", responseString.c_str());
    }
    
    if(response->getResponseCode() == 200)          //Get content success
    {
        std::string basePath = FileUtils::getInstance()->getWritablePath() + response->getHttpRequest()->getTag();
        std::string targetPath = basePath + "/package.json";
        FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
        
        std::string startFile = getStartFileFromJson(targetPath);
        if(checkIfFileExists(basePath + targetPath))
        {
            std::string startFileNameWithPath = getStartFileFromJson(targetPath);
            startGame(basePath + startFileNameWithPath);
        }
        else
        {
            std::string uri = getDownloadUrlForGame(basePath + targetPath);
            getGameZipFile(uri, response->getHttpRequest()->getTag());
        }
        
    }
    else
    {
        CCLOG("GET CONTENT FAIL Response code: %ld", response->getResponseCode());
        CCLOG("GET CONTENT FAIL Response: %s", responseString.c_str());
    }
}

bool GameDataManager::checkIfFileExists(std::string fileWithPath)
{
    return FileUtils::getInstance()->isFileExist(fileWithPath);
}

std::string GameDataManager::getDownloadUrlForGame(std::string jsonFileName)
{
    if(FileUtils::getInstance()->isFileExist(jsonFileName))
    {
        CCLOG("The file exists here: %s", jsonFileName.c_str());
    }
    else
    {
        CCLOG("The file does not exist here: %s", jsonFileName.c_str());
    }
    
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    return gameData["uri"].GetString();
}

std::string GameDataManager::getStartFileFromJson(std::string jsonFileName)
{
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    return gameData["pathToStartPage"].GetString();
}

void GameDataManager::getGameZipFile(std::string url, std::string itemId)
{
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", DataStorage::getInstance()->dataDownloadCookiesForCpp.c_str()));
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(GameDataManager::onGetGameZipFileAnswerReceived, this));
    request->setTag(itemId);
    HttpClient::getInstance()->send(request);
}

void GameDataManager::onGetGameZipFileAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
    }
    
    if(response->getResponseCode() == 200)          //Get content success
    {
        std::string basePath = FileUtils::getInstance()->getWritablePath() + response->getHttpRequest()->getTag();
        std::string targetPath = basePath + "/game.zip";
        FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
        
        CCLOG("Target path: %s", targetPath.c_str());
        
        unzipGame(targetPath.c_str(), basePath.c_str(), nullptr);
    }
    else
    {
        CCLOG("GET CONTENT FAIL Response code: %ld", response->getResponseCode());
        CCLOG("GET CONTENT FAIL Response: %s", responseString.c_str());
    }
}

 bool GameDataManager::unzipGame(const char *zipPath,const char *dirpath,const char *passwd)
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
            
            FILE* pFile2 = fopen(newName.c_str(), "w");
            
            if(!pFile2)
            {
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
            ret = false;
        }
        else
        {
            unzCloseCurrentFile(pFile);
        }
        err = unzGoToNextFile(pFile);
    }
    
    if(err != UNZ_END_OF_LIST_OF_FILE)
    {
        ret = false;
    }
    unzClose(pFile);
    
    removeGameZip(zipPath);
    std::string startFileNameWithPath = getStartFileFromJson(std::string(dirpath) + "/package.json");
    startGame(std::string(dirpath) + "/" + startFileNameWithPath);
    
    return ret;

}

bool GameDataManager::removeGameZip(std::string fileNameWithPath)
{
    return FileUtils::getInstance()->removeFile(fileNameWithPath);
}

void GameDataManager::startGame(std::string fileName)
{
    auto webViewScene = WebViewSelector::createSceneWithUrl(fileName);
    Director::getInstance()->replaceScene(webViewScene);
}
