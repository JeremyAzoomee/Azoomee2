#include "SceneManagerScene.h"
#include "OnboardingScene.h"
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include "OnboardingSuccessScene.h"
#include "ChildAccountScene.h"
#include "ChildSelectorScene.h"
#include "LoginScene.h"
#include "ChildAccountSuccessScene.h"

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
            ForceToLandscape();
            goToScene = LoginScene::createScene();
            break;
        }
        case Onboarding:
        {
            ForceToPortrait();
            goToScene = OnboardingScene::createScene();
            break;
        }
        case Base:
        {
            ForceToLandscape();
            HQHistoryManager::getInstance()->emptyHistory();
            goToScene = BaseScene::createScene();
            break;
        }
        case ChildAccount:
        {
            ForceToPortrait();
            goToScene = ChildAccountScene::createScene("", 0);
            break;
        }
        case ChildAccountSuccessScene:
        {
            ForceToLandscape();
            goToScene = ChildAccountSuccessScene::createScene();
            break;
        }
        case ChildSelector:
        {
            ForceToLandscape();
            goToScene = ChildSelectorScene::createScene();
            break;
        }
        case OnboardingSuccessScene:
        {
            ForceToLandscape();
            goToScene = OnboardingSuccessScene::createScene();
            break;
        }
        default:
            break;
    }
    
    Director::getInstance()->replaceScene(goToScene);
}

void SceneManagerScene::ForceToPortrait()
{
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

void SceneManagerScene::ForceToLandscape()
{
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
