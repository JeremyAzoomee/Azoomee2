#include "cocos2d.h"
#include "network/HttpClient.h"
#include "external/json/document.h"

class GameDataManager : public cocos2d::Ref
{
    
public:
    static GameDataManager* getInstance(void);
    virtual ~GameDataManager();
    bool init(void);
    void startProcessingGame(std::string url, std::string itemId);
    
private:
    std::string getFileNameFromUrl(std::string url);
    
    void getJSONGameData(std::string url, std::string itemId);
    void onGetJSONGameDataAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    bool checkIfFileExists(std::string fileWithPath);
    
    std::string getDownloadUrlForGame(std::string jsonFileName);
    std::string getStartFileFromJson(std::string jsonFileName);
    
    void getGameZipFile(std::string url, std::string itemId);
    void onGetGameZipFileAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    bool unzipGame(const char *zipPath,const char *dirpath,const char *passwd);
    bool removeGameZip(std::string fileNameWithPath);
    
    std::string getGameCachePath();
    std::string getGameIdPath(std::string gameId);
    
    void startGame(std::string fileName);
    
    //Loading screen
    void displayLoadingScreen();
    void hideLoadingScreen();
    void showErrorMessage();
};
