#ifndef Azoomee_GameDataManager_h
#define Azoomee_GameDataManager_h

#include <cocos/cocos2d.h>
#include "network/HttpClient.h"
#include "external/json/document.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include "SceneManagerScene.h"
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>

NS_AZOOMEE_BEGIN

class GameDataManager : public cocos2d::Ref, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
    
public:
    ImageDownloaderRef imageDownloader;
    
    static GameDataManager* getInstance(void);
    virtual ~GameDataManager();
    bool init(void);

    void startProcessingGame(const HQContentItemObjectRef &itemData);
    void getJSONGameData(const std::string &url, const std::string &itemId);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
private:
    void saveFeedDataToFile(const HQContentItemObjectRef &itemData);
    std::string getFeedDataFromFolder(const std::string &feedPath);
    
    void JSONFileIsPresent(const std::string &itemId);
    void createGamePathDirectories(const std::string &basePath);
    std::string getFileNameFromUrl(const std::string &url);
    
    void onGetJSONGameDataAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void removeOldGameIfUpgradeNeeded(const std::string &downloadedJSONString, const std::string &gameId);
    bool checkIfFileExists(const std::string &fileWithPath);
    
    std::string getDownloadUrlForGameFromJSONFile(const std::string &jsonFileName);
    std::string getStartFileFromJSONFile(const std::string &jsonFileName);
    int getCurrentGameVersionFromJSONFile(const std::string &jsonFileName);
    int getMinGameVersionFromJSONString(const std::string &jsonString);
    
    void getGameZipFile(const std::string &url, const std::string &itemId);
    void onGetGameZipFileAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    bool unzipGame(const std::string& zipPath,const std::string& dirpath,const std::string& passwd);
    bool removeGameZip(const std::string &fileNameWithPath);
    
    void removeGameFolderOnError(const std::string &dirPath);
    
    std::string getGameCachePath();
    std::string getGameIdPath(const std::string &gameId);
    
    void startGame(const std::string &basePath, const std::string &startFileName);
    
    //Loading screen
    void displayLoadingScreen();
    void hideLoadingScreen();
    void showErrorMessage();
    void showIncompatibleMessage();
    
    cocos2d::network::HttpRequest* jsonRequest;
    cocos2d::network::HttpRequest* zipRequest;
    bool processCancelled = false;
    
    bool isGameCompatibleWithCurrentAzoomeeVersion(const std::string &jsonFileName);
    Orientation getGameOrientation(const std::string& jsonFileName);
    
    void getContentItemImageForOfflineUsage(const std::string &gameId); //if only a 2x2, 2x1 or 1x2 image is present, we still need the 1x1 image for the offline mode to be cached for.
};

NS_AZOOMEE_END

#endif
