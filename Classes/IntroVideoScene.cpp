//ATTENTION! FRAMEWORK MODIFICATION REQUIRED IN ORDER TO HAVE THE VIDEO PLAYED WITHOUT CONTROL BAR!
//cocos2d/cocos/platform/android/java/src/org/cocos2dx/lib/Cocos2dxVideoView.java row 204-206 if(isPlaying()) to be commented
//cocos2d/cocos/ui/UIVideoPlayer-ios.mm - roww 144-145 - MPMovideControlStyleNone, interactionenabled: false

#include "IntroVideoScene.h"
#include "SlideShowScene.h"
#include "ConfigStorage.h"
#include "BaseScene.h"
#include "LoginScene.h"
#include "HQHistoryManager.h"
#include "AnalyticsSingleton.h"
#include "StringMgr.h"
#include "ChildAccountSuccessScene.h"
#include "LoginLogicHandler.h"

//ATTENTION! FRAMEWORK MODIFICATION REQUIRED IN ORDER TO HAVE THE VIDEO PLAYED WITHOUT CONTROL BAR!
//cocos2d/cocos/platform/android/java/src/org/cocos2dx/lib/Cocos2dxVideoView.java row 204-206 if(isPlaying()) to be commented out
//cocos2d/cocos/ui/UIVideoPlayer-ios.mm - roww 144-145 - MPMovideControlStyleNone, interactionenabled: false

Scene* IntroVideoScene::createScene()
{
    auto scene = Scene::create();
    auto layer = IntroVideoScene::create();
    scene->addChild(layer);

    return scene;
}

bool IntroVideoScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto funcCallAction = CallFunc::create([=](){
        
        videoErrorText = StringUtils::format("%svideo failsafe triggered.",videoErrorText.c_str());
        AnalyticsSingleton::getInstance()->introVideoTimedOutError(videoErrorText);
        navigateToNextScene();
    });
    
    funcCallAction->setTag(2);
    this->runAction(Sequence::create(DelayTime::create(7), funcCallAction, NULL));

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Rect _visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();

    auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
    videoPlayer->setContentSize(_visibleRect.size);
    videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoPlayer->setPosition(Vec2(_visibleRect.origin.x + _visibleRect.size.width / 2,_visibleRect.origin.y + _visibleRect.size.height /2));
    videoPlayer->setFileName("res/introAssets/Opening_Animation.mp4");
    videoPlayer->setKeepAspectRatioEnabled(true);
    videoPlayer->addEventListener(CC_CALLBACK_2(IntroVideoScene::videoEventCallback, this));
    
    addChild(videoPlayer);
    videoPlayer->play();
    
    return true;
}

void IntroVideoScene::videoEventCallback(Ref* sender, VideoPlayer::EventType eventType)
{
    switch (eventType) {
        case VideoPlayer::EventType::PAUSED:
            break;
        case VideoPlayer::EventType::PLAYING:
        {
            videoErrorText = "Video Started Playing and ";
        }
            break;
        case VideoPlayer::EventType::COMPLETED:
        {
            navigateToNextScene();
            break;
        }
        default:
        {
            videoErrorText = StringUtils::format("%svideo default event triggered.",videoErrorText.c_str());
            AnalyticsSingleton::getInstance()->introVideoTimedOutError(videoErrorText);
            navigateToNextScene();
            break;
        }
    }
}

void IntroVideoScene::navigateToNextScene()
{
    this->stopActionByTag(2);
    AnalyticsSingleton::getInstance()->registerAppVersion();
    
    if(ConfigStorage::getInstance()->shouldShowFirstSlideShowScene())
    {
        auto slideShowScene = SlideShowScene::createScene();
        Director::getInstance()->replaceScene(slideShowScene);
    }
    else
    {
        auto loginLogicHandler = new LoginLogicHandler();
        loginLogicHandler->doLoginLogic();
    }
}

