#include "IntroVideoScene.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Strings.h>
#include "LoginLogicHandler.h"
#include "SceneManagerScene.h"
#include "StartScreen.h"
#include "BackEndCaller.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include <AzoomeeCommon/Utils/IosNativeFunctionsSingleton.h>
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "platform/android/jni/JniHelper.h"
    static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";
#endif

using namespace cocos2d;
using namespace cocos2d::experimental::ui;

//#define novideo

NS_AZOOMEE_BEGIN

//ATTENTION! FRAMEWORK MODIFICATION REQUIRED IN ORDER TO HAVE THE VIDEO PLAYED WITHOUT CONTROL BAR!
//cocos2d/cocos/platform/android/java/src/org/cocos2dx/lib/Cocos2dxVideoView.java row 204-206 if(isPlaying()) to be commented out
//cocos2d/cocos/ui/UIVideoPlayer-ios.mm - roww 144-145 - MPMovideControlStyleNone, interactionenabled: false

const char* const IntroVideoScene::kSkipVideoKeyInUserDefault = "canSkipVideo";

bool IntroVideoScene::init()
{
    if ( !Super::init() )
    {
        return false;
    }
    
    cocos2d::log("Cache folder: %s", FileUtils::getInstance()->getDocumentsPath().c_str());
    
    AnalyticsSingleton::getInstance()->registerCurrentScene("INTRO_VIDEO");
    
    auto funcCallAction = CallFunc::create([=]()
    {
        
        videoErrorText = StringUtils::format("%svideo failsafe triggered.",videoErrorText.c_str());
        AnalyticsSingleton::getInstance()->introVideoTimedOutError(videoErrorText);
        navigateToNextScene();
    });
    
    auto action = Sequence::create(DelayTime::create(8.0f), funcCallAction, NULL);
    
    action->setTag(3);
    this->runAction(action);
    
#ifndef novideo
    
    auto addListenerAction = CallFunc::create([=]()
    {
        const Size& visibleSize = Director::getInstance()->getVisibleSize();
        
        const std::string& videoFilename = visibleSize.width > visibleSize.height ? "res/introAssets/Opening_Animation.mp4" : "res/introAssets/azoomee_intro_portrait.mp4";
        
        cocos2d::log("screen size: %f,%f", visibleSize.width, visibleSize.height);
        
        _videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
        _videoPlayer->setContentSize(visibleSize);
        _videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _videoPlayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height /2));
        _videoPlayer->setSwallowTouches(false);
        _videoPlayer->setFileName(videoFilename);
        _videoPlayer->setKeepAspectRatioEnabled(true);
        
        addChild(_videoPlayer);
        _videoPlayer->play();
        
        _videoPlayer->addEventListener(CC_CALLBACK_2(IntroVideoScene::videoEventCallback, this));
    });
    
    auto delayListenerAction = Sequence::create(DelayTime::create(1.0f), addListenerAction, NULL); //video player from iOS 11.3 sends a playback complete event on initialisation, so we set up the listener a bit later.
    delayListenerAction->setTag(4);
    this->runAction(delayListenerAction);
#endif

    return true;
}

void IntroVideoScene::onEnter()
{
    Super::onEnter();
    BackEndCaller::getInstance()->ipCheck();
    
#ifdef novideo
    navigateToNextScene();
#endif
    
    UserDefault* userDefault = UserDefault::getInstance();
    
    if(!userDefault->getBoolForKey(kSkipVideoKeyInUserDefault))
    {
        userDefault->setBoolForKey(kSkipVideoKeyInUserDefault, true);
        userDefault->flush();
    }
    else
    {
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch *touch, Event *event)
        {
            this->navigateToNextScene();
            return true;
        };
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
}

void IntroVideoScene::videoEventCallback(Ref* sender, VideoPlayer::EventType eventType)
{
    switch (eventType) {
        case VideoPlayer::EventType::PAUSED:
        {
            cocos2d::log("VIDEO: VIDEO PAUSED");
            break;
        }
        case VideoPlayer::EventType::PLAYING:
        {
            videoErrorText = "Video Started Playing and ";
            break;
        }
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
    this->stopActionByTag(3);
    this->stopActionByTag(4);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IosNativeFunctionsSingleton::getInstance()->identifyMixpanel();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "identifyMixpanel");
#endif
    
    if(_videoPlayer)
    {
        _videoPlayer->stop();
        _videoPlayer->setVisible(false);
    }
    AnalyticsSingleton::getInstance()->registerAppVersion();
    
    LoginLogicHandler::getInstance()->doLoginLogic();
    /*
    if(ConfigStorage::getInstance()->shouldShowFirstSlideShowScene())
    {
        AnalyticsSingleton::getInstance()->registerCurrentScene("INTRO_SLIDESHOW");
        Director::getInstance()->replaceScene(StartScreen::create());
    }
    else
    {
        LoginLogicHandler::getInstance()->doLoginLogic();
    }
     */
}

void IntroVideoScene::onSizeChanged()
{
    if(_videoPlayer)
    {
        const cocos2d::Rect& visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();
        _videoPlayer->setContentSize(visibleRect.size);
        _videoPlayer->setPosition(Vec2(visibleRect.size.width / 2, visibleRect.size.height /2));
    }
}

NS_AZOOMEE_END

