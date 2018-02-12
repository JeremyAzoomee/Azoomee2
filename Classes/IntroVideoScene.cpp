#include "IntroVideoScene.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Strings.h>
#include "LoginLogicHandler.h"
#include "SceneManagerScene.h"
#include "SlideShowScene.h"
#include "StartScreen.h"

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
    Rect _visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();

    videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
    videoPlayer->setContentSize(_visibleRect.size);
    videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoPlayer->setPosition(Vec2(_visibleRect.origin.x + _visibleRect.size.width / 2,_visibleRect.origin.y + _visibleRect.size.height /2));
    videoPlayer->setFileName("res/introAssets/Opening_Animation.mp4");
    videoPlayer->setKeepAspectRatioEnabled(true);
    videoPlayer->addEventListener(CC_CALLBACK_2(IntroVideoScene::videoEventCallback, this));
    
    addChild(videoPlayer);
    
#ifndef novideo
    videoPlayer->play();
#endif
    
    return true;
}

void IntroVideoScene::onEnter()
{
    Node::onEnter();
#ifdef novideo
    navigateToNextScene();
#endif
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
    this->stopActionByTag(3);
    
    videoPlayer->setVisible(false);
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

