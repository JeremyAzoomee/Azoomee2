#include "ios_Cocos2d_Callbacks.h"
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include "MixPanelSingleton.h"
#include "OfflineHubScene.h"
#include "BackEndCaller.h"

void navigateToBaseScene()
{
    if(HQHistoryManager::getInstance()->isOffline)
    {
        cocos2d::Director::getInstance()->replaceScene(OfflineHubScene::createScene());
        return;
    }
    
    HQHistoryManager::getInstance()->addHomeIfHistoryEmpty();
    
    MixPanelSingleton::getInstance()->mixPanel_closeContentEvent();
    
    auto baseScene = BaseScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(baseScene);
}

void navigateToLoginScene()
{
    MixPanelSingleton::getInstance()->mixPanel_closeContentEvent();
    
    BackEndCaller::getInstance()->getAvailableChildren();
}

void sendMixPanelData(const char* host, const char* query)
{
    CCLOG("host: %s, query: %s", host, query);
    std::string strHost = cocos2d::StringUtils::format("%s", host);
    std::string strQuery = cocos2d::StringUtils::format("%s", query);
    
    if(strHost == "play")
    {
        //No play event in mixpanel singleton, TBI
    }
    
    if(strHost == "pause")
    {
        MixPanelSingleton::getInstance()->mixPanel_mediaPausedEvent();
    }
    
    if(strHost == "quality")
    {
        MixPanelSingleton::getInstance()->mixPanel_mediaQuality(strQuery);
    }
    
    if(strHost == "time")
    {
        MixPanelSingleton::getInstance()->mixPanel_mediaProgress(std::atoi(strQuery.c_str()));
    }
    
    if(strHost == "complete")
    {
        //Further implementation required - need to get played time.
    }
}
