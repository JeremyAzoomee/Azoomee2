//ATTENTION! FRAMEWORK MODIFICATION REQUIRED IN ORDER TO HAVE THE VIDEO PLAYED WITHOUT CONTROL BAR!
//cocos2d/cocos/platform/android/java/src/org/cocos2dx/lib/Cocos2dxVideoView.java row 204-206 if(isPlaying()) to be commented
//cocos2d/cocos/ui/UIVideoPlayer-ios.mm - roww 144-145 - MPMovideControlStyleNone, interactionenabled: false

#include "IntroVideoScene.h"
#include "SlideShowScene.h"
#include "ConfigStorage.h"
#include "BaseScene.h"
#include "LoginScene.h"
#include "HQHistoryManager.h"
#include "MixPanelSingleton.h"
#include "StringMgr.h"

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
    
    auto test = StringMgr::getInstance()->getStringForKey("pause_layer_pause_title");
    auto test1 = StringMgr::getInstance()->getErrorMessageForKey("pause_layer_pause_title");
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Rect _visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();

    auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
    videoPlayer->setContentSize(_visibleRect.size);
    videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoPlayer->setPosition(Vec2(_visibleRect.origin.x + _visibleRect.size.width / 2,_visibleRect.origin.y + _visibleRect.size.height /2));
    videoPlayer->setFileName("res/introAssets/Opening Animation.mp4");
    videoPlayer->setKeepAspectRatioEnabled(true);
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
            MixPanelSingleton::getInstance()->mixPanel_registerAppVersion();
            
            if(ConfigStorage::getInstance()->shouldShowFirstSlideShowScene())
            {
                auto slideShowScene = SlideShowScene::createScene();
                Director::getInstance()->replaceScene(slideShowScene);
            }
            else
            {
#ifdef forgetuserdata
                UserDefault* def2 = UserDefault::getInstance();
                def2->setStringForKey("username", "");
                def2->setStringForKey("password", "");
                def2->flush();
#endif
                
                UserDefault* def = UserDefault::getInstance();
                std::string username = def->getStringForKey("username", "");
                std::string password = def->getStringForKey("password", "");
                def->flush();
                
                if((username == "")||(password == ""))
                {
                    CCLOG("autologin NOT called");
                    HQHistoryManager::getInstance()->emptyHistory();
                    auto baseScene = BaseScene::createScene();
                    Director::getInstance()->replaceScene(baseScene);
                }
                else
                {
                    CCLOG("autologin called");
                    auto loginScene = LoginScene::createSceneWithAutoLogin();
                    Director::getInstance()->replaceScene(loginScene);
                }
                
            }
            break;
        }
        default:
            break;
    }
}

