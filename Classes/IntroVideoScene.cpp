//ATTENTION! FRAMEWORK MODIFICATION REQUIRED IN ORDER TO HAVE THE VIDEO PLAYED WITHOUT CONTROL BAR!
//cocos2d/cocos/platform/android/java/src/org/cocos2dx/lib/Cocos2dxVideoView.java row 204-206 if(isPlaying()) to be commented
//cocos2d/cocos/ui/UIVideoPlayer-ios.mm - roww 144-145 - MPMovideControlStyleNone, interactionenabled: false

#include "IntroVideoScene.h"
#include "LoginScene.h"

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
            //NEED KEYVALUE TO KNOW IF FIRST TIME USER
            bool isFirstTimeUser = false;
            bool isLoggedIn = false;
            
            if(isFirstTimeUser)
            {
                //WILL LOAD SLIDESHOW SCENE WHEN CREATED
            }
            else if(isLoggedIn)
            {
                //WILL GO TO HUB OR CHILD SELECTOR
            }
            else
            {
                //WILL CHANGE AND GO TO PREVIEW HUB WHEN CREATED
                auto loginScene = LoginScene::createScene(0);
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, loginScene, Color3B(0,0,0)));
            }
            break;
        }
        default:
            break;
    }
}

