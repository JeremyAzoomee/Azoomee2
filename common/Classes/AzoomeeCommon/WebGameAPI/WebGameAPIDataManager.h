#ifndef __WEBGAMEAPIDATAMANAGER_H__
#define __WEBGAMEAPIDATAMANAGER_H__

#include <cocos/cocos2d.h>
#include "../Azoomee.h"

NS_AZOOMEE_BEGIN

class WebGameAPIDataManager
{
public:
    static WebGameAPIDataManager* getInstance();
    virtual ~WebGameAPIDataManager();

    char* handleAPIRequest(const char* method, const char* responseId, const char* sendData);
    void setGameId(std::string gameId);
    void saveLocalStorageData(std::string stringToBeWritten);
    char* getLocalStorageData();
    
private:
    char* createReturnStringForAPI(const char* method, const char* responseId, const char* responseKey, const char* responseValue);
    std::string runningGameId;
    int getCurrentHighScoreForGame();
    int updateCurrentHighScoreForGame(int newScore);
    std::string getPathForHighScoreFile();
    void createDirectoryTree();
    
    std::string getPathForLocalStorageFile();
};

NS_AZOOMEE_END

#endif
