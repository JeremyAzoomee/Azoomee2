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
#include <AzoomeeCommon/Analytics/analyticsSingleton.h>

USING_NS_CC;
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

    // create a scene. it's an autorelease object
    auto scene = IntroVideoScene::createScene();
    director->runWithScene(scene);
    
    RoutePaymentSingleton::getInstance();
    SessionIdManager::getInstance();
    AnalyticsSingleton::getInstance()->setLandscapeOrientation();

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
    Super::applicationDidEnterBackground();
    
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
        AnalyticsSingleton::getInstance()->closeContentEvent();
        
        if(HQHistoryManager::getInstance()->thereWasAnError)
        {
            HQHistoryManager::getInstance()->thereWasAnError = false;
            LoginLogicHandler::getInstance()->setErrorMessageCodeToDisplay(1006);
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
