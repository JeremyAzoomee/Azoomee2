#include "WebViewNativeCaller_android.h"
#include <AzoomeeCommon/Data/Cookie/CookieDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "WebGameAPIDataManager.h"
#include "VideoPlaylistManager.h"
#include "FavouritesManager.h"
#include "ContentHistoryManager.h"
#include "RecentlyPlayedManager.h"
#include <AzoomeeCommon/Utils/SessionIdManager.h>
#include <AzoomeeCommon/Data/Cookie/CookieDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include <AzoomeeCommon/Strings.h>
#include "ChatDelegate.h"
#include "BackEndCaller.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

#endif

using namespace cocos2d;
using namespace Azoomee;

NS_AZOOMEE_BEGIN

cocos2d::Scene* WebViewNativeCaller_android::createSceneWithUrl(const std::string& url, Orientation orientation, Vec2 closeButtonAnchor, int videoProgressSeconds)
{
    auto scene = cocos2d::Scene::create();
    auto layer = WebViewNativeCaller_android::create();
    scene->addChild(layer);
    
    layer->loadUrl = url;
    layer->_orientation = orientation;
    layer->_closeButtonAnchor = closeButtonAnchor;
	layer->_videoProgressSeconds = videoProgressSeconds;
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
    cocos2d::log("sending to mixpanel: %s, %s", eventKey, eventValue);
    
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
        HQContentItemObjectRef contentItem = VideoPlaylistManager::getInstance()->getContentItemDataForPlaylistElement(std::atoi(strValue.c_str()));
        AnalyticsSingleton::getInstance()->updateContentItemDetails(contentItem);
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
    JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "startWebView", loadUrl,ChildDataProvider::getInstance()->getLoggedInChildId(),(int)_orientation, _closeButtonAnchor.x, _closeButtonAnchor.y, _videoProgressSeconds);
        
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
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIGetStringForKey(JNIEnv* env, jobject thiz, jstring key);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIGetStringForKey(JNIEnv* env, jobject thiz, jstring key)
{
    const char* cKey = env->GetStringUTFChars(key, NULL);
    
    jstring returnString = env->NewStringUTF(StringMgr::getInstance()->getStringForKey(cKey).c_str());
    return returnString;
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNISendAPIRequest(JNIEnv* env, jobject thiz, jstring method, jstring responseID, jstring sendData);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNISendAPIRequest(JNIEnv* env, jobject thiz, jstring method, jstring responseID, jstring sendData)
{
    const char* cMethod = env->GetStringUTFChars(method, NULL);
    const char* cResponseID = env->GetStringUTFChars(responseID, NULL);
    const char* cSendData = env->GetStringUTFChars(sendData, NULL);
    
    jstring returnString = env->NewStringUTF(WebGameAPIDataManager::getInstance()->handleAPIRequest(cMethod, cResponseID, cSendData));
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIGetRemoteWebGameAPIPath(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIGetRemoteWebGameAPIPath(JNIEnv* env, jobject thiz)
{
    jstring returnString = env->NewStringUTF(ConfigStorage::getInstance()->getRemoteWebGameAPIPath().c_str());
    return returnString;
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIAddToFavourites(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIAddToFavourites(JNIEnv* env, jobject thiz)
{
    FavouritesManager::getInstance()->addToFavourites(ContentHistoryManager::getInstance()->getLastOpenedContent());
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRemoveFromFavourites(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRemoveFromFavourites(JNIEnv* env, jobject thiz)
{
    FavouritesManager::getInstance()->removeFromFavourites(ContentHistoryManager::getInstance()->getLastOpenedContent());
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT bool JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIIsInFavourites(JNIEnv* env, jobject thiz);
};

JNIEXPORT bool JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIIsInFavourites(JNIEnv* env, jobject thiz)
{
    return FavouritesManager::getInstance()->isFavouriteContent(ContentHistoryManager::getInstance()->getLastOpenedContent()->getContentItemId());
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIShareInChat(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIShareInChat(JNIEnv* env, jobject thiz)
{
	if(!HQHistoryManager::getInstance()->isOffline)
	{
    	ChatDelegate::getInstance()->_sharedContentId = ContentHistoryManager::getInstance()->getLastOpenedContent()->getContentItemId();
	}
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT bool JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIIsChatEntitled(JNIEnv* env, jobject thiz);
};

JNIEXPORT bool JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIIsChatEntitled(JNIEnv* env, jobject thiz)
{
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kChatHQName)->getHqEntitlement() && !HQHistoryManager::getInstance()->isOffline;
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT bool JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIIsAnonUser(JNIEnv* env, jobject thiz);
};

JNIEXPORT bool JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIIsAnonUser(JNIEnv* env, jobject thiz)
{
    return ParentDataProvider::getInstance()->isLoggedInParentAnonymous();
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNISendVideoProgress(JNIEnv* env, jobject thiz, int playlistIndex, int progressSeconds);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNISendVideoProgress(JNIEnv* env, jobject thiz, int playlistIndex, int progressSeconds)
{
	 BackEndCaller::getInstance()->updateVideoProgress(VideoPlaylistManager::getInstance()->getContentItemDataForPlaylistElement(playlistIndex)->getContentItemId(), progressSeconds);
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNINewVideoOpened(JNIEnv* env, jobject thiz, int playlistIndex);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNINewVideoOpened(JNIEnv* env, jobject thiz, int playlistIndex)
{
	const auto& contentItem = VideoPlaylistManager::getInstance()->getContentItemDataForPlaylistElement(playlistIndex);
	RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), ConfigStorage::kVideoHQName);
	RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), ConfigStorage::kMeHQName);
	ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
}

#endif
