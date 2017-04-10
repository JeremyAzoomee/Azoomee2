#ifndef __MAINHUB_SCENE_H__
#define __MAINHUB_SCENE_H__

#include <cocos/cocos2d.h>

class MainHubScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(MainHubScene);
    static cocos2d::Scene* createScene();
    virtual bool init();
    void onEnter();
    
private:
    void addBackgroundCircles();
    void addImageContainers();
    
    cocos2d::Sprite* createCirclesForBackground(int circleNumber);
    
    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    float zoomFactor;
};

#endif
