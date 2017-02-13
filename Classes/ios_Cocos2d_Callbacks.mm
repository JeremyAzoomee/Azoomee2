#include "ios_Cocos2d_Callbacks.h"
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include "LoginScene.h"

void navigateToBaseScene()
{
    HQHistoryManager::getInstance()->addHomeIfHistoryEmpty();
    
    auto baseScene = BaseScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(baseScene);
}

void navigateToLoginScene()
{
    auto loginScene = LoginScene::createSceneWithAutoLoginAndErrorDisplay();
    cocos2d::Director::getInstance()->replaceScene(loginScene);
}
