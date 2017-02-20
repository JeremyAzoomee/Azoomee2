#ifndef __OFFLINEHUB_SCENE_VISUALS_H__
#define __OFFLINEHUB_SCENE_VISUALS_H__

#include "cocos2d.h"

class OfflineHubSceneVisuals : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineHubSceneVisuals);
    void onEnter();
    virtual bool init();
    
private:
    void addOomeesToScreen();

};

#endif
