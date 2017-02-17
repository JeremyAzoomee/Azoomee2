#ifndef __OFFLINE_GAME_SEARCH_SCENE_H__
#define __OFFLINE_GAME_SEARCH_SCENE_H__

#include "cocos2d.h"

class OfflineGameSearch : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineGameSearch);
    
    virtual bool init();
    
private:

};

#endif
