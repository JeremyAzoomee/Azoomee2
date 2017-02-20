#ifndef __OFFLINEHUB_SCENE_H__
#define __OFFLINEHUB_SCENE_H__

#include "cocos2d.h"

class OfflineHubScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineHubScene);
    
    virtual bool init();
    void onEnterTransitionDidFinish();
    
private:
    
    void addVisuals();
    void addHubHQ();
    void addOfflineExit();

};

#endif
