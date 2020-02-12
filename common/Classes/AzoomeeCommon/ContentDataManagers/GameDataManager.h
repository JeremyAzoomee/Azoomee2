#ifndef Azoomee_GameDataManager_h
#define Azoomee_GameDataManager_h

#include <cocos/cocos2d.h>
#include "../Azoomee.h"
#include "../Data/HQDataObject/HQContentItemObject.h"
#include "../ImageDownloader/ImageDownloader.h"
#include "../Utils/FileZipUtil.h"
#include "../Utils/FileDownloader.h"
#include "../Data/HQDataObject/HQCarouselObject.h"
#include "../UI/UIConsts.h"

NS_AZOOMEE_BEGIN

class GameDataManager : public cocos2d::Ref, public FileDownloaderDelegate, public FileZipDelegate
{
public:
    typedef std::function<void(Orientation orientation, const std::string& path, const cocos2d::Vec2& closeButtonAnchor)> OnSuccessCallback;
    typedef std::function<void(const std::string& errorType, long errorCode)> OnFailedCallback;
    
    static GameDataManager* getInstance(void);
    static const char* const kManualGameId;
    static const std::string kGameCacheFolder;
    static const std::map<std::string, cocos2d::Vec2> kCloseAnchorKeyToVec2Map;
    
    static const std::string kZipDownloadError;
    static const std::string kJsonDownloadError;
    static const std::string kVersionIncompatibleError;
    static const std::string kUnzipError;
    static const std::string kStartFileUnavailableError;
    
    virtual ~GameDataManager();
    bool init(void);

    void startProcessingGame(const HQContentItemObjectRef &itemData, const OnSuccessCallback& successCallback, const OnFailedCallback& failedCallback, bool offline);
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
    
    OnSuccessCallback _successCallback = nullptr;
    OnFailedCallback _failedCallback = nullptr;
    
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
    
    void finishGameProcessing(const std::string &basePath, const std::string &startFileName);
    
    FileDownloaderRef _fileDownloader = nullptr;
    ImageDownloaderRef _imageDownloader;
    
    std::string _contentId = "";
    
    bool _gameIsBeingStreamed = false;
    bool _offline = false;
    
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
