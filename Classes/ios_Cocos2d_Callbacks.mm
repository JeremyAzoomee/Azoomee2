#include "ios_Cocos2d_Callbacks.h"
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include "LoginScene.h"
#include "AnalyticsSingleton.h"

void navigateToBaseScene()
{
    HQHistoryManager::getInstance()->addHomeIfHistoryEmpty();
    
    AnalyticsSingleton::getInstance()->mixPanel_closeContentEvent();
    
    auto baseScene = BaseScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(baseScene);
}

void navigateToLoginScene()
{
    AnalyticsSingleton::getInstance()->mixPanel_closeContentEvent();
    
    auto loginScene = LoginScene::createSceneWithAutoLoginAndErrorDisplay();
    cocos2d::Director::getInstance()->replaceScene(loginScene);
}

void sendMixPanelData(const char* host, const char* query)
{
    CCLOG("host: %s, query: %s", host, query);
    std::string strHost = StringUtils::format("%s", host);
    std::string strQuery = StringUtils::format("%s", query);
    
    if(strHost == "play")
    {
        //No play event in mixpanel singleton, TBI
    }
    
    if(strHost == "pause")
    {
        AnalyticsSingleton::getInstance()->mixPanel_mediaPausedEvent();
    }
    
    if(strHost == "quality")
    {
        AnalyticsSingleton::getInstance()->mixPanel_mediaQuality(strQuery);
    }
    
    if(strHost == "time")
    {
        AnalyticsSingleton::getInstance()->mixPanel_mediaProgress(std::atoi(strQuery.c_str()));
    }
    
    if(strHost == "complete")
    {
        //Further implementation required - need to get played time.
    }
}
