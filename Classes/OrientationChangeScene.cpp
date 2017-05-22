#include "OrientationChangeScene.h"
#include "OnboardingScene.h"
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include "OnboardingSuccessScene.h"
#include "ChildAccountScene.h"
#include "ChildSelectorScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "OrientationFunctions_ios.h"
#endif

Scene* OrientationChangeScene::createScene(int toScene, long withErrorCode)
{
    auto scene = Scene::create();
    auto layer = OrientationChangeScene::create();
    
    layer->toScene = toScene;
    layer->errorCode = withErrorCode;
    
    scene->addChild(layer);
    
    return scene;
}

Scene* OrientationChangeScene::createSceneChildAccountNext(int toScene, long withErrorCode, std::string childName, int oomeeAvatarNo)
{
    auto scene = Scene::create();
    auto layer = OrientationChangeScene::create();
    
    layer->toScene = toScene;
    layer->errorCode = withErrorCode;
    layer->_childName = childName;
    layer->_oomeeAvatarNo = oomeeAvatarNo;
    
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
    
    Node::onEnter();
}

void OrientationChangeScene::onEnterTransitionDidFinish()
{
    switch (toScene) {
        case ONBOARDING_SCENE:
        {
            ForceToPortrait();
            auto onboardingScene = OnboardingScene::createScene(errorCode);
            Director::getInstance()->replaceScene(onboardingScene);
            break;
        }
        case BASE_SCENE:
        {
            ForceToLandscape();
            HQHistoryManager::getInstance()->emptyHistory();
            auto baseScene = BaseScene::createScene();
            Director::getInstance()->replaceScene(baseScene);
            break;
        }
        case ONBOARDING_SUCCESS_SCENE:
        {
            ForceToLandscape();
            auto onboardingSuccessScene = OnboardingSuccessScene::createScene(false);
            Director::getInstance()->replaceScene(onboardingSuccessScene);
            break;
        }
        case CHILD_ACCOUNT_SCENE_AFTERSIGNUP:
        {
            ForceToPortrait();
            auto newChildScene = ChildAccountScene::createScene("", 0, ONBOARDING_SUCCESS_SCENE);
            Director::getInstance()->replaceScene(newChildScene);
            break;
        }
        case CHILD_ACCOUNT_SCENE_CHILD_CREATION:
        {
            ForceToPortrait();
            auto newChildScene = ChildAccountScene::createScene("", 0,CHILD_ACCOUNT_SUCCESS_SCENE_CREATION);
            Director::getInstance()->replaceScene(newChildScene);
            break;
        }
        case CHILD_ACCOUNT_SCENE_CHILD_EDIT:
        {
            ForceToPortrait();
            auto newChildScene = ChildAccountScene::createScene("", 0,CHILD_ACCOUNT_SUCCESS_SCENE_EDITED);
            Director::getInstance()->replaceScene(newChildScene);
            break;
        }
        case CHILD_ACCOUNT_SUCCESS_SCENE_CREATION:
        {
            ForceToLandscape();
            //auto newChildScene = ChildAccountScene::createScene("", 0);
            //Director::getInstance()->replaceScene(newChildScene);
            break;
        }
        case CHILD_ACCOUNT_SUCCESS_SCENE_EDITED:
        {
            ForceToLandscape();
            //auto newChildScene = ChildAccountScene::createScene("", 0);
            //Director::getInstance()->replaceScene(newChildScene);
            break;
        }
        case CHILD_SELECTOR_SCENE:
        {
            ForceToLandscape();
            auto childSelectorScene = ChildSelectorScene::createScene();
            Director::getInstance()->replaceScene(childSelectorScene);
            break;
        }
        default:
            break;
    }
}

void OrientationChangeScene::ForceToPortrait()
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

void OrientationChangeScene::ForceToLandscape()
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
