#ifndef __OFFLINE_GAME_SEARCH_SCENE_H__
#define __OFFLINE_GAME_SEARCH_SCENE_H__

#include <cocos/cocos2d.h>

class OfflineGameSearch : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineGameSearch);
    
    virtual bool init();
    std::vector<std::map<std::string, std::string>> getOfflineGamesList();
    
private:
    std::vector<std::string> getJsonFileListFromDir();
    bool isStarterFileExists(std::string gameId);
    std::string getStartFileFromJson(std::string gameId);
    std::map<std::string, std::string> getGameDetails(std::string gameId);
};

#endif
