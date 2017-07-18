#ifndef __SCENE_MANAGER_SCENE_H__
#define __SCENE_MANAGER_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

enum SceneNameEnum {
    Login,
    Onboarding,
    Base,
    BaseWithNoHistory,
    ChildAccount,
    ChildAccountSuccessScene,
    ChildSelector,
    OnboardingSuccessScene,
    OfflineHub,
    OfflineArtsAppHQ,
    FTUScene,
    ChatEntryPointScene,
    ArtAppEntryPointScene,
    SettingsFromChat
};

class SceneManagerScene : public cocos2d::Layer
{
private:
    
    SceneNameEnum nextScene;
    
    void forceToPortrait();
    void forceToLandscape();
    void acceptAnyOrientation();
    
public:
    static cocos2d::Scene* createScene(SceneNameEnum sceneName);
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    CREATE_FUNC(SceneManagerScene);
};

NS_AZOOMEE_END

#endif
