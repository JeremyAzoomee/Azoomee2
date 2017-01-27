//ATTENTION! FRAMEWORK MODIFICATION REQUIRED IN ORDER TO HAVE THE VIDEO PLAYED WITHOUT CONTROL BAR!
//cocos2d/cocos/platform/android/java/src/org/cocos2dx/lib/Cocos2dxVideoView.java row 204-206 if(isPlaying()) to be commented
//cocos2d/cocos/ui/UIVideoPlayer-ios.mm - roww 144-145 - MPMovideControlStyleNone, interactionenabled: false

#include "IntroVideoScene.h"
#include "SlideShowScene.h"
#include "ConfigStorage.h"
#include "BaseScene.h"

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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Rect _visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();

    auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
    videoPlayer->setContentSize(visibleSize);
    videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoPlayer->setPosition(Vec2(_visibleRect.origin.x + _visibleRect.size.width / 2,_visibleRect.origin.y + _visibleRect.size.height /2));
    videoPlayer->setFileName("res/introAssets/Opening Animation.mp4");

    videoPlayer->addEventListener(CC_CALLBACK_2(IntroVideoScene::videoEventCallback, this));
    
    addChild(videoPlayer);
    videoPlayer->play();
    
    return true;
}

void IntroVideoScene::videoEventCallback(Ref* sender, VideoPlayer::EventType eventType)
{
    switch (eventType) {
        case VideoPlayer::EventType::COMPLETED:
        {
            if(ConfigStorage::getInstance()->shouldShowFirstSlideShowScene())
            {
                auto slideShowScene = SlideShowScene::createScene();
                Director::getInstance()->replaceScene(slideShowScene);
            }
            else
            {
                auto baseScene = BaseScene::createScene();
                Director::getInstance()->replaceScene(baseScene);
            }
            break;
        }
        default:
            break;
    }
}

