#include "GameDataManager.h"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "external/json/prettywriter.h"
#include <AzoomeeCommon/Utils/FileZipUtil.h>

#include <AzoomeeCommon/Data/Cookie/CookieDataProvider.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "LoginLogicHandler.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include <AzoomeeCommon/Strings.h>
#include "WebGameAPIDataManager.h"
#include <AzoomeeCommon/Utils/VersionChecker.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "FlowDataSingleton.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/ErrorCodes.h>
#include "WebViewSelector.h"

using namespace cocos2d;
using namespace cocos2d::network;

NS_AZOOMEE_BEGIN

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

void GameDataManager::startProcessingGame(std::map<std::string, std::string> itemData)
{
    AnalyticsSingleton::getInstance()->contentItemProcessingStartedEvent();
    
    processCancelled = false;
    displayLoadingScreen();
    
    WebGameAPIDataManager::getInstance()->setGameId(itemData["id"]);
    
    saveFeedDataToFile(itemData);
    
    const std::string& basePath = getGameIdPath(itemData["id"]);
    const std::string& fileName = getFileNameFromUrl(itemData["uri"]);
    
    
    if(checkIfFileExists(basePath + fileName))
    {
        if(HQHistoryManager::getInstance()->isOffline)
        {
            JSONFileIsPresent(itemData["id"]);
        }
        else
        {
            getJSONGameData(itemData["uri"], itemData["id"]);
        }
    }
    else
    {
        getJSONGameData(itemData["uri"], itemData["id"]); //the callback of this method will get back to JSONFileIsPresent
    }
}

void GameDataManager::saveFeedDataToFile(std::map<std::string, std::string> itemData)
{
    if(HQHistoryManager::getInstance()->isOffline || itemData.find("title") == itemData.end() || itemData.find("description") == itemData.end())
    {
        return;
    }
    const std::string& basePath = getGameIdPath(itemData["id"]);
    const std::string& targetPath = basePath + "feedData.json";
    
    createGamePathDirectories(basePath);
    FileUtils::getInstance()->writeStringToFile(getJSONStringFromMap(itemData), targetPath);
}

void GameDataManager::JSONFileIsPresent(std::string itemId)
{
    const std::string& basePath = getGameIdPath(itemId);
    const std::string& basePathWithFileName = basePath + "package.json";
    
    const std::string& startFile = getStartFileFromJSONFile(basePathWithFileName);
    
    if(!isGameCompatibleWithCurrentAzoomeeVersion(basePathWithFileName))
    {
        hideLoadingScreen();
        showIncompatibleMessage();
        return;
    }
    
    if(checkIfFileExists(basePath + startFile))
    {
        startGame(basePath, startFile);
    }
    else
    {
        const std::string& downloadUrl = getDownloadUrlForGameFromJSONFile(basePathWithFileName);
        getGameZipFile(downloadUrl, itemId); //getGameZipFile callback will call unzipGame and startGame
    }
}

void GameDataManager::createGamePathDirectories(std::string basePath)
{
    if(!FileUtils::getInstance()->isDirectoryExist(this->getGameCachePath()))
    {
        FileUtils::getInstance()->createDirectory(this->getGameCachePath());
    }
    if(!FileUtils::getInstance()->isDirectoryExist(basePath))
    {
        FileUtils::getInstance()->createDirectory(basePath);
    }
}

std::string GameDataManager::getFileNameFromUrl(std::string url)
{
    int startPoint = (int)url.find_last_of("/") + 1;
    
    int endPoint = (int)url.length();
    if(url.find("?", 0) != url.npos)
    {
        endPoint = (int)url.find("?", 0);
    }
    int subLength = endPoint - startPoint;
    
    return url.substr(startPoint, subLength);
}

void GameDataManager::getJSONGameData(std::string url, std::string itemId)
{
    jsonRequest = new HttpRequest();
    jsonRequest->setRequestType(HttpRequest::Type::GET);
    jsonRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers{
        StringUtils::format("Cookie: %s", CookieDataProvider::getInstance()->getCookiesForRequest(url).c_str())
    };
    jsonRequest->setHeaders(headers);

    jsonRequest->setResponseCallback(CC_CALLBACK_2(GameDataManager::onGetJSONGameDataAnswerReceived, this));
    jsonRequest->setTag(itemId);
    HttpClient::getInstance()->setTimeoutForConnect(2);
    HttpClient::getInstance()->setTimeoutForRead(2);
    HttpClient::getInstance()->sendImmediate(jsonRequest);
}

