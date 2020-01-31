#ifndef Azoomee_GameDataManager_h
#define Azoomee_GameDataManager_h

#include <cocos/cocos2d.h>
#include "network/HttpClient.h"
#include "external/json/document.h"
#include <AzoomeeCommon/Azoomee.h>
#include "SceneManagerScene.h"
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include <AzoomeeCommon/Utils/FileZipUtil.h>
#include <AzoomeeCommon/Utils/FileDownloader.h>
#include <AzoomeeCommon/Data/HQDataObject/HQCarouselObject.h>

NS_AZOOMEE_BEGIN

class GameDataManager : public cocos2d::Ref, public FileDownloaderDelegate, public FileZipDelegate
{
    
public:
    ImageDownloaderRef imageDownloader;
    
    static GameDataManager* getInstance(void);
    static const char* const kManualGameId;
    static const std::string kGameCacheFolder;
    static const std::map<std::string, cocos2d::Vec2> kCloseAnchorKeyToVec2Map;
    
    virtual ~GameDataManager();
    bool init(void);

    void startProcessingGame(const HQContentItemObjectRef &itemData);
    void getJSONGameData(const std::string &url, const std::string &itemId);
    
    bool unzipGame(const std::string& zipPath,const std::string& dirpath,const std::string& passwd);
    
    //Delegate Functions
    void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath);
    void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode);
    
    std::vector<HQContentItemObjectRef> getOfflineGameList();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
public:
    void createBundledGamesMap();
    bool isGameBundled(const std::string& gameId);
    HQCarouselObjectRef createFilteredCarouselForBundledGames(const HQCarouselObjectRef& carousel);
private:
    bool copyBundledGameToCache(const std::string& gameId);
    std::unordered_map<std::string, std::string> _bundedGamesMap;
#endif
    
private:
    //offline game tools
    std::vector<std::string> getJsonFileListFromDir() const;
    
    void saveFeedDataToFile(const HQContentItemObjectRef &itemData);
    std::string getFeedDataFromFolder(const std::string &feedPath);
    
    void JSONFileIsPresent(const std::string &itemId);
    void createGamePathDirectories(const std::string &basePath);
    std::string getFileNameFromUrl(const std::string &url);
    
    void removeOldGameIfUpgradeNeeded(const std::string &downloadedJSONString, const std::string &gameId);
    bool checkIfFileExists(const std::string &fileWithPath);
    
    std::string getDownloadUrlForGameFromJSONFile(const std::string &jsonFileName) const;
    std::string getStartFileFromJSONFile(const std::string &jsonFileName) const;
    int getCurrentGameVersionFromJSONFile(const std::string &jsonFileName) const;
    int getMinGameVersionFromJSONString(const std::string &jsonString) const;
    bool getIsGameStreamableFromJSONFile(const std::string& jsonFileName) const;
    bool getIsGameDownloadableFromJSONFile(const std::string& jsonFileName) const;
    cocos2d::Vec2 getCloseButtonAnchorPointFromJSONFile(const std::string& jsonFileName) const;
    
    void streamGameIfPossible(const std::string& jsonFileName);
    void downloadGameIfPossible(const std::string& jsonFileName, const std::string& itemId);
    
    void getGameZipFile(const std::string &url, const std::string &itemId);
    
    bool removeGameZip(const std::string &fileNameWithPath);
    
    void removeGameFolderOnError(const std::string &dirPath);
    
    std::string getGameCachePath() const;
    std::string getGameIdPath(const std::string &gameId);
    
    void startGame(const std::string &basePath, const std::string &startFileName);
    
    //Loading screen
    void displayLoadingScreen();
    void hideLoadingScreen();
    void showErrorMessage();
    void showIncompatibleMessage();
    
    FileDownloaderRef _fileDownloader = nullptr;
    
    std::string _contentId = "";
    
    bool processCancelled = false;
    
    bool _gameIsBeingStreamed = false;
    
    bool isGameCompatibleWithCurrentAppVersion(const std::string &jsonFileName);
    Orientation getGameOrientation(const std::string& jsonFileName);
    
    void getContentItemImageForOfflineUsage(const std::string &gameId); //if only a 2x2, 2x1 or 1x2 image is present, we still need the 1x1 image for the offline mode to be cached for.

    //game data maintainance functions
    void performGameCleanup();
    void setupTimestampFilesForExistingGames();
    bool gameCleanupDue(const std::string& filenameWithPath, const int cleanupInterval);
    void addTimestampFile(const std::string& filenameWithPath);
    
    const int _kGameCleanupCheckFreq = 432000; //5 days in seconds
    const int _kGameCleanupUnusedTime = 1814400; //21 days in seconds
    
    // file download request tags
    const std::string _kZipTag = "zip";
    const std::string _kJsonTag = "json";
};


NS_AZOOMEE_END

#endif
