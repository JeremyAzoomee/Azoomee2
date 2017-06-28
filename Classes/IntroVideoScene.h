#ifndef __INTRO_VIDEO_SCENE_H__
#define __INTRO_VIDEO_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "ui/UIVideoPlayer.h"

NS_AZOOMEE_BEGIN

class IntroVideoScene: public cocos2d::Layer
{
private:
    void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
    void navigateToNextScene();
    std::string videoErrorText;

    cocos2d::experimental::ui::VideoPlayer* videoPlayer;
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(IntroVideoScene);
};

NS_AZOOMEE_END

#endif // __INTRO_VIDEO_SCENE_H__
