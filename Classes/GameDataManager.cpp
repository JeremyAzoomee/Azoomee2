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

const std::map<std::string, Vec2> GameDataManager::kCloseAnchorKeyToVec2Map = {
    {"TOP_LEFT",Vec2(0,0)},
    {"MIDDLE_LEFT",Vec2(0,0.5)},
    {"BOTTOM_LEFT", Vec2(0,1)},
    {"TOP_RIGHT",Vec2(1,0)},
    {"MIDDLE_RIGHT",Vec2(1,0.5)},
    {"BOTTOM_RIGHT", Vec2(1,1)}
};

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
    AnalyticsSingleton::getInstance()->contentItemProcessingStartedEvent();
    
    processCancelled = false;
    _gameIsBeingStreamed = false;
    
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
    else if(!HQHistoryManager::getInstance()->isOffline)
    {
        streamGameIfPossible(basePathWithFileName);
        
        downloadGameIfPossible(basePathWithFileName, itemId);
    }
    else
    {
        hideLoadingScreen();
        return;
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

std::string GameDataManager::getDownloadUrlForGameFromJSONFile(const std::string &jsonFileName) const
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    return getStringFromJson("uri", gameData);
}

std::string GameDataManager::getStartFileFromJSONFile(const std::string &jsonFileName) const
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    return getStringFromJson("pathToStartPage", gameData);
}

int GameDataManager::getCurrentGameVersionFromJSONFile(const std::string &jsonFileName) const
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    return getIntFromJson("currentVersion", gameData, 0);
}

bool GameDataManager::getIsGameStreamableFromJSONFile(const std::string &jsonFileName) const
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    return getBoolFromJson("isStreamable", gameData);
}

bool GameDataManager::getIsGameDownloadableFromJSONFile(const std::string &jsonFileName) const
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    return getBoolFromJson("isDownloadable", gameData, true);
}

int GameDataManager::getMinGameVersionFromJSONString(const std::string &jsonString) const
{
    rapidjson::Document gameData;
    gameData.Parse(jsonString.c_str());
    
    return getIntFromJson("minVersion", gameData, 0);
}

Vec2 GameDataManager::getCloseButtonAnchorPointFromJSONFile(const std::string &jsonFileName) const
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    const std::string& anchorKey = getStringFromJson("closeButtonPosition", gameData, "TOP_LEFT");
    if(kCloseAnchorKeyToVec2Map.find(anchorKey) != kCloseAnchorKeyToVec2Map.end())
    {
        return kCloseAnchorKeyToVec2Map.at(anchorKey);
    }
    return Vec2(0,0);
}

void GameDataManager::streamGameIfPossible(const std::string &jsonFileName)
{
    if(getIsGameStreamableFromJSONFile(jsonFileName))
    {
        std::string uri = getDownloadUrlForGameFromJSONFile(jsonFileName);
        uri = uri.substr(0, uri.find_last_of("/"));
        const std::string& startFileNameWithPath = getStartFileFromJSONFile(jsonFileName);
        Director::getInstance()->replaceScene(SceneManagerScene::createWebview(getGameOrientation(jsonFileName), uri + "/" + startFileNameWithPath, getCloseButtonAnchorPointFromJSONFile(jsonFileName)));
        _gameIsBeingStreamed = true;
    }
}

void GameDataManager::downloadGameIfPossible(const std::string &jsonFileName, const std::string& itemId)
{
    if(getIsGameDownloadableFromJSONFile(jsonFileName))
    {
        const std::string &downloadUrl = getDownloadUrlForGameFromJSONFile(jsonFileName);
        getGameZipFile(downloadUrl, itemId); //getGameZipFile callback will call unzipGame and startGame
    }
    else
    {
        if(!_gameIsBeingStreamed)
        {
            hideLoadingScreen();
            showErrorMessage();
        }
    }
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
    
    return true;
}

bool GameDataManager::removeGameZip(const std::string &fileNameWithPath)
{
    return FileUtils::getInstance()->removeFile(fileNameWithPath);
}

void GameDataManager::startGame(const std::string &basePath, const std::string &fileName)
{
    if(processCancelled)
    {
        return;
    }
    
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
    Director::getInstance()->replaceScene(SceneManagerScene::createWebview(getGameOrientation(basePath + "package.json"), basePath + fileName, getCloseButtonAnchorPointFromJSONFile(basePath + "package.json")));
}

std::string GameDataManager::getGameIdPath(const std::string &gameId)
{
    return ConfigStorage::getInstance()->getGameCachePath() + gameId + "/";
}

std::string GameDataManager::getGameCachePath()
{
    return ConfigStorage::getInstance()->getGameCachePath();
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
    const Point& origin = Vec2(0,0);
    
    ModalMessages::getInstance()->startLoading();
    
    ElectricDreamsButton *cancelButton = ElectricDreamsButton::createWindowCloseButton();
    cancelButton->setName("cancelButton");
    cancelButton->setCenterPosition(Vec2(origin.x + size.width - cancelButton->getContentSize().width, origin.y + size.height - cancelButton->getContentSize().height));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("CancelGameLoading");
    Director::getInstance()->getRunningScene()->addChild(cancelButton, 9999);
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
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    const std::string& minAppVersion = getStringFromJson("minAppVersion", gameData);

    return ( minAppVersion.empty() || azoomeeMeetsVersionRequirement(minAppVersion) );
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

void GameDataManager::onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath)
{
    if(success)
    {
        removeGameZip(zipPath);
        if(!_gameIsBeingStreamed)
        {
            const std::string &basePathWithFileName = dirpath + "package.json";
            const std::string &startFile = getStartFileFromJSONFile(basePathWithFileName);
            startGame(dirpath, startFile);
        }
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
