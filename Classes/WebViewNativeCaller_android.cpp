#include "WebViewNativeCaller_android.h"
#include "SimpleAudioEngine.h"
#include "CookieDataProvider.h"
#include "ChildDataProvider.h"

USING_NS_CC;

cocos2d::Scene* WebViewNativeCaller_android::createSceneWithUrl(std::string url)
{
    auto scene = cocos2d::Scene::create();
    auto layer = WebViewNativeCaller_android::create();
    scene->addChild(layer);
    
    loadUrl = url;

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


void WebViewNativeCaller_android::onEnterTransitionDidFinish()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
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
    
    CCLOG("Cookie to be sent to jni: %s", CookieDataProvider::getInstance()->getCookiesForRequest(url).c_str());
    CCLOG("CookieURL to be sent to jni: %s", getUrlWithoutPath(url).c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl, jcookieurl, jcookie, juserid);

    methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
#endif
}

// on "init" you need to initialize your instance
bool WebViewNativeCaller_android::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    
    return true;
}
