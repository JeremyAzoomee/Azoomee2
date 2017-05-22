#ifndef __ORIENTATION_CHANGE_SCENE_H__
#define __ORIENTATION_CHANGE_SCENE_H__

#include <cocos/cocos2d.h>

#define ONBOARDING_SCENE 0
#define BASE_SCENE 1
#define ONBOARDING_SUCCESS_SCENE 2
#define CHILD_ACCOUNT_SCENE_AFTERSIGNUP 3
#define CHILD_ACCOUNT_SCENE_CHILD_CREATION 4
#define CHILD_ACCOUNT_SCENE_CHILD_EDIT 5
#define CHILD_ACCOUNT_SUCCESS_SCENE_CREATION 6
#define CHILD_ACCOUNT_SUCCESS_SCENE_EDITED 7
#define CHILD_SELECTOR_SCENE 8



USING_NS_CC;

class OrientationChangeScene : public cocos2d::Layer
{
private:
    int toScene;
    long errorCode;
    
    std::string _childName;
    int _oomeeAvatarNo;
    
    void ForceToPortrait();
    void ForceToLandscape();
    
public:
    static Scene* createScene(int toScene, long withErrorCode);
    static Scene* createSceneChildAccountNext(int toScene, long withErrorCode, std::string childName, int oomeeAvatarNo);
    virtual bool init();
    
    void onEnter();
    virtual void onEnterTransitionDidFinish();
    
    CREATE_FUNC(OrientationChangeScene);
};

#endif
