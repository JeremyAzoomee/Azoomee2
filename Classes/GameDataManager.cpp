#include "GameDataManager.h"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"

USING_NS_CC;

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "external/json/prettywriter.h"

#include "external/unzip/unzip.h"

#include "WebViewSelector.h"
#include "CookieDataStorage.h"
#include "BackEndCaller.h"
#include "ModalMessages.h"
#include "LoginScene.h"
#include "MessageBox.h"
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include "StringMgr.h"


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
    processCancelled = false;
    displayLoadingScreen();
    
    std::string basePath = getGameIdPath(itemId);
    
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
            getGameZipFile(downloadUrl, itemId); //getGameZipFile callback will call unzipGame and startGame
        }
    }
    else
    {
        if(!FileUtils::getInstance()->isDirectoryExist(this->getGameCachePath())) FileUtils::getInstance()->createDirectory(this->getGameCachePath());
        if(!FileUtils::getInstance()->isDirectoryExist(basePath)) FileUtils::getInstance()->createDirectory(basePath);
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
    jsonRequest = new HttpRequest();
    jsonRequest->setRequestType(HttpRequest::Type::GET);
    jsonRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", CookieDataStorage::getInstance()->dataDownloadCookiesForCpp.c_str()));
    jsonRequest->setHeaders(headers);
    
    CCLOG("Cookies being used are: %s", headers.at(0).c_str());
    
    jsonRequest->setResponseCallback(CC_CALLBACK_2(GameDataManager::onGetJSONGameDataAnswerReceived, this));
    jsonRequest->setTag(itemId);
    HttpClient::getInstance()->setTimeoutForConnect(2);
    HttpClient::getInstance()->setTimeoutForRead(2);
    HttpClient::getInstance()->send(jsonRequest);
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
        std::string basePath = getGameIdPath(response->getHttpRequest()->getTag());
        std::string targetPath = basePath + "package.json";
        FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
        
        std::string startFile = getStartFileFromJson(targetPath);
        if(checkIfFileExists(basePath + startFile))
        {
            startGame(basePath + startFile);
        }
        else
        {
            std::string uri = getDownloadUrlForGame(targetPath);
            getGameZipFile(uri, response->getHttpRequest()->getTag());
        }
        
    }
    else
    {
        Director::getInstance()->replaceScene(LoginScene::createSceneWithAutoLoginAndErrorDisplay());
    }
}

bool GameDataManager::checkIfFileExists(std::string fileWithPath)
{
    return FileUtils::getInstance()->isFileExist(fileWithPath);
}

std::string GameDataManager::getDownloadUrlForGame(std::string jsonFileName)
{
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
    zipRequest = new HttpRequest();
    zipRequest->setRequestType(HttpRequest::Type::GET);
    zipRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", CookieDataStorage::getInstance()->dataDownloadCookiesForCpp.c_str()));
    zipRequest->setHeaders(headers);
    
    CCLOG("Cookies being used are: %s", headers.at(0).c_str());
    
    zipRequest->setResponseCallback(CC_CALLBACK_2(GameDataManager::onGetGameZipFileAnswerReceived, this));
    zipRequest->setTag(itemId);
    HttpClient::getInstance()->setTimeoutForConnect(2);
    HttpClient::getInstance()->setTimeoutForRead(2);
    HttpClient::getInstance()->send(zipRequest);
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
        std::string basePath = getGameIdPath(response->getHttpRequest()->getTag());
        std::string targetPath = basePath + "game.zip";
        FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
        
        unzipGame(targetPath.c_str(), basePath.c_str(), nullptr);
    }
    else
    {
        Director::getInstance()->replaceScene(LoginScene::createSceneWithAutoLoginAndErrorDisplay());
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
                
                unzClose(pFile);
                removeGameFolderOnError(dirpath);
                hideLoadingScreen(); //ERROR TO BE ADDED
                CCLOG("unzip can not create file");
                showErrorMessage();
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
            removeGameFolderOnError(dirpath);
            hideLoadingScreen(); //ERROR TO BE ADDED
            showErrorMessage();
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
        removeGameFolderOnError(dirpath);
        hideLoadingScreen(); //ERROR TO BE ADDED
        showErrorMessage();
        return false;
    }
    unzClose(pFile);
    
    removeGameZip(zipPath);
    std::string startFileNameWithPath = getStartFileFromJson(std::string(dirpath) + "package.json");
    
    if(FileUtils::getInstance()->isFileExist(dirpath + startFileNameWithPath))
    {
       startGame(std::string(dirpath) + startFileNameWithPath);
    }
    else
    {
        removeGameFolderOnError(dirpath);
        hideLoadingScreen();
        showErrorMessage();
        return false;
    }
    
    return ret;

}

bool GameDataManager::removeGameZip(std::string fileNameWithPath)
{
    return FileUtils::getInstance()->removeFile(fileNameWithPath);
}

void GameDataManager::startGame(std::string fileName)
{
    if(processCancelled) return;
    
    if(!FileUtils::getInstance()->isFileExist(fileName))
    {
        hideLoadingScreen();
        showErrorMessage();
        return;
    }
    
    //hideLoadingScreen();
    WebViewSelector::createSceneWithUrl(fileName);
    //We don't need to add this to the screen, because in create phase WebViewSelector will do a replaceScene.
}

std::string GameDataManager::getGameIdPath(std::string gameId)
{
    return FileUtils::getInstance()->getWritablePath() + "gameCache/" + gameId + "/";
}

std::string GameDataManager::getGameCachePath()
{
    return FileUtils::getInstance()->getWritablePath() + "gameCache/";
}

//---------------------LOADING SCREEN----------------------------------
void GameDataManager::displayLoadingScreen()
{
    Size size = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    ModalMessages::getInstance()->startLoading();
    
    ElectricDreamsButton *cancelButton = ElectricDreamsButton::createButtonWithText("Cancel");
    cancelButton->setName("cancelButton");
    cancelButton->setCenterPosition(Vec2(origin.x + size.width / 2, origin.y + size.height * 0.25));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("CancelGameLoading");
    Director::getInstance()->getRunningScene()->addChild(cancelButton);
}
void GameDataManager::hideLoadingScreen()
{
    Director::getInstance()->getRunningScene()->removeChild(Director::getInstance()->getRunningScene()->getChildByName("cancelButton"));
    ModalMessages::getInstance()->stopLoading();
}

void GameDataManager::showErrorMessage()
{
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, this);
}

void GameDataManager::removeGameFolderOnError(std::string dirPath)
{
    FileUtils::getInstance()->removeDirectory(dirPath);
}

//--------------- DELEGATE FUNCTIONS ------------------

void GameDataManager::buttonPressed(ElectricDreamsButton *button)
{
    processCancelled = true;
    
    if(zipRequest) zipRequest->setResponseCallback(nullptr);
    if(jsonRequest) jsonRequest->setResponseCallback(nullptr);
    
    Director::getInstance()->getRunningScene()->removeChild(Director::getInstance()->getRunningScene()->getChildByName("cancelButton"));
    ModalMessages::getInstance()->stopLoading();
}

void GameDataManager::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    std::map<std::string, std::string> errorStringMap = StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_SOMETHING_WENT_WRONG);

    if(messageBoxTitle == errorStringMap[ERROR_TITLE] && buttonTitle == errorStringMap[ERROR_BUTTON])
    {
        HQHistoryManager::getInstance()->emptyHistory();
        auto baseScene = BaseScene::createScene();
        Director::getInstance()->replaceScene(baseScene);
    }
}
