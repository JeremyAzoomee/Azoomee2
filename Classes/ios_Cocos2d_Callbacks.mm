#include "ios_Cocos2d_Callbacks.h"
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include "LoginScene.h"
#include "MixPanelSingleton.h"

void navigateToBaseScene()
{
    HQHistoryManager::getInstance()->addHomeIfHistoryEmpty();
    
    MixPanelSingleton::getInstance()->mixPanel_closeContentEvent();
    
    auto baseScene = BaseScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(baseScene);
}

void navigateToLoginScene()
{
    MixPanelSingleton::getInstance()->mixPanel_closeContentEvent();
    
    auto loginScene = LoginScene::createSceneWithAutoLoginAndErrorDisplay();
    cocos2d::Director::getInstance()->replaceScene(loginScene);
}
