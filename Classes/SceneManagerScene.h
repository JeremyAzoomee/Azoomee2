#ifndef __SCENE_MANAGER_SCENE_H__
#define __SCENE_MANAGER_SCENE_H__

#include <cocos/cocos2d.h>

enum SceneNameEnum { Login, Onboarding,Base, BaseWithNoHistory, ChildAccount, ChildAccountSuccessScene, ChildSelector, OnboardingSuccessScene,OfflineHub,OfflineArtsAppHQ};

USING_NS_CC;

class SceneManagerScene : public cocos2d::Layer
{
private:
    
    SceneNameEnum nextScene;
    
    void forceToPortrait();
    void forceToLandscape();
    
public:
    static Scene* createScene(SceneNameEnum sceneName);
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    CREATE_FUNC(SceneManagerScene);
};

#endif