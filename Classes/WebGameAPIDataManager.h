#ifndef __WEBGAMEAPIDATAMANAGER_H__
#define __WEBGAMEAPIDATAMANAGER_H__

#include "cocos2d.h"

class WebGameAPIDataManager : public cocos2d::Ref
{
    
public:
    static WebGameAPIDataManager* getInstance(void);
    virtual ~WebGameAPIDataManager();
    bool init(void);
    char* handleAPIRequest(const char* method, const char* responseId, const char* score);
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

#endif
