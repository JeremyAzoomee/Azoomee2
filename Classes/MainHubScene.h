#ifndef __MAINHUB_SCENE_H__
#define __MAINHUB_SCENE_H__

#include "cocos2d.h"

class MainHubScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void addBackgroundCircles();
    void addImageContainers();
    
    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    
    CREATE_FUNC(MainHubScene);
};

#endif
