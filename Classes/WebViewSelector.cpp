#include "WebViewSelector.h"
#include "CookieDataProvider.h"
#include "SimpleAudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "WebViewNative_ios.h"
#endif

USING_NS_CC;

cocos2d::Scene* WebViewSelector::createScene()
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = WebViewSelector::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

cocos2d::Scene* WebViewSelector::createSceneWithUrl(std::string url)
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = WebViewSelector::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    layer->loadWebView(url);
    
    // return the scene
    return scene;
}

int WebViewSelector::findPositionOfNthString(std::string string, std::string whatToFind, int whichOne)
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

std::string WebViewSelector::getUrlWithoutPath(std::string url)
{
    int until = findPositionOfNthString(url, "/", 3);
    
    return(url.substr(0, until)); //returning string before the 3rd slash.
}

void WebViewSelector::loadWebView(std::string url)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto iosWebView = WebViewNative_ios::createSceneWithURL(url);
    Director::getInstance()->replaceScene(iosWebView);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "startWebView", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jurl = methodInfo.env->NewStringUTF(url.c_str());
    jstring jcookieurl = methodInfo.env->NewStringUTF(getUrlWithoutPath(url).c_str());
    jstring jcookie = methodInfo.env->NewStringUTF(CookieDataProvider::getInstance()->getCookiesForRequest(url).c_str());
    
    CCLOG("Cookie to be sent to jni: %s", CookieDataProvider::getInstance()->getCookiesForRequest(url).c_str());
    CCLOG("CookieURL to be sent to jni: %s", getUrlWithoutPath(url).c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl, jcookieurl, jcookie);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
#endif
}

// on "init" you need to initialize your instance
bool WebViewSelector::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    
    return true;
}
