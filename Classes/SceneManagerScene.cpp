#include "SceneManagerScene.h"
#include "OnboardingScene.h"
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include "OnboardingSuccessScene.h"
#include "ChildAccountScene.h"
#include "ChildSelectorScene.h"
#include "LoginScene.h"
#include "ChildAccountSuccessScene.h"
#include "OfflineHubScene.h"
#include "HQScene.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "FlowDataSingleton.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "OrientationFunctions_ios.h"
#endif

Scene* SceneManagerScene::createScene(SceneNameEnum sceneName)
{
    auto scene = Scene::create();
    auto layer = SceneManagerScene::create();
    
    layer->nextScene = sceneName;
    scene->addChild(layer);
    
    return scene;
}

bool SceneManagerScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void SceneManagerScene::onEnterTransitionDidFinish()
{
    Scene* goToScene;
    
    switch (nextScene) {
        case Login:
        {
            forceToLandscape();
            goToScene = LoginScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("LOGIN");
            break;
        }
        case Onboarding:
        {
            forceToPortrait();
            goToScene = OnboardingScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("ONBOARDING");
            break;
        }
        case Base:
        {
            FlowDataSingleton::getInstance()->clearData();
            forceToLandscape();
            HQHistoryManager::getInstance()->addHomeIfHistoryEmpty();
            goToScene = BaseScene::createScene();
            break;
        }
        case BaseWithNoHistory:
        {
            FlowDataSingleton::getInstance()->clearData();
            forceToLandscape();
            HQHistoryManager::getInstance()->emptyHistory();
            goToScene = BaseScene::createScene();
            break;
        }
        case ChildAccount:
        {
            forceToPortrait();
            goToScene = ChildAccountScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_ACCOUNT");
            break;
        }
        case ChildAccountSuccessScene:
        {
            forceToLandscape();
            goToScene = ChildAccountSuccessScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_ACCOUNT_SUCCESS");
            break;
        }
        case ChildSelector:
        {
            FlowDataSingleton::getInstance()->clearData();
            forceToLandscape();
            goToScene = ChildSelectorScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_SELECTOR");
            break;
        }
        case OnboardingSuccessScene:
        {
            forceToLandscape();
            goToScene = OnboardingSuccessScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("ONBOARDING_SUCCESS");
            break;
        }
        case OfflineHub:
        {
            forceToLandscape();
            OfflineChecker::getInstance()->setDelegate(nullptr);
            goToScene = OfflineHubScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("OFFLINE");
            break;
        }
        case OfflineArtsAppHQ:
        {
            forceToLandscape();
            goToScene = HQScene::createSceneForOfflineArtsAppHQ();
            AnalyticsSingleton::getInstance()->registerCurrentScene("OFFLINE_ARTS_APP");
            break;
        }
        default:
            break;
    }
    
    Director::getInstance()->replaceScene(goToScene);
}

void SceneManagerScene::forceToPortrait()
{
    AnalyticsSingleton::getInstance()->setPortraitOrientation();
    auto director = cocos2d::Director::getInstance();
    auto glView = director->getOpenGLView();
    auto frameSize = glView->getFrameSize();
    
    if((int) frameSize.width < (int) frameSize.height)
        cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) frameSize.width, (int) frameSize.height);
    else
        cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) frameSize.height, (int) frameSize.width);
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        setOrientationToPortrait();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        cocos2d::JniMethodInfo methodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "setOrientationPortrait", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    #endif
}

void SceneManagerScene::forceToLandscape()
{
    AnalyticsSingleton::getInstance()->setLandscapeOrientation();
    auto director = cocos2d::Director::getInstance();
    auto glView = director->getOpenGLView();
    auto frameSize = glView->getFrameSize();
    
    if((int) frameSize.width > (int) frameSize.height)
        cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) frameSize.width, (int) frameSize.height);
    else
        cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) frameSize.height, (int) frameSize.width);
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        setOrientationToLandscape();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        cocos2d::JniMethodInfo methodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "setOrientationLandscape", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    #endif
}
