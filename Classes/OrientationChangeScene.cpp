#include "OrientationChangeScene.h"
#include "OrientationFunctions.h"
#include "OnboardingScene.h"
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include "OnboardingSuccessScene.h"

Scene* OrientationChangeScene::createScene(bool forcePortrait,int toScene, long withErrorCode)
{
    auto scene = Scene::create();
    auto layer = OrientationChangeScene::create();
    
    layer->forcePortrait = forcePortrait;
    layer->toScene = toScene;
    layer->errorCode = withErrorCode;
    
    scene->addChild(layer);
    
    return scene;
}

bool OrientationChangeScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void OrientationChangeScene::onEnter()
{
    if(forcePortrait)
        setOrientationToPortrait();
    else
        setOrientationToLandscape();
    
    Node::onEnter();
}

void OrientationChangeScene::onEnterTransitionDidFinish()
{
    switch (toScene) {
        case ONBOARDING_SCENE:
        {
            auto onboardingScene = OnboardingScene::createScene(errorCode);
            Director::getInstance()->replaceScene(onboardingScene);
            break;
        }
        case BASE_SCENE:
        {
            HQHistoryManager::getInstance()->emptyHistory();
            auto baseScene = BaseScene::createScene();
            Director::getInstance()->replaceScene(baseScene);
            break;
        }
        case ONBOARDING_SUCCESS_SCENE:
        {
            auto onboardingSuccessScene = OnboardingSuccessScene::createScene(false);
            Director::getInstance()->replaceScene(onboardingSuccessScene);
            break;
        }
        default:
            break;
    }
}
