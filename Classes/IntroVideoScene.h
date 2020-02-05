#ifndef __INTRO_VIDEO_SCENE_H__
#define __INTRO_VIDEO_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include "ForceUpdateSingleton.h"
#include "ui/UIVideoPlayer.h"

NS_AZOOMEE_BEGIN

class IntroVideoScene: public Azoomee::Scene, ForceUpdateDelegate
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
	void onExit() override;

    virtual bool init() override;

    CREATE_FUNC(IntroVideoScene);
	
	//Delegate functions
	void onForceUpdateCheckFinished(const ForceUpdateResult& result) override;
};

NS_AZOOMEE_END

#endif // __INTRO_VIDEO_SCENE_H__
