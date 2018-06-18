#ifndef __INTRO_VIDEO_SCENE_H__
#define __INTRO_VIDEO_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include "ui/UIVideoPlayer.h"

NS_AZOOMEE_BEGIN

class IntroVideoScene: public Azoomee::Scene
{
    typedef Azoomee::Scene Super;
private:
    static const char* const kSkipVideoKeyInUserDefault;
    
    void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
    void navigateToNextScene();
    std::string videoErrorText;

    cocos2d::experimental::ui::VideoPlayer* _videoPlayer;
    
protected:
    virtual void onSizeChanged() override;
    
public:
    void onEnter() override;

    virtual bool init() override;

    CREATE_FUNC(IntroVideoScene);
};

NS_AZOOMEE_END

#endif // __INTRO_VIDEO_SCENE_H__
