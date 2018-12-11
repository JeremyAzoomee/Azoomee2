#ifndef __SCENE_MANAGER_SCENE_H__
#define __SCENE_MANAGER_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Orientation.h>

NS_AZOOMEE_BEGIN

enum SceneNameEnum {
    Login,
    Base,
    BaseWithNoHistory,
    ChildSelector,
    OfflineHub,
    OfflineArtsAppHQ,
    ChatEntryPointScene,
    WebviewPortrait,
    WebviewLandscape,
    ArtAppEntryPointScene,
    OomeeMakerEntryPointScene,
    SettingsFromChat,
    SettingsFromHQ,
	SettingsFromChildSelect,
    introVideo,
    AddChild,
    AddChildFirstTime,
	VodacomOnboarding,
	InteractiveContentFeed
};

class SceneManagerScene : public cocos2d::Layer
{
private:
    
    SceneNameEnum nextScene;
    
    std::string webviewURL;
    
    cocos2d::Vec2 _closeButtonAnchor;
    
    void returnToPrevOrientation();
    
    void forceToPortrait();
    void forceToLandscape();
    void acceptAnyOrientation();
    
public:
    static cocos2d::Scene* createScene(SceneNameEnum sceneName);
    static cocos2d::Scene* createWebview(Orientation _orientation, const std::string& URL, cocos2d::Vec2 closeButtonAnchor = cocos2d::Vec2(0,0));
    
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    CREATE_FUNC(SceneManagerScene);
};

NS_AZOOMEE_END

#endif
