#include "OrientationChangeScene.h"
#include "OnboardingScene.h"
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include "OnboardingSuccessScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "OrientationFunctions_ios.h"
#endif

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
    else
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
