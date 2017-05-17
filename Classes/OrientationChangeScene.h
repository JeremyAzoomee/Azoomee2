#ifndef __ORIENTATION_CHANGE_SCENE_H__
#define __ORIENTATION_CHANGE_SCENE_H__

#include <cocos/cocos2d.h>

#define ONBOARDING_SCENE 0
#define BASE_SCENE 1
#define ONBOARDING_SUCCESS_SCENE 2
#define CHILD_ACCOUNT_SCENE 3


USING_NS_CC;

class OrientationChangeScene : public cocos2d::Layer
{
private:
    bool forcePortrait;
    int toScene;
    long errorCode;
    
public:
    static Scene* createScene(bool forcePortrait,int toScene, long withErrorCode);
    virtual bool init();
    
    void onEnter();
    virtual void onEnterTransitionDidFinish();
    
    CREATE_FUNC(OrientationChangeScene);
};

#endif
