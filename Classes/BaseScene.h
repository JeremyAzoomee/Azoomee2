#ifndef __BASESCENE_SCENE_H__
#define __BASESCENE_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "network/HttpClient.h"

NS_AZOOMEE_BEGIN

class BaseScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(BaseScene);
    
    virtual bool init();
    
    void startBuildingHQs();
    
private:
    void createHQScene2(const std::string &sceneName, Node* toBeAddedTo);
    cocos2d::Layer* createContentLayer();
    void addNavigationLayer();
    void onEnterTransitionDidFinish();
    void addParticleElementsToBackground();
    
    void addXmasDecoration();
};

NS_AZOOMEE_END

#endif
