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
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "FlowDataSingleton.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/ErrorCodes.h>
#include "WebViewSelector.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <ctime>
#include <cstdlib>

using namespace cocos2d;
using namespace cocos2d::network;

NS_AZOOMEE_BEGIN

const char* const GameDataManager::kManualGameId = "MANUAL_GAME";

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

void GameDataManager::startProcessingGame(const HQContentItemObjectRef &itemData)
{
    //Director::getInstance()->replaceScene(SceneManagerScene::createWebview(Azoomee::Orientation::Landscape, "https://games.azoomee.ninja/" + itemData->getContentItemId() + "/export/index-azoomee.html"));
    //Director::getInstance()->replaceScene(SceneManagerScene::createWebview(Azoomee::Orientation::Landscape, "https://media.azoomee.ninja/distribution/global/001e8b25-878c-498b-ac22-59f53c616300/index.html"));
    //displayLoadingScreen();
    //if(!FileUtils::getInstance()->isDirectoryExist(getGameCachePath() + "001e8b25-878c-498b-ac22-59f53c616300"))
    //{
    //    FileUtils::getInstance()->createDirectory(getGameCachePath() + "001e8b25-878c-498b-ac22-59f53c616300");
    //}
    //getGameZipFile("https://media.azoomee.ninja/distribution/global/001e8b25-878c-498b-ac22-59f53c616300/game.zip", "001e8b25-878c-498b-ac22-59f53c616300");
    //return;
    
    AnalyticsSingleton::getInstance()->contentItemProcessingStartedEvent();
    
    processCancelled = false;
    displayLoadingScreen();
    
    performGameCleanup();
    
    saveFeedDataToFile(itemData);
    const std::string &itemId = itemData->getContentItemId();
    const std::string &itemUri = itemData->getUri();
    const std::string &basePath = getGameIdPath(itemId);
    const std::string &fileName = getFileNameFromUrl(itemUri);
    
    WebGameAPIDataManager::getInstance()->setGameId(itemId);

    _contentId = itemId;
    
    saveFeedDataToFile(itemData);
    
    if(checkIfFileExists(basePath + fileName))
    {
        if(HQHistoryManager::getInstance()->isOffline)
        {
            JSONFileIsPresent(itemId);
        }
        else
        {
            getJSONGameData(itemUri, itemId);
        }
    }
    else
    {
        getJSONGameData(itemUri, itemId); //the callback of this method will get back to JSONFileIsPresent
    }
}

void GameDataManager::saveFeedDataToFile(const HQContentItemObjectRef &itemData)
{
    if(HQHistoryManager::getInstance()->isOffline)
    {
        return;
    }
    
    const std::string &basePath = getGameIdPath(itemData->getContentItemId());
    const std::string &targetPath = basePath + "feedData.json";
    
    createGamePathDirectories(basePath);
    FileUtils::getInstance()->writeStringToFile(itemData->getJSONRepresentationOfStructure(), targetPath);
}

void GameDataManager::JSONFileIsPresent(const std::string &itemId)
{
    getContentItemImageForOfflineUsage(itemId);
    
    const std::string &basePath = getGameIdPath(itemId);
    const std::string &basePathWithFileName = basePath + "package.json";
    const std::string &startFile = getStartFileFromJSONFile(basePathWithFileName);
    
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
        std::string uri = getDownloadUrlForGameFromJSONFile(basePathWithFileName);
        uri = uri.substr(0, uri.find_last_of("/"));
        const std::string& startFileNameWithPath = getStartFileFromJSONFile(basePathWithFileName);
        Director::getInstance()->replaceScene(SceneManagerScene::createWebview(getGameOrientation(basePathWithFileName), uri + "/" + startFileNameWithPath));

        const std::string &downloadUrl = getDownloadUrlForGameFromJSONFile(basePathWithFileName);
        getGameZipFile(downloadUrl, itemId); //getGameZipFile callback will call unzipGame and startGame
    }
}

void GameDataManager::createGamePathDirectories(const std::string &basePath)
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

std::string GameDataManager::getFileNameFromUrl(const std::string &url)
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

void GameDataManager::getJSONGameData(const std::string &url, const std::string &itemId)
{
    if(itemId == kManualGameId)
    {
        _contentId = kManualGameId;
    }
    
    _fileDownloader = FileDownloader::create();
    _fileDownloader->setDelegate(this);
    _fileDownloader->downloadFileFromServer(url, _kJsonTag);
}

void GameDataManager::removeOldGameIfUpgradeNeeded(const std::string &downloadedJSONString, const std::string &gameId)
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

