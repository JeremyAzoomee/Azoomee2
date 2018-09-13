#include "AppDelegate.h"
#include "BaseScene.h"
#include "IntroVideoScene.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQHistoryManager.h"
#include "OfflineHubScene.h"
#include "LoginLogicHandler.h"
#include "NativeContentInterface_ios.h"
#include <AzoomeeCommon/Utils/SessionIdManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/PushNotificationsHandler.h>
#include "FlowDataSingleton.h"
#include <AzoomeeCommon/ErrorCodes.h>
#include "ContentHistoryManager.h"
#include "IAPProductDataHandler.h"
#include "ChatDelegate.h"
#include "SceneManagerScene.h"

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
    
    // create a scene. it's an autorelease object
    Director::getInstance()->runWithScene(SceneManagerScene::createScene(introVideo));
    
    SessionIdManager::getInstance();
    AnalyticsSingleton::getInstance()->setLandscapeOrientation();
    AnalyticsSingleton::getInstance()->firstLaunchEvent();
    
    PushNotificationsHandler::getInstance()->resetExistingNotifications();
    
    IAPProductDataHandler::getInstance()->fetchProductData();
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    if(visibleSize.width / visibleSize.height > 1.95)
    {
        ConfigStorage::getInstance()->setIsDevice18x9(true);
    }

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
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
    
    PushNotificationsHandler::getInstance()->resetExistingNotifications();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    if(Director::getInstance()->getRunningScene()->getChildByName("androidWebView"))
    {
        AnalyticsSingleton::getInstance()->contentItemClosedEvent();
        
        if(HQHistoryManager::getInstance()->thereWasAnError)
        {
            HQHistoryManager::getInstance()->thereWasAnError = false;
            FlowDataSingleton::getInstance()->setErrorCode(ERROR_CODE_SOMETHING_WENT_WRONG);
            LoginLogicHandler::getInstance()->doLoginLogic();
            return;
        }
        
        if(HQHistoryManager::getInstance()->isOffline == true)
        {
            Director::getInstance()->replaceScene(OfflineHubScene::createScene());
            return;
        }
        if(HQHistoryManager::getInstance()->getCurrentHQ() != ConfigStorage::kHomeHQName && !(HQHistoryManager::getInstance()->getCurrentHQ() == ConfigStorage::kGroupHQName && HQHistoryManager::getInstance()->getPreviousHQ() == ConfigStorage::kHomeHQName))
        {
            ContentHistoryManager::getInstance()->setReturnedFromContent(true);
        }
        
        if(ChatDelegate::getInstance()->_sharedContentId != "")
        {
            ChatDelegate::getInstance()->shareContentInChat();
            return;
        }
        
        HQHistoryManager::getInstance()->addDefaultHQIfHistoryEmpty();
        
        cocos2d::Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
    }

#endif
}

