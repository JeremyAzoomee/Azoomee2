#include "ios_Cocos2d_Callbacks.h"
#include "BaseScene.h"

void navigateToBaseScene()
{
    auto baseScene = BaseScene::createScene();
    cocos2d::Director::getInstance()->replaceScene(baseScene);
}
