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

USING_NS_CC;
using namespace Azoomee;

static cocos2d::Size designResolutionLandscapeSize = cocos2d::Size(2732, 2048);
static cocos2d::Size designResolutionPortraitSize = cocos2d::Size(designResolutionLandscapeSize.height, designResolutionLandscapeSize.width);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("azoomee2", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("azoomee2");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the initial resolution
    director->setContentScaleFactor(1.0f);
    auto frameSize = glview->getFrameSize();
    applicationScreenSizeChanged(frameSize.width, frameSize.height);

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
void AppDelegate::applicationDidEnterBackground() {

    SessionIdManager::getInstance()->registerAppWentBackgroundEvent();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    if(Director::getInstance()->getRunningScene()->getChildByName("iosWebView"))
    {
        WebViewNative_ios *webview = (WebViewNative_ios*)Director::getInstance()->getRunningScene()->getChildByName("iosWebView");
        webview->removeWebViewFromScreen();
    }
    
#endif
    
    Director::getInstance()->stopAnimation();
    Director::getInstance()->pause();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    
    Director::getInstance()->stopAnimation();
    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();
    
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

void AppDelegate::applicationScreenSizeChanged(int newWidth, int newHeight)
{
    cocos2d::log( "AppDelegate::applicationScreenSizeChanged: %d, %d", newWidth, newHeight );
    
    // First tell cocos to use this new size for the GLView
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    glview->setFrameSize(newWidth, newHeight);
    
    
    // Use the correct design resolution
    
    // Landscape
    if(newWidth > newHeight)
    {
        glview->setDesignResolutionSize(designResolutionLandscapeSize.width, designResolutionLandscapeSize.height, ResolutionPolicy::NO_BORDER);
    }
    // Portrait
    else
    {
        glview->setDesignResolutionSize(designResolutionPortraitSize.width, designResolutionPortraitSize.height, ResolutionPolicy::NO_BORDER);
    }
    
    
    // Resize the running scene
    Scene* scene = director->getRunningScene();
    if(scene != nullptr)
    {
        // Landscape
        if(newWidth > newHeight)
        {
            scene->setContentSize(designResolutionLandscapeSize);
        }
        // Portrait
        else
        {
            scene->setContentSize(designResolutionPortraitSize);
        }
    }
}

