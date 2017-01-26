#ifndef __BASESCENE_SCENE_H__
#define __BASESCENE_SCENE_H__

#include "cocos2d.h"

class BaseScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(BaseScene);
    virtual bool init();
    
private:
    void createHQScene(std::string sceneName, Node* toBeAddedTo);
    cocos2d::Layer* createContentLayer();
    void addMainHubScene(cocos2d::Node *toBeAddedTo);
    void addNavigationLayer();
};

#endif
