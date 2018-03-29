#include "IntroVideoScene.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Strings.h>
#include "LoginLogicHandler.h"
#include "SceneManagerScene.h"
#include "StartScreen.h"

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
    
    cocos2d::log("Cache folder: %s", FileUtils::getInstance()->getDocumentsPath().c_str());
    
    AnalyticsSingleton::getInstance()->registerCurrentScene("INTRO_VIDEO");
    
    auto funcCallAction = CallFunc::create([=](){
        
        videoErrorText = StringUtils::format("%svideo failsafe triggered.",videoErrorText.c_str());
        AnalyticsSingleton::getInstance()->introVideoTimedOutError(videoErrorText);
        navigateToNextScene();
    });
    
    auto action = Sequence::create(DelayTime::create(7), funcCallAction, NULL);
    
    action->setTag(3);
    this->runAction(action);

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    cocos2d::Rect _visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();

    _videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
    _videoPlayer->setContentSize(_visibleRect.size);
    _videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _videoPlayer->setPosition(Vec2(_visibleRect.origin.x + _visibleRect.size.width / 2,_visibleRect.origin.y + _visibleRect.size.height /2));
    _videoPlayer->setSwallowTouches(false);
    _videoPlayer->setFileName("res/introAssets/Opening_Animation.mp4");
    _videoPlayer->setKeepAspectRatioEnabled(true);
    _videoPlayer->addEventListener(CC_CALLBACK_2(IntroVideoScene::videoEventCallback, this));
    
    addChild(_videoPlayer);
    
#ifndef novideo
    _videoPlayer->play();
#endif

    return true;
}

void IntroVideoScene::onEnter()
{
    Node::onEnter();
#ifdef novideo
    navigateToNextScene();
#endif
    
    UserDefault* userDefault = UserDefault::getInstance();
    
    if(!userDefault->getBoolForKey("canSkipVideo"))
    {
        userDefault->setBoolForKey("canSkipVideo", true);
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
    this->stopActionByTag(3);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IosNativeFunctionsSingleton::getInstance()->identifyMixpanel();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "identifyMixpanel");
#endif
    
    _videoPlayer->stop();
    _videoPlayer->setVisible(false);
    AnalyticsSingleton::getInstance()->registerAppVersion();
    
    if(ConfigStorage::getInstance()->shouldShowFirstSlideShowScene())
    {
        AnalyticsSingleton::getInstance()->registerCurrentScene("INTRO_SLIDESHOW");
        Director::getInstance()->replaceScene(StartScreen::createScene());
    }
    else
    {
        LoginLogicHandler::getInstance()->doLoginLogic();
    }
}

NS_AZOOMEE_END

