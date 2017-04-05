#include "ios_Cocos2d_Callbacks.h"
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include "LoginScene.h"
#include "AnalyticsSingleton.h"
#include "OfflineHubScene.h"
#include "BackEndCaller.h"
#include "WebGameAPIDataManager.h"

void navigateToBaseScene()
{
    if(HQHistoryManager::getInstance()->isOffline)
    {
        cocos2d::Director::getInstance()->replaceScene(OfflineHubScene::createScene());
        return;
    }
    
    HQHistoryManager::getInstance()->addHomeIfHistoryEmpty();
    
    AnalyticsSingleton::getInstance()->closeContentEvent();
    
    auto baseScene = BaseScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(baseScene);
}

void navigateToLoginScene()
{
    AnalyticsSingleton::getInstance()->closeContentEvent();
    
    BackEndCaller::getInstance()->getAvailableChildren();
}

void sendMixPanelData(const char* host, const char* query)
{
    CCLOG("host: %s, query: %s", host, query);
    std::string strHost = cocos2d::StringUtils::format("%s", host);
    std::string strQuery = cocos2d::StringUtils::format("%s", query);
    
    if(strHost == "video.play")
    {
        //No play event in mixpanel singleton, TBI
    }
    
    if(strHost == "video.pause")
    {
        AnalyticsSingleton::getInstance()->mediaPausedEvent();
    }
    
    if(strHost == "video.quality")
    {
        AnalyticsSingleton::getInstance()->mediaQualityEvent(strQuery);
    }
    
    if(strHost == "video.time")
    {
        AnalyticsSingleton::getInstance()->mediaProgressEvent(std::atoi(strQuery.c_str()));
    }
    
    if(strHost == "video.complete")
    {
        //Further implementation required - need to get played time.
    }
    if(strHost == "video.firstFrame")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerFirstFrameEvent(strQuery.c_str());
    }
}

const char* sendGameApiRequest(const char* method, const char* responseid, const char* score)
{
    char* returnString = WebGameAPIDataManager::getInstance()->handleAPIRequest(method, responseid, score);
    CCLOG("returnString in callback listener: %s", returnString);
    
    return returnString;
}

const char* getLocalStorageForGame()
{
    char* returnString = WebGameAPIDataManager::getInstance()->getLocalStorageData();
    return returnString;
}

void saveLocalStorageData(NSString* stringToSave)
{
    std::string stringToSaveStd = [stringToSave UTF8String];
    WebGameAPIDataManager::getInstance()->saveLocalStorageData(stringToSaveStd);
}