std::string GameDataManager::getFeedDataFromFolder(const std::string &feedPath)
{
    if(!FileUtils::getInstance()->isFileExist(feedPath))
    {
        return "";
    }
    return FileUtils::getInstance()->getStringFromFile(feedPath);
}

bool GameDataManager::checkIfFileExists(const std::string &fileWithPath)
{
    return FileUtils::getInstance()->isFileExist(fileWithPath);
}

std::string GameDataManager::getDownloadUrlForGameFromJSONFile(const std::string &jsonFileName)
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    return getStringFromJson("uri", gameData);
}

std::string GameDataManager::getStartFileFromJSONFile(const std::string &jsonFileName)
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    return getStringFromJson("pathToStartPage", gameData);
}

int GameDataManager::getCurrentGameVersionFromJSONFile(const std::string &jsonFileName)
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    if(gameData.HasParseError())
    {
        return 0;
    }
    
    if(gameData.HasMember("currentVersion"))
    {
        if(gameData["currentVersion"].IsInt())
        {
            return gameData["currentVersion"].GetInt();
        }
    }
    
    return 0;
}

int GameDataManager::getMinGameVersionFromJSONString(const std::string &jsonString)
{
    rapidjson::Document gameData;
    gameData.Parse(jsonString.c_str());
    if(gameData.HasParseError())
    {
        return 0;
    }
    
    if(gameData.HasMember("minVersion"))
    {
        if(gameData["minVersion"].IsInt())
        {
            return gameData["minVersion"].GetInt();
        }
    }
    
    return 0;
}

void GameDataManager::getGameZipFile(const std::string &url, const std::string &itemId)
{
    _fileDownloader = FileDownloader::create();
    _fileDownloader->setDelegate(this);
    _fileDownloader->downloadFileFromServer(url, _kZipTag);
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
    
    FileZipUtil::getInstance()->asyncUnzip(zipPath, dirpath,"", this);
    
    /*if(FileZipUtil::getInstance()->unzip(zipPath.c_str(), dirpath.c_str(),nullptr))
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
            const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(dirpath + "package.json");
            rapidjson::Document gameData;
            gameData.Parse(fileContent.c_str());
            std::string uri = gameData["uri"].GetString();
            uri = uri.substr(0, uri.find_last_of("/"));
            Director::getInstance()->replaceScene(SceneManagerScene::createWebview(Azoomee::Orientation::Landscape, uri + startFileNameWithPath));
            return true;;
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
    }*/
    
    return true;
}

bool GameDataManager::removeGameZip(const std::string &fileNameWithPath)
{
    return FileUtils::getInstance()->removeFile(fileNameWithPath);
}

void GameDataManager::startGame(const std::string &basePath, const std::string &fileName)
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
    addTimestampFile(basePath + "lastUsedTimestamp.txt");
    Director::getInstance()->replaceScene(SceneManagerScene::createWebview(getGameOrientation(basePath + "package.json"), basePath + fileName));
}

std::string GameDataManager::getGameIdPath(const std::string &gameId)
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
    if(getBoolFromJson("isPortrait", gameData))
    {
        return Orientation::Portrait;
    }
    
    return Orientation::Landscape;
}

