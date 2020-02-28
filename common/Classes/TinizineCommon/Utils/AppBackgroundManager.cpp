#include "AppBackgroundManager.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "../Device.h"
#include <chrono>

using namespace cocos2d;

NS_TZ_BEGIN

static std::auto_ptr<AppBackgroundManager> sAppBackgroundManagerSharedInstance;

AppBackgroundManager::AppBackgroundManager()
{
    _timeStampGoingBackground = 0;
    _timeStampAndroidSceneChange = 0;
    _backgroundTimeInContent = 0;
}

AppBackgroundManager* AppBackgroundManager::getInstance()
{
    if(!sAppBackgroundManagerSharedInstance.get())
    {
        sAppBackgroundManagerSharedInstance.reset(new AppBackgroundManager());
    }
    return sAppBackgroundManagerSharedInstance.get();
}

AppBackgroundManager::~AppBackgroundManager(void)
{
    
}

void AppBackgroundManager::registerAppWentBackgroundEvent()
{
    _timeStampGoingBackground = time(NULL);
    
    if(!eventHappenedDuringAndroidSceneChange())
    {
        AnalyticsSingleton::getInstance()->enteredBackgroundEvent();
    }
}

void AppBackgroundManager::registerAppCameForegroundEvent()
{
    if(!eventHappenedDuringAndroidSceneChange())
    {
        increaseBackgroundTimeInContent();
        AnalyticsSingleton::getInstance()->enteredForegroundEvent();
    }
}

void AppBackgroundManager::registerAndroidSceneChangeEvent()
{
    _timeStampAndroidSceneChange = time(NULL);
}

void AppBackgroundManager::resetBackgroundTimeInContent()
{
    _backgroundTimeInContent = 0;
}

long AppBackgroundManager::getBackgroundTimeInContent()
{
    return _backgroundTimeInContent;
}

//---------------------------------------PRIVATE METHODS--------------------------------------

bool AppBackgroundManager::eventHappenedDuringAndroidSceneChange()
{
    if(_timeStampAndroidSceneChange == 0)
    {
        return false;
    }
    
    if(time(NULL) - _timeStampAndroidSceneChange > 2)
    {
        return false;
    }
    
    return true;
}

void AppBackgroundManager::increaseBackgroundTimeInContent()
{
    if(_timeStampGoingBackground == 0)
    {
        return;
    }
    
    long timeSpentBackground = time(NULL) - _timeStampGoingBackground;
    
    if(timeSpentBackground > 2)
    {
        _backgroundTimeInContent += timeSpentBackground;
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAppWentBackgroundEvent(JNIEnv* env, jobject thiz);
    };

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAppWentBackgroundEvent(JNIEnv* env, jobject thiz)
    {
        AppBackgroundManager::getInstance()->registerAppWentBackgroundEvent();
    }

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAppCameForegroundEvent(JNIEnv* env, jobject thiz);
    };

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAppCameForegroundEvent(JNIEnv* env, jobject thiz)
    {
        AppBackgroundManager::getInstance()->registerAppCameForegroundEvent();
    }

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAndroidSceneChangeEvent(JNIEnv* env, jobject thiz);
    };

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAndroidSceneChangeEvent(JNIEnv* env, jobject thiz)
    {
        AppBackgroundManager::getInstance()->registerAndroidSceneChangeEvent();
    }

#endif


NS_TZ_END

