#include "IntroVideoScene.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include "LoginController.h"
#include "SceneManagerScene.h"
#include "BackEndCaller.h"
#include "ForceUpdateAppLockScene.h"
#include "LanguageSelectScene.h"
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include "PopupMessageBox.h"
#include <AzoomeeCommon/Device.h>

using namespace cocos2d;
using namespace cocos2d::experimental::ui;

//#define novideo

USING_NS_TZ

NS_AZ_BEGIN

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
    
    auto addVideoPlayer = CallFunc::create([=]()
    {
        const Size& visibleSize = Director::getInstance()->getVisibleSize();
        
        const std::string& videoFilename = visibleSize.width > visibleSize.height ? "res/introAssets/Opening_Animation.mp4" : "res/introAssets/azoomee_intro_portrait.mp4";
        
        _videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
        _videoPlayer->setContentSize(visibleSize);
        _videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _videoPlayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height /2));
        _videoPlayer->setSwallowTouches(false);
        _videoPlayer->setFileName(videoFilename);
        _videoPlayer->setKeepAspectRatioEnabled(true);
        
        // Hide controls and disable interaction
        _videoPlayer->setUserInputEnabled(false);
        _videoPlayer->setStyle(cocos2d::experimental::ui::VideoPlayer::StyleType::NONE);
        
        addChild(_videoPlayer);
        _videoPlayer->play();
    });
        
    auto addListenerAction = CallFunc::create([=]()
    {
        _videoPlayer->addEventListener(CC_CALLBACK_2(IntroVideoScene::videoEventCallback, this));
    });
    
    auto delayListenerAction = Sequence::create(DelayTime::create(1.0f),addVideoPlayer, DelayTime::create(1.0f), addListenerAction, NULL); //video player from iOS 11.3 sends a playback complete event on initialisation, so we set up the listener a bit later.
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

void IntroVideoScene::onExit()
{
	ForceUpdateSingleton::getInstance()->setDelegate(nullptr);
	Super::onExit();
}

void IntroVideoScene::videoEventCallback(Ref* sender, VideoPlayer::EventType eventType)
{
    switch (eventType) {
        case VideoPlayer::EventType::PAUSED:
        {
            cocos2d::log("VIDEO: VIDEO PAUSED");
            break;
        }
        case VideoPlayer::EventType::STOPPED:
        {
            cocos2d::log("VIDEO: VIDEO STOPPED");
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
    
    TZ::Device::getInstance()->identifyMixpanel();
    
    if(_videoPlayer)
    {
        _videoPlayer->stop();
        _videoPlayer->setVisible(false);
    }
    AnalyticsSingleton::getInstance()->registerAppVersion();
	
	ForceUpdateSingleton::getInstance()->setDelegate(this);
	ForceUpdateSingleton::getInstance()->doForceUpdateLogic();
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

void IntroVideoScene::onForceUpdateCheckFinished(const ForceUpdateResult& result)
{
	switch (result)
	{
		case ForceUpdateResult::DO_NOTHING:
		{
			if(!UserAccountManager::getInstance()->userHasLoggedInOnDevice())
			{
				if(UserDefault::getInstance()->getStringForKey(UserAccountManager::kAnonEmailKey, "") == "")
				{
					Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::WelcomeScene));
				}
				else
				{
                    LoginController::getInstance()->anonLogin();
				}
			}
			else
			{
				LoginController::getInstance()->doLoginLogic();
			}
			
			break;
		}
		case ForceUpdateResult::NOTIFY:
		{
            PopupMessageBox* messageBox = PopupMessageBox::create();
            messageBox->setTitle(_("Update recommended"));
            messageBox->setBody(_("You should update to the latest version of Azoomee. Ask a grown-up to help you."));
            messageBox->setButtonText(_("OK"));
            messageBox->setButtonColour(Colours::Color_3B::darkIndigo);
            messageBox->setButtonPressedCallback([this](MessagePopupBase* pSender){
                pSender->removeFromParent();
                this->onForceUpdateCheckFinished(ForceUpdateResult::DO_NOTHING);
            });
            messageBox->setSecondButtonText(_("Update"));
            messageBox->setSecondButtonColour(Colours::Color_3B::strongPink);
            messageBox->setSecondButtonPressedCallback([](MessagePopupBase* pSender){
                pSender->removeFromParent();
                Director::getInstance()->replaceScene(ForceUpdateAppLockScene::create());
            });
            addChild(messageBox, 1);
			break;
		}
		case ForceUpdateResult::LOCK:
		{
			Director::getInstance()->replaceScene(ForceUpdateAppLockScene::create());
		}
		default:
			break;
	}
}

NS_AZ_END

