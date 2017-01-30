#ifndef __BASESCENE_SCENE_H__
#define __BASESCENE_SCENE_H__

#include "cocos2d.h"

class BaseScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    static cocos2d::Scene* createSceneWithHQ(std::string hqName);
    
    CREATE_FUNC(BaseScene);
    
    virtual bool init();
    
    void addMainHubScene(cocos2d::Node *toBeAddedTo);
    void startBuildingHQs();
    
    std::string hqName;
    
private:
    void createHQScene(std::string sceneName, Node* toBeAddedTo);
    cocos2d::Layer* createContentLayer();
    void addNavigationLayer();
};

#endif
