#include "WebViewNativeCaller_android.h"
#include <AzoomeeCommon/Data/Cookie/CookieDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "WebGameAPIDataManager.h"

USING_NS_CC;
using namespace Azoomee;


cocos2d::Scene* WebViewNativeCaller_android::createSceneWithUrl(std::string url)
{
    auto scene = cocos2d::Scene::create();
    auto layer = WebViewNativeCaller_android::create();
    scene->addChild(layer);
    
    layer->loadUrl = url;

    return scene;
}

std::string WebViewNativeCaller_android::getUrlWithoutPath(std::string url)
{
    int until = findPositionOfNthString(url, "/", 3);
    
    return(url.substr(0, until)); //returning string before the 3rd slash.
}

int WebViewNativeCaller_android::findPositionOfNthString(std::string string, std::string whatToFind, int whichOne)
{
    int startSearchPos = 0;
    
    for(int i = 0; i < whichOne; i++)
    {
        if(string.find(whatToFind, startSearchPos) == string.npos)
        {
            return int(string.length());
        }
        else
        {
            startSearchPos = int(string.find(whatToFind, startSearchPos) + 1);
        }
    }
    
    return startSearchPos - 1;
}

void sendEventToMixPanel(const char* eventKey, const char* eventValue)
{
    CCLOG("sending to mixpanel: %s, %s", eventKey, eventValue);
    
    std::string strKey = StringUtils::format("%s", eventKey);
    std::string strValue = StringUtils::format("%s", eventValue);
    
    if(strKey == "play")
    {
        //No play event in mixpanel singleton, TBI
    }
    
    if(strKey == "pause")
    {
        AnalyticsSingleton::getInstance()->mediaPausedEvent();
    }
    
    if(strKey == "quality")
    {
        AnalyticsSingleton::getInstance()->mediaQualityEvent(strValue);
    }
    
    if(strKey == "time")
    {
        AnalyticsSingleton::getInstance()->mediaProgressEvent(std::atoi(strValue.c_str()));
    }
    
    if(strKey == "complete")
    {
        //Further implementation required - need to get played time.
    }
    
    if(strKey == "firstFrame")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerFirstFrameEvent(strValue.c_str());
    }
}


void WebViewNativeCaller_android::onEnterTransitionDidFinish()
{
    AudioMixer::getInstance()->stopBackgroundMusic();
    Director::getInstance()->purgeCachedData();
    this->setName("androidWebView");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "startWebView", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jurl = methodInfo.env->NewStringUTF(loadUrl.c_str());
    jstring jcookieurl = methodInfo.env->NewStringUTF(getUrlWithoutPath(loadUrl).c_str());
    jstring jcookie = methodInfo.env->NewStringUTF(CookieDataProvider::getInstance()->getCookiesForRequest(loadUrl).c_str());
    jstring juserid = methodInfo.env->NewStringUTF(ChildDataProvider::getInstance()->getLoggedInChildId().c_str());
    
    CCLOG("Cookie to be sent to jni: %s", CookieDataProvider::getInstance()->getCookiesForRequest(loadUrl).c_str());
    CCLOG("CookieURL to be sent to jni: %s", getUrlWithoutPath(loadUrl).c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl, jcookieurl, jcookie, juserid);

    methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
#endif
}

bool WebViewNativeCaller_android::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    
    return true;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_NativeView_getBackToLoginScreen(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_NativeView_getBackToLoginScreen(JNIEnv* env, jobject thiz)
{
    HQHistoryManager::getInstance()->thereWasAnError = true;
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_NativeView_sendMediaPlayerData(JNIEnv* env, jobject thiz, jstring eventKey, jstring eventValue);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_NativeView_sendMediaPlayerData(JNIEnv* env, jobject thiz, jstring eventKey, jstring eventValue)
{
    const char* cEventKey = env->GetStringUTFChars(eventKey, NULL);
    const char* cEventValue = env->GetStringUTFChars(eventValue, NULL);
    
    sendEventToMixPanel(cEventKey, cEventValue);
}

#endif

//-----------------------------------------------------Add webapi functions----------------------------------------------------------

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JsInterface_JNISaveLocalDataStorage(JNIEnv* env, jobject thiz, jstring data);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JsInterface_JNISaveLocalDataStorage(JNIEnv* env, jobject thiz, jstring data)
{
    std::string cData(env->GetStringUTFChars(data, NULL));
    WebGameAPIDataManager::getInstance()->saveLocalStorageData(cData);
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JsInterface_JNIGetLocalDataStorage(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JsInterface_JNIGetLocalDataStorage(JNIEnv* env, jobject thiz)
{
    jstring returnString = env->NewStringUTF(WebGameAPIDataManager::getInstance()->getLocalStorageData());
    return returnString;
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JsInterface_JNISendAPIRequest(JNIEnv* env, jobject thiz, jstring method, jstring responseID, jstring score);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JsInterface_JNISendAPIRequest(JNIEnv* env, jobject thiz, jstring method, jstring responseID, jstring score)
{
    const char* cMethod = env->GetStringUTFChars(method, NULL);
    const char* cResponseID = env->GetStringUTFChars(responseID, NULL);
    const char* cScore = env->GetStringUTFChars(score, NULL);
    
    jstring returnString = env->NewStringUTF(WebGameAPIDataManager::getInstance()->handleAPIRequest(cMethod, cResponseID, cScore));
    return returnString;
}

#endif
