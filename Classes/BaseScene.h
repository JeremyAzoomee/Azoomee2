#ifndef __BASESCENE_SCENE_H__
#define __BASESCENE_SCENE_H__

#include <cocos/cocos2d.h>

class BaseScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(BaseScene);
    
    virtual bool init();
    
    void addMainHubScene(cocos2d::Node *toBeAddedTo);
    void startBuildingHQs();
    
private:
    void createHQScene(std::string sceneName, Node* toBeAddedTo);
    cocos2d::Layer* createContentLayer();
    void addNavigationLayer();
    void onEnterTransitionDidFinish();
};

#endif