//---------------------LOADING SCREEN----------------------------------
void GameDataManager::displayLoadingScreen()
{
    const Size& size = Director::getInstance()->getVisibleSize();
    const Point& origin = Director::getInstance()->getVisibleOrigin();
    
    ModalMessages::getInstance()->startLoading();
    
    ElectricDreamsButton *cancelButton = ElectricDreamsButton::createWindowCloseButton();
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

void GameDataManager::removeGameFolderOnError(const std::string &dirPath)
{
    FileUtils::getInstance()->removeDirectory(dirPath);
}

bool GameDataManager::isGameCompatibleWithCurrentAzoomeeVersion(const std::string &jsonFileName)
{
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    if(gameData.HasParseError())
    {
        return true;
    }
    
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

void GameDataManager::getContentItemImageForOfflineUsage(const std::string &gameId)
{
    if(imageDownloader)
    {
        imageDownloader.reset();
    }
    
    imageDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    imageDownloader->downloadImage(nullptr, HQDataProvider::getInstance()->getThumbnailUrlForItem(gameId));
}

void GameDataManager::performGameCleanup()
{
    const std::string& basePath = getGameCachePath();
    const std::string& cleanupCheckFile = basePath + "lastCleanupTimestamp.txt";
    if(!FileUtils::getInstance()->isFileExist(cleanupCheckFile))
    {
        setupTimestampFilesForExistingGames();
        return;
    }
    
    if(gameCleanupDue(cleanupCheckFile, _kGameCleanupCheckFreq))
    {
        const std::vector<std::string>& gameDirs = DirectorySearcher::getInstance()->getFoldersInDirectory(basePath);
        for(const std::string& dir : gameDirs)
        {
            const std::string& gameLastUsedFile = basePath + dir + "/lastUsedTimestamp.txt";
            if(gameCleanupDue(gameLastUsedFile, _kGameCleanupUnusedTime))
            {
                removeGameFolderOnError(basePath + dir);
            }
        }
        
        addTimestampFile(cleanupCheckFile);
    }
    
    
}

void GameDataManager::setupTimestampFilesForExistingGames()
{
    const std::string& basePath = getGameCachePath();
    addTimestampFile(basePath + "lastCleanupTimestamp.txt");
    
    const std::vector<std::string>& gameDirs = DirectorySearcher::getInstance()->getFoldersInDirectory(basePath);
    for(const std::string& dir : gameDirs)
    {
        addTimestampFile(basePath + dir + "/lastUsedTimestamp.txt");
    }
}

bool GameDataManager::gameCleanupDue(const std::string& filenameWithPath, const int cleanupInterval)
{
    const std::string& fileTimeStr = FileUtils::getInstance()->getStringFromFile(filenameWithPath);
    const long fileTime = std::strtol(fileTimeStr.c_str(),nullptr,10);
    const std::time_t currentTime = std::time(nullptr);
    if(cleanupInterval < (currentTime - fileTime))
    {
        return true;
    }
    return false;
}

void GameDataManager::addTimestampFile(const std::string& filenameWithPath)
{
    FileUtils::getInstance()->writeStringToFile(StringUtils::format("%i",(int)std::time(nullptr)), filenameWithPath);
}


//--------------- DELEGATE FUNCTIONS ------------------

void GameDataManager::buttonPressed(ElectricDreamsButton *button)
{
    processCancelled = true;
    
    if(_fileDownloader)
    {
        _fileDownloader->setDelegate(nullptr);
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

void GameDataManager::onAsyncUnzipComplete(bool success, std::string zipPath,std::string dirpath)
{
    if(success)
    {
        removeGameZip(zipPath);
        /*
        //Director::getInstance()->getScheduler()->schedule([&](float deltat){
        //    Director::getInstance()->replaceScene(SceneManagerScene::createWebview(Azoomee::Orientation::Landscape, "https://media.azoomee.ninja/distribution/global/001e8b25-878c-498b-ac22-59f53c616300/index.html"));
            
        //}, this, 0, 0, 0.5, false, "startGame");
        
        
        if(!isGameCompatibleWithCurrentAzoomeeVersion(dirpath + "package.json"))
        {
            hideLoadingScreen(); //ERROR TO BE ADDED
            showIncompatibleMessage();
        }
        
        std::string startFileNameWithPath = getStartFileFromJSONFile(dirpath + "package.json");
        
        if(FileUtils::getInstance()->isFileExist(dirpath + startFileNameWithPath))
        {
            const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(dirpath + "package.json");
            rapidjson::Document gameData;
            gameData.Parse(fileContent.c_str());
            std::string uri = gameData["uri"].GetString();
            uri = uri.substr(0, uri.find_last_of("/"));
            //Director::getInstance()->replaceScene(SceneManagerScene::createWebview(Azoomee::Orientation::Landscape, uri + startFileNameWithPath));
            startGame(dirpath, startFileNameWithPath);
        }
        else
        {
            AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
            removeGameFolderOnError(dirpath);
            hideLoadingScreen();
            showErrorMessage();
        }*/
    }
    else
    {
        AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
        removeGameFolderOnError(dirpath);
        hideLoadingScreen();
        showErrorMessage();
    }
}

void GameDataManager::onFileDownloadComplete(const std::string &fileString,const std::string& tag, long responseCode)
{
    if(responseCode != 200)
    {
        AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
        FlowDataSingleton::getInstance()->setErrorCode(ERROR_CODE_SOMETHING_WENT_WRONG);
        LoginLogicHandler::getInstance()->doLoginLogic();
        return;
    }
    if(tag == _kJsonTag)
    {
        removeOldGameIfUpgradeNeeded(fileString, _contentId);
        JSONFileIsPresent(_contentId);
        
    }
    else if(tag == _kZipTag)
    {
        const std::string& basePath = getGameIdPath(_contentId);
        const std::string& targetPath = basePath + "game.zip";
        FileUtils::getInstance()->writeStringToFile(fileString, targetPath);
        
        unzipGame(targetPath, basePath, "");
    }
}

NS_AZOOMEE_END
