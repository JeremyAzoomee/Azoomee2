#ifndef Azoomee_GameDataManager_h
#define Azoomee_GameDataManager_h

#include <cocos/cocos2d.h>
#include "network/HttpClient.h"
#include "external/json/document.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>

NS_AZOOMEE_BEGIN

class GameDataManager : public cocos2d::Ref, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
    
public:
    static GameDataManager* getInstance(void);
    virtual ~GameDataManager();
    bool init(void);
    void startProcessingGame(std::map<std::string, std::string> itemData);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    void getJSONGameData(std::string url, std::string itemId);
    
private:
    void saveFeedDataToFile(std::map<std::string, std::string> itemData);
    std::string getFeedDataFromFolder(std::string feedPath);
    
    void JSONFileIsPresent(std::string itemId);
    void createGamePathDirectories(std::string basePath);
    std::string getFileNameFromUrl(std::string url);
    
    void onGetJSONGameDataAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void removeOldGameIfUpgradeNeeded(std::string downloadedJSONString, std::string gameId);
    bool checkIfFileExists(std::string fileWithPath);
    
    std::string getDownloadUrlForGameFromJSONFile(std::string jsonFileName);
    std::string getStartFileFromJSONFile(std::string jsonFileName);
    int getCurrentGameVersionFromJSONFile(std::string jsonFileName);
    int getMinGameVersionFromJSONString(std::string jsonString);
    
    void getGameZipFile(std::string url, std::string itemId);
    void onGetGameZipFileAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    bool unzipGame(const char *zipPath,const char *dirpath,const char *passwd);
    bool removeGameZip(std::string fileNameWithPath);
    
    void removeGameFolderOnError(std::string dirPath);
    
    std::string getGameCachePath();
    std::string getGameIdPath(std::string gameId);
    
    void startGame(std::string basePath, std::string startFileName);
    
    //Loading screen
    void displayLoadingScreen();
    void hideLoadingScreen();
    void showErrorMessage();
    void showIncompatibleMessage();
    
    cocos2d::network::HttpRequest* jsonRequest;
    cocos2d::network::HttpRequest* zipRequest;
    bool processCancelled = false;
    
    bool isGameCompatibleWithCurrentAzoomeeVersion(std::string jsonFileName);
};

NS_AZOOMEE_END

#endif