void GameDataManager::onGetJSONGameDataAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
    }
    
    if(response->getResponseCode() == 200)          //Get content success
    {
        removeOldGameIfUpgradeNeeded(responseString, response->getHttpRequest()->getTag());
        JSONFileIsPresent(response->getHttpRequest()->getTag());
    }
    else
    {
        AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
        FlowDataSingleton::getInstance()->setErrorCode(ERROR_CODE_SOMETHING_WENT_WRONG);
        LoginLogicHandler::getInstance()->doLoginLogic();
    }
}

void GameDataManager::removeOldGameIfUpgradeNeeded(std::string downloadedJSONString, std::string gameId)
{
    const std::string& basePath = getGameIdPath(gameId);
    const std::string& targetPath = basePath + "package.json";
    const std::string& feedPath = basePath + "feedData.json";
    
    if(!checkIfFileExists(targetPath) || getCurrentGameVersionFromJSONFile(targetPath) < getMinGameVersionFromJSONString(downloadedJSONString))
    {
        const std::string& feedDataToBeSaved = getFeedDataFromFolder(feedPath);
        FileUtils::getInstance()->removeDirectory(basePath);
        createGamePathDirectories(basePath);
        
        if(feedDataToBeSaved != "")
        {
            FileUtils::getInstance()->writeStringToFile(feedDataToBeSaved, feedPath);
        }
        FileUtils::getInstance()->writeStringToFile(downloadedJSONString, targetPath);
    }
}

std::string GameDataManager::getFeedDataFromFolder(std::string feedPath)
{
    if(!FileUtils::getInstance()->isFileExist(feedPath))
    {
        return "";
    }
    return FileUtils::getInstance()->getStringFromFile(feedPath);
}

bool GameDataManager::checkIfFileExists(std::string fileWithPath)
{
    return FileUtils::getInstance()->isFileExist(fileWithPath);
}

std::string GameDataManager::getDownloadUrlForGameFromJSONFile(std::string jsonFileName)
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    return gameData["uri"].GetString();
}

std::string GameDataManager::getStartFileFromJSONFile(std::string jsonFileName)
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    return gameData["pathToStartPage"].GetString();
}

int GameDataManager::getCurrentGameVersionFromJSONFile(std::string jsonFileName)
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    if(gameData.HasMember("currentVersion"))
    {
        if(gameData["currentVersion"].IsInt())
        {
            return gameData["currentVersion"].GetInt();
        }
    }
    
    return 0;
}

int GameDataManager::getMinGameVersionFromJSONString(std::string jsonString)
{
    rapidjson::Document gameData;
    gameData.Parse(jsonString.c_str());
    
    if(gameData.HasMember("minVersion"))
    {
        if(gameData["minVersion"].IsInt())
        {
            return gameData["minVersion"].GetInt();
        }
    }
    
    return 0;
}

void GameDataManager::getGameZipFile(std::string url, std::string itemId)
{
    zipRequest = new HttpRequest();
    zipRequest->setRequestType(HttpRequest::Type::GET);
    zipRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers{
        StringUtils::format("Cookie: %s", CookieDataProvider::getInstance()->getCookiesForRequest(url).c_str())
    };
    zipRequest->setHeaders(headers);
    
    zipRequest->setResponseCallback(CC_CALLBACK_2(GameDataManager::onGetGameZipFileAnswerReceived, this));
    zipRequest->setTag(itemId);
    HttpClient::getInstance()->setTimeoutForConnect(2);
    HttpClient::getInstance()->setTimeoutForRead(2);
    HttpClient::getInstance()->sendImmediate(zipRequest);
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
        const std::string& basePath = getGameIdPath(response->getHttpRequest()->getTag());
        const std::string& targetPath = basePath + "game.zip";
        FileUtils::getInstance()->writeStringToFile(responseString, targetPath);
        
        unzipGame(targetPath, basePath, "");
    }
    else
    {
        AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
        FlowDataSingleton::getInstance()->setErrorCode(ERROR_CODE_SOMETHING_WENT_WRONG);
        LoginLogicHandler::getInstance()->doLoginLogic();
    }
}

 bool GameDataManager::unzipGame(const std::string& zipPath,const std::string& dirpath,const std::string& passwd)
{
    if(!FileUtils::getInstance()->isFileExist(zipPath))
    {
        AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
        hideLoadingScreen();
        showErrorMessage();
        return false;
    }
    
    if(FileZipUtil::getInstance()->unzip(zipPath.c_str(), dirpath.c_str(),nullptr))
    {
    
        removeGameZip(zipPath);
    
        if(!isGameCompatibleWithCurrentAzoomeeVersion(dirpath + "package.json"))
        {
            hideLoadingScreen(); //ERROR TO BE ADDED
            showIncompatibleMessage();
            return false;
        }
    
        std::string startFileNameWithPath = getStartFileFromJSONFile(dirpath + "package.json");
    
        if(FileUtils::getInstance()->isFileExist(dirpath + startFileNameWithPath))
        {
            startGame(dirpath, startFileNameWithPath);
        }
        else
        {
            AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
            removeGameFolderOnError(dirpath);
            hideLoadingScreen();
            showErrorMessage();
            return false;
        }
    }
    else
    {
        AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
        removeGameFolderOnError(dirpath);
        hideLoadingScreen();
        showErrorMessage();
        return false;
    }
    
    return true;
}

