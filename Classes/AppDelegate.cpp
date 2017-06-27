#include "AppDelegate.h"
#include "BaseScene.h"
#include "IntroVideoScene.h"
#include "HQScene.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQHistoryManager.h"
#include "OfflineHubScene.h"
#include "LoginLogicHandler.h"
#include "RoutePaymentSingleton.h"
#include "WebViewNative_ios.h"
#include <AzoomeeCommon/Utils/SessionIdManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "FlowDataSingleton.h"

using namespace cocos2d;
using namespace Azoomee;


// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    Super::applicationDidFinishLaunching();
    
    register_all_packages();
    
    // Initialise Pusher
    PusherSDK::initialise("09995b8ae8cc75b36c25");

    // create a scene. it's an autorelease object
    auto scene = IntroVideoScene::createScene();
    Director::getInstance()->runWithScene(scene);
    
    RoutePaymentSingleton::getInstance();
    SessionIdManager::getInstance();
    AnalyticsSingleton::getInstance()->setLandscapeOrientation();
    AnalyticsSingleton::getInstance()->firstLaunchEvent();

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(Director::getInstance()->getRunningScene()->getChildByName("iosWebView"))
    {
        WebViewNative_ios *webview = (WebViewNative_ios*)Director::getInstance()->getRunningScene()->getChildByName("iosWebView");
        webview->removeWebViewFromScreen();
    }
    
#endif
    
    SessionIdManager::getInstance()->registerAppWentBackgroundEvent();
    AnalyticsSingleton::getInstance()->enteredBackgroundEvent();
    
    Super::applicationDidEnterBackground();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Super::applicationWillEnterForeground();
    
    AnalyticsSingleton::getInstance()->enteredForegroundEvent();
    SessionIdManager::getInstance()->registerAppCameForegroundEvent();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(Director::getInstance()->getRunningScene()->getChildByName("iosWebView"))
    {
        WebViewNative_ios *webview = (WebViewNative_ios*)Director::getInstance()->getRunningScene()->getChildByName("iosWebView");
        webview->reAddWebViewToScreen();
    }
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    if(Director::getInstance()->getRunningScene()->getChildByName("androidWebView"))
    {
        AnalyticsSingleton::getInstance()->contentItemClosedEvent();
        
        if(HQHistoryManager::getInstance()->thereWasAnError)
        {
            HQHistoryManager::getInstance()->thereWasAnError = false;
            FlowDataSingleton::getInstance()->setErrorCode(1006);
            LoginLogicHandler::getInstance()->doLoginLogic();
            return;
        }
        
        if(HQHistoryManager::getInstance()->isOffline == true)
        {
            Director::getInstance()->replaceScene(OfflineHubScene::createScene());
            return;
        }
        
        HQHistoryManager::getInstance()->addHomeIfHistoryEmpty();
        
        auto baseScene = BaseScene::createScene();
        cocos2d::Director::getInstance()->replaceScene(baseScene);
    }

#endif
}

