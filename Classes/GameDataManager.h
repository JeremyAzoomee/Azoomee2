#include <cocos/cocos2d.h>
#include "network/HttpClient.h"
#include "external/json/document.h"
#include "ElectricDreamsButton.h"
#include "MessageBox.h"

class GameDataManager : public cocos2d::Ref, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
    
public:
    static GameDataManager* getInstance(void);
    virtual ~GameDataManager();
    bool init(void);
    void startProcessingGame(std::string url, std::string itemId);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    void getJSONGameData(std::string url, std::string itemId);
    
private:
    std::string getFileNameFromUrl(std::string url);
    
    void onGetJSONGameDataAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    bool checkIfFileExists(std::string fileWithPath);
    
    std::string getDownloadUrlForGame(std::string jsonFileName);
    std::string getStartFileFromJson(std::string jsonFileName);
    
    void getGameZipFile(std::string url, std::string itemId);
    void onGetGameZipFileAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    bool unzipGame(const char *zipPath,const char *dirpath,const char *passwd);
    bool removeGameZip(std::string fileNameWithPath);
    
    void removeGameFolderOnError(std::string dirPath);
    
    std::string getGameCachePath();
    std::string getGameIdPath(std::string gameId);
    
    void startGame(std::string fileName);
    
    //Loading screen
    void displayLoadingScreen();
    void hideLoadingScreen();
    void showErrorMessage();
    
    cocos2d::network::HttpRequest* jsonRequest;
    cocos2d::network::HttpRequest* zipRequest;
    bool processCancelled;
};