bool GameDataManager::removeGameZip(std::string fileNameWithPath)
{
    return FileUtils::getInstance()->removeFile(fileNameWithPath);
}

void GameDataManager::startGame(std::string basePath, std::string fileName)
{
    if(processCancelled) return;
    
    if(!FileUtils::getInstance()->isFileExist(basePath + fileName))
    {
        AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
        hideLoadingScreen();
        showErrorMessage();
        return;
    }
    
    if(!isGameCompatibleWithCurrentAzoomeeVersion(basePath + "package.json"))
    {
        AnalyticsSingleton::getInstance()->contentItemIncompatibleEvent();
        hideLoadingScreen();
        showIncompatibleMessage();
        return;
    }
    
    Director::getInstance()->replaceScene(SceneManagerScene::createWebview(getGameOrientation(basePath + "package.json"), basePath + fileName));
}

std::string GameDataManager::getGameIdPath(std::string gameId)
{
    return FileUtils::getInstance()->getWritablePath() + "gameCache/" + gameId + "/";
}

std::string GameDataManager::getGameCachePath()
{
    return FileUtils::getInstance()->getWritablePath() + "gameCache/";
}

Orientation GameDataManager::getGameOrientation(const std::string& jsonFileName)
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    if(gameData.HasMember("isPortrait"))
        if(gameData["isPortrait"].IsBool() && gameData["isPortrait"].GetBool())
            return Orientation::Portrait;
    
    return Orientation::Landscape;
}

//---------------------LOADING SCREEN----------------------------------
void GameDataManager::displayLoadingScreen()
{
    const Size& size = Director::getInstance()->getVisibleSize();
    const Point& origin = Director::getInstance()->getVisibleOrigin();
    
    ModalMessages::getInstance()->startLoading();
    
    ElectricDreamsButton *cancelButton = ElectricDreamsButton::createWindowCloselButton();
    cancelButton->setName("cancelButton");
    cancelButton->setCenterPosition(Vec2(origin.x + size.width - cancelButton->getContentSize().width, origin.y + size.height - cancelButton->getContentSize().height));
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

void GameDataManager::showIncompatibleMessage()
{
    MessageBox::createWith(ERROR_CODE_GAME_INCOMPATIBLE, this);
}

void GameDataManager::removeGameFolderOnError(std::string dirPath)
{
    FileUtils::getInstance()->removeDirectory(dirPath);
}

bool GameDataManager::isGameCompatibleWithCurrentAzoomeeVersion(std::string jsonFileName)
{
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    if(gameData.HasMember("minAppVersion"))
    {
        if(gameData["minAppVersion"].IsString())
        {
            if(!azoomeeMeetsVersionRequirement(gameData["minAppVersion"].GetString()))
            {
                return false;
            }
        }
    }
    
    return true;
}

//--------------- DELEGATE FUNCTIONS ------------------

void GameDataManager::buttonPressed(ElectricDreamsButton *button)
{
    processCancelled = true;
    
    if(zipRequest)
    {
        zipRequest->setResponseCallback(nullptr);
    }
    if(jsonRequest)
    {
        jsonRequest->setResponseCallback(nullptr);
    }
    
    Director::getInstance()->getRunningScene()->removeChild(Director::getInstance()->getRunningScene()->getChildByName("cancelButton"));
    ModalMessages::getInstance()->stopLoading();
}

void GameDataManager::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    std::map<std::string, std::string> errorStringMap = StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_SOMETHING_WENT_WRONG);

    if(messageBoxTitle == errorStringMap[ERROR_TITLE] && buttonTitle == errorStringMap[ERROR_BUTTON])
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
    }
}

NS_AZOOMEE_END
