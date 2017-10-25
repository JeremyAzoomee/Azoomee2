#ifndef __MAINHUB_SCENE_H__
#define __MAINHUB_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class MainHubScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(MainHubScene);
    static cocos2d::Scene* createScene();
    virtual bool init();
    void onEnter();
    void buildMainHubScene();
    
private:
    void addBackgroundCircles();
    void addBackgroundWires();
    void addImageContainers();
    void addPrivacyButton();
    
    cocos2d::Sprite* createCirclesForBackground(int circleNumber);
    
    cocos2d::Size visibleSize;
    cocos2d::Point origin;
};

NS_AZOOMEE_END

#endif
