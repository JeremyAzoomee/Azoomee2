#ifndef __INTRO_VIDEO_SCENE_H__
#define __INTRO_VIDEO_SCENE_H__

#include "cocos2d.h"
#include "ui/UIVideoPlayer.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d::experimental::ui;

class IntroVideoScene: public cocos2d::Layer
{
private:
    void videoEventCallback(Ref* sender, VideoPlayer::EventType eventType);
    void navigateToNextScene();
    std::string videoErrorText;
    
    bool isNavigatingToNextScene;
    
    cocos2d::experimental::ui::VideoPlayer* videoPlayer;
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(IntroVideoScene);
};

#endif // __INTRO_VIDEO_SCENE_H__
