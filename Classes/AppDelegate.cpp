#include "AppDelegate.h"
#include "IntroVideoScene.h"
#include "HQHistoryManager.h"
#include "LoginController.h"
#include "NativeContentInterface_ios.h"
#include <AzoomeeCommon/Utils/SessionIdManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "FlowDataSingleton.h"
#include "ErrorCodes.h"
#include <AzoomeeCommon/ContentDataManagers/ContentHistoryManager.h>
#include "IAPProductDataHandler.h"
#include "ChatDelegate.h"
#include "SceneManagerScene.h"
#include "OfflineScene.h"
#include "../artapp/Classes/AzoomeeArt/MainScene.h"
#include <AzoomeeCommon/ContentDataManagers/GameDataManager.h>
#include <AzoomeeCommon/Device.h>
#include "WebViewSelector.h"

using namespace cocos2d;
using namespace AZ;


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
    Director::getInstance()->runWithScene(SceneManagerScene::createScene(SceneNameEnum::introVideo));
    
    #ifdef AZOOMEE_ENVIRONMENT_CI
        const std::string& url = "https://media.azoomee.ninja/static/popups/languages/languages.zip";
    #else
        const std::string& url = "https://media.azoomee.com/static/popups/languages/languages.zip";
    #endif
    LocaleManager::getInstance()->downloadRemoteStringsFiles(url);
    
    SessionIdManager::getInstance();
    AnalyticsSingleton::getInstance()->setLandscapeOrientation();
    AnalyticsSingleton::getInstance()->firstLaunchEvent();
	
    IAPProductDataHandler::getInstance()->fetchProductData();
    
    GameDataManager::getInstance(); //for initialisation of bundled games list for IOS
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    if(visibleSize.width / visibleSize.height > 1.95)
    {
        TZ::Device::getInstance()->setIsDevice18x9(true);
    }
    
#ifdef AZOOMEE_ENVIRONMENT_CI
    cocos2d::log("AZOOMEE_ENVIRONMENT_CI");
#else
    cocos2d::log("AZOOMEE_ENVIRONMENT_LIVE");
#endif
#ifdef AZOOMEE_VODACOM_BUILD
    cocos2d::log("AZOOMEE_VODACOM_BUILD");
#endif
#ifdef AZOOMEE_AMAZON_BUILD
    cocos2d::log("AZOOMEE_AMAZON_BUILD");
#endif

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(Director::getInstance()->getRunningScene()->getChildByName(WebViewSelector::kIosWebviewName))
	{
		NativeContentInterface_ios *webview = dynamic_cast<NativeContentInterface_ios*>(Director::getInstance()->getRunningScene()->getChildByName(WebViewSelector::kIosWebviewName));
		if(webview)
		{
			webview->removeWebViewFromScreen();
		}
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
	if(Director::getInstance()->getRunningScene()->getChildByName(WebViewSelector::kIosWebviewName))
	{
		NativeContentInterface_ios *webview = dynamic_cast<NativeContentInterface_ios*>(Director::getInstance()->getRunningScene()->getChildByName(WebViewSelector::kIosWebviewName));
		if(webview)
		{
			webview->reAddWebViewToScreen();
		}
	}
#endif

    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    if(Director::getInstance()->getRunningScene()->getChildByName(WebViewSelector::kAndroidWebviewName))
    {
        AnalyticsSingleton::getInstance()->contentItemClosedEvent();
        if(HQHistoryManager::getInstance()->hasError())
        {
            HQHistoryManager::getInstance()->setHasError(false);
            FlowDataSingleton::getInstance()->setErrorCode(ERROR_CODE_SOMETHING_WENT_WRONG);
            LoginController::getInstance()->doLoginLogic();
            return;
        }
        
        if(HQHistoryManager::getInstance()->isOffline())
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
            return;
        }
        
        ContentHistoryManager::getInstance()->setReturnedFromContent(true);
		
        if(ChatDelegate::getInstance()->_sharedContentId != "")
        {
            ChatDelegate::getInstance()->shareContentInChat();
            return;
        }
        
        HQHistoryManager::getInstance()->addDefaultHQIfHistoryEmpty();
        
        cocos2d::Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
    }
	else
	{
		AZ::ArtApp::MainScene* artAppScene = dynamic_cast<TZ::ArtApp::MainScene*>(Director::getInstance()->getRunningScene()->getChildByName("ArtAppMainScene"));
		if(artAppScene)
		{
			artAppScene->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([artAppScene](){
				artAppScene->reloadRenderTextureObject();
			}),NULL));
		}
	}
	
#endif
}

