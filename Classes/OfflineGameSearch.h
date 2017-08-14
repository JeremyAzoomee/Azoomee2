#ifndef __OFFLINE_GAME_SEARCH_SCENE_H__
#define __OFFLINE_GAME_SEARCH_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class OfflineGameSearch : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineGameSearch);
    
    virtual bool init();
    std::vector<std::map<std::string, std::string>> getOfflineGamesList();
    
private:
    std::vector<std::string> getJsonFileListFromDir();
    bool isStarterFileExists(const std::string &gameId);
    std::string getStartFileFromJson(const std::string &gameId);
    std::map<std::string, std::string> getGameDetails(const std::string &gameId);
};

NS_AZOOMEE_END

#endif
