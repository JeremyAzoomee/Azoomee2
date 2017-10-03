#include "WebViewNativeCaller_android.h"
#include <AzoomeeCommon/Data/Cookie/CookieDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "WebGameAPIDataManager.h"
#include "VideoPlaylistManager.h"
#include <AzoomeeCommon/Utils/SessionIdManager.h>
#include <AzoomeeCommon/Data/Cookie/CookieDataProvider.h>

using namespace cocos2d;
using namespace Azoomee;

NS_AZOOMEE_BEGIN

cocos2d::Scene* WebViewNativeCaller_android::createSceneWithUrl(const std::string& url, Orientation orientation)
{
    auto scene = cocos2d::Scene::create();
    auto layer = WebViewNativeCaller_android::create();
    scene->addChild(layer);
    
    layer->loadUrl = url;
    layer->_orientation = orientation;

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
        AnalyticsSingleton::getInstance()->mediaPlayerVideoPlayEvent();
    }
    
    if(strKey == "pause")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerPausedEvent();
    }
    
    if(strKey == "quality")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerQualityEvent(strValue);
    }
    
    if(strKey == "time")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerProgressEvent(std::atoi(strValue.c_str()));
    }
    
    if(strKey == "complete")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerVideoCompletedEvent();
    }
    
    if(strKey == "firstFrame")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerFirstFrameEvent(strValue.c_str());
    }
    
    if(strKey == "playlistItem")
    {
        HQContentItemObject contentItem = VideoPlaylistManager::getInstance()->getContentItemDataForPlaylistElement(std::atoi(strValue.c_str()));
        AnalyticsSingleton::getInstance()->updateContentItemDetails(&contentItem);
        AnalyticsSingleton::getInstance()->mediaPlayerNewPlaylistItemSetEvent(std::atoi(strValue.c_str()));
    }
    
    if(strKey == "playlistComplete")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerPlaylistCompletedEvent();
    }
}


void WebViewNativeCaller_android::onEnterTransitionDidFinish()
{
    AudioMixer::getInstance()->stopBackgroundMusic();
    Director::getInstance()->purgeCachedData();
    this->setName("androidWebView");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SessionIdManager::getInstance()->registerAndroidSceneChangeEvent();
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "startWebView", "(Ljava/lang/String;Ljava/lang/String;I)V"))
    {
        return;
    }
    
    jstring jurl = methodInfo.env->NewStringUTF(loadUrl.c_str());
    jstring juserid = methodInfo.env->NewStringUTF(ChildDataProvider::getInstance()->getLoggedInChildId().c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl, juserid,_orientation);

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

NS_AZOOMEE_END


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_getBackToLoginScreen(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_getBackToLoginScreen(JNIEnv* env, jobject thiz)
{
    HQHistoryManager::getInstance()->thereWasAnError = true;
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_sendMediaPlayerData(JNIEnv* env, jobject thiz, jstring eventKey, jstring eventValue);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_sendMediaPlayerData(JNIEnv* env, jobject thiz, jstring eventKey, jstring eventValue)
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
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNISaveLocalDataStorage(JNIEnv* env, jobject thiz, jstring data);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNISaveLocalDataStorage(JNIEnv* env, jobject thiz, jstring data)
{
    std::string cData(env->GetStringUTFChars(data, NULL));
    WebGameAPIDataManager::getInstance()->saveLocalStorageData(cData);
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIGetLocalDataStorage(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIGetLocalDataStorage(JNIEnv* env, jobject thiz)
{
    jstring returnString = env->NewStringUTF(WebGameAPIDataManager::getInstance()->getLocalStorageData());
    return returnString;
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNISendAPIRequest(JNIEnv* env, jobject thiz, jstring method, jstring responseID, jstring score);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNISendAPIRequest(JNIEnv* env, jobject thiz, jstring method, jstring responseID, jstring score)
{
    const char* cMethod = env->GetStringUTFChars(method, NULL);
    const char* cResponseID = env->GetStringUTFChars(responseID, NULL);
    const char* cScore = env->GetStringUTFChars(score, NULL);
    
    jstring returnString = env->NewStringUTF(WebGameAPIDataManager::getInstance()->handleAPIRequest(cMethod, cResponseID, cScore));
    return returnString;
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIGetVideoPlaylist(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIGetVideoPlaylist(JNIEnv* env, jobject thiz)
{
    jstring returnString = env->NewStringUTF(VideoPlaylistManager::getInstance()->getPlaylist().c_str());
    return returnString;
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIGetAllCookies(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIGetAllCookies(JNIEnv* env, jobject thiz)
{
    jstring returnString = env->NewStringUTF(CookieDataProvider::getInstance()->getAllCookiesInJson().c_str());
    return returnString;
}

#endif
