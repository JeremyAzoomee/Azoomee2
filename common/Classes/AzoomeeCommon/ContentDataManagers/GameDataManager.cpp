#include "GameDataManager.h"
#include "../Data/Json.h"
#include "../Utils/FileZipUtil.h"
#include "../Utils/LocaleManager.h"
#include "../WebGameAPI/WebGameAPIDataManager.h"
#include "../Data/AppConfig.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "../Utils/StringFunctions.h"
#include "../Utils/DirUtil.h"
#include "../Data/HQDataObject/ContentItemManager.h"

using namespace cocos2d;
using namespace cocos2d::network;

NS_TZ_BEGIN

const char* const GameDataManager::kManualGameId = "MANUAL_GAME";
const std::string GameDataManager::kGameCacheFolder = "gameCache/";

const std::string GameDataManager::kZipDownloadError = "zipDownloadError";
const std::string GameDataManager::kJsonDownloadError = "jsonDownloadError";
const std::string GameDataManager::kVersionIncompatibleError = "versionIncompatibleError";
const std::string GameDataManager::kUnzipError = "unzipError";
const std::string GameDataManager::kStartFileUnavailableError = "startFileUnavailableError";

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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    createBundledGamesMap();
    
    // copy games loading pages from bundle to games cache so game data can be accessed in the new WKWebView
    const std::string& cacheFolder = TZ::DirUtil::getCachesPath() + kGameCacheFolder;
    FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile("res/webcommApi/index_ios.html"), cacheFolder + "index_ios.html");
    FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile("res/webcommApi/circle_1.png"), cacheFolder + "circle_1.png");
    FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile("res/webcommApi/load.png"), cacheFolder + "load.png");
    FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile("res/webcommApi/style.css"), cacheFolder + "style.css");
#endif
    
    _imageDownloader = ImageDownloader::create(ImageDownloader::kImageCachePath, ImageDownloader::CacheMode::File);
    
    return true;
}

std::vector<HQContentItemObjectRef> GameDataManager::getOfflineGameList()
{
    std::vector<HQContentItemObjectRef> gameList;
    const std::vector<std::string>& jsonList = getJsonFileListFromDir();
    
    for(const auto& json : jsonList)
    {
        if(json.length() > 3)
        {
            const std::string& jsonFilepath = getGameCachePath() + json + "/package.json";
            if(getStartFileFromJSONFile(jsonFilepath) != "")
            {
                auto item = ContentItemManager::getInstance()->getContentItemForId(json);
                if(item && item->isEntitled())
                {
                    gameList.push_back(item);
                }
            }
        }
    }
    return gameList;
}

std::vector<std::string> GameDataManager::getJsonFileListFromDir() const
{
    return DirUtil::getFoldersInDirectory(getGameCachePath());
}

void GameDataManager::startProcessingGame(const HQContentItemObjectRef &itemData, const OnSuccessCallback& successCallback, const OnFailedCallback& failedCallback, bool offline)
{
    _successCallback = successCallback;
    _failedCallback = failedCallback;
    
    _offline = offline;
    
    AnalyticsSingleton::getInstance()->contentItemProcessingStartedEvent();
    
    _gameIsBeingStreamed = false;
    
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
        if(_offline)
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
    if(_offline)
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
    
    if(!isGameCompatibleWithCurrentAppVersion(basePathWithFileName))
    {
        if(_failedCallback)
        {
            _failedCallback(kVersionIncompatibleError, 200);
        }
        return;
    }
    
    if(checkIfFileExists(basePath + startFile))
    {
        finishGameProcessing(basePath, startFile);
    }
    else if(!_offline)
    {
        streamGameIfPossible(basePathWithFileName);
        
        downloadGameIfPossible(basePathWithFileName, itemId);
    }
    else
    {
        if(_failedCallback)
        {
            _failedCallback(kStartFileUnavailableError, 200);
        }
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
        if(_successCallback)
        {
            _successCallback(getGameOrientation(jsonFileName), uri + "/" + startFileNameWithPath, getCloseButtonAnchorPointFromJSONFile(jsonFileName));
        }
        _gameIsBeingStreamed = true;
    }
}

void GameDataManager::downloadGameIfPossible(const std::string &jsonFileName, const std::string& itemId)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    if(getIsGameDownloadableFromJSONFile(jsonFileName))
    {
        const std::string &downloadUrl = getDownloadUrlForGameFromJSONFile(jsonFileName);
        getGameZipFile(downloadUrl, itemId); //getGameZipFile callback will call unzipGame and finishGameProcessing
    }
    else
#else
    if(!copyBundledGameToCache(itemId))
#endif
    {
        // override for ios if manual game input used
        if(itemId == kManualGameId)
        {
            const std::string &downloadUrl = getDownloadUrlForGameFromJSONFile(jsonFileName);
            getGameZipFile(downloadUrl, itemId); //getGameZipFile callback will call unzipGame and finishGameProcessing
        }
        else if(!_gameIsBeingStreamed)
        {
            if(_failedCallback)
            {
                _failedCallback(kZipDownloadError, 404);
            }
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
        if(_failedCallback)
        {
            _failedCallback(kUnzipError, 200);
        }
        return false;
    }
    
    FileZipUtil::getInstance()->asyncUnzip(zipPath, dirpath,"", this);
    
    return true;
}

bool GameDataManager::removeGameZip(const std::string &fileNameWithPath)
{
    return FileUtils::getInstance()->removeFile(fileNameWithPath);
}

void GameDataManager::finishGameProcessing(const std::string &basePath, const std::string &fileName)
{
    if(!FileUtils::getInstance()->isFileExist(basePath + fileName))
    {
        AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
        if(_failedCallback)
        {
            _failedCallback(kStartFileUnavailableError, 200);
        }
        return;
    }
    
    if(!isGameCompatibleWithCurrentAppVersion(basePath + "package.json"))
    {
        AnalyticsSingleton::getInstance()->contentItemIncompatibleEvent();
        if(_failedCallback)
        {
            _failedCallback(kVersionIncompatibleError, 200);
        }
        return;
    }
    addTimestampFile(basePath + "lastUsedTimestamp.txt");
    if(_successCallback)
    {
        _successCallback(getGameOrientation(basePath + "package.json"), basePath + fileName, getCloseButtonAnchorPointFromJSONFile(basePath + "package.json"));
    }
}

std::string GameDataManager::getGameIdPath(const std::string &gameId)
{
    return getGameCachePath() + gameId + "/";
}

std::string GameDataManager::getGameCachePath() const
{
    return DirUtil::getCachesPath() + kGameCacheFolder;
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

void GameDataManager::removeGameFolderOnError(const std::string &dirPath)
{
    FileUtils::getInstance()->removeDirectory(dirPath);
}

bool GameDataManager::isGameCompatibleWithCurrentAppVersion(const std::string &jsonFileName)
{
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    const std::string& minAppVersion = getStringFromJson("minAppVersion", gameData);

    return ( minAppVersion.empty() || StringFunctions::compareVersionNumbers(minAppVersion, AppConfig::getInstance()->getVersionNumber()) >= 0);
}

void GameDataManager::getContentItemImageForOfflineUsage(const std::string &gameId)
{
    _imageDownloader->downloadImage(nullptr, ContentItemManager::getInstance()->getThumbnailUrlForItem(gameId));
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
        const std::vector<std::string>& gameDirs = DirUtil::getFoldersInDirectory(basePath);
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
    
    const std::vector<std::string>& gameDirs = DirUtil::getFoldersInDirectory(basePath);
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

void GameDataManager::onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath)
{
    if(success)
    {
        removeGameZip(zipPath);
        if(!_gameIsBeingStreamed)
        {
            const std::string &basePathWithFileName = dirpath + "package.json";
            const std::string &startFile = getStartFileFromJSONFile(basePathWithFileName);
            finishGameProcessing(dirpath, startFile);
        }
    }
    else
    {
        AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
        removeGameFolderOnError(dirpath);
        if(_failedCallback)
        {
            _failedCallback(kUnzipError, 200);
        }
    }
}

void GameDataManager::onFileDownloadComplete(const std::string &fileString,const std::string& tag, long responseCode)
{
    if(responseCode != 200)
    {
        AnalyticsSingleton::getInstance()->contentItemProcessingErrorEvent();
        if(_failedCallback)
        {
            _failedCallback(tag == _kJsonTag ? kJsonDownloadError : kZipDownloadError, responseCode);
        }
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

//IOS GAME BUNDLING FUNCS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void GameDataManager::createBundledGamesMap()
{
    _bundedGamesMap.clear();
    const std::string& fileExt = ".zip";
    const std::string& localDir = "bundledGames/";
    const std::string& fullPath = FileUtils::getInstance()->fullPathForFilename(localDir + "dirLocator.png"); //use dummy file to get full path to res dir to use with DirUtil
    const std::string& bundledGamesDir = fullPath.substr(0,fullPath.find(localDir)) + localDir;
    const auto& filenames = DirUtil::getFilesInDirectoryWithExtention(bundledGamesDir, fileExt);
    for(const std::string& filename : filenames)
    {
        const std::string& gameId = filename.substr(0, filename.length() - fileExt.length());
        _bundedGamesMap[gameId] = bundledGamesDir + filename;
    }
}

bool GameDataManager::isGameBundled(const std::string& gameId)
{
    return _bundedGamesMap.find(gameId) != _bundedGamesMap.end();
}

HQCarouselObjectRef GameDataManager::createFilteredCarouselForBundledGames(const HQCarouselObjectRef &carousel)
{
    MutableHQCarouselObjectRef output = MutableHQCarouselObject::create();
    output->setColour(carousel->getColour());
    output->setTitle(carousel->getTitle());
    output->setIcon(carousel->getIcon());
    const auto& items = carousel->getContentItems();
    for(const auto& item : items)
    {
        if(isGameBundled(item->getContentItemId()) || item->getType() == HQContentItemObject::kContentTypeManual)
        {
            output->addContentItemToCarousel(item);
        }
    }
    return output;
}

bool GameDataManager::copyBundledGameToCache(const std::string& gameId)
{
    if(!isGameBundled(gameId))
    {
        return false;
    }
    const std::string& zipData = FileUtils::getInstance()->getStringFromFile(_bundedGamesMap.at(gameId));
    const std::string& basePath = getGameIdPath(_contentId);
    const std::string& targetPath = basePath + "game.zip";
    FileUtils::getInstance()->writeStringToFile(zipData, targetPath);
    
    unzipGame(targetPath, basePath, "");
    return true;
}
#endif

NS_TZ_END
