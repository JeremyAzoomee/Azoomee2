#include "WebViewSelector.h"
#include "DataStorage.h"

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

void WebViewSelector::loadWebView()
{
    //std::string url = "https://localhost/reqtest/index.html";
    std::string url = "https://media.azoomee.ninja/free/f50a74dd-185f-4010-ab6f-b34858b96bcd/video_stream.m3u8";
    //CCLOG("To be sent to jni: %s", DataStorage::getInstance()->dataDownloadCookiesWithCommas.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto iosWebView = WebViewNative_ios::createSceneWithURL(url);
    Director::getInstance()->replaceScene(iosWebView);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "alertJNI", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jurl = methodInfo.env->NewStringUTF(url.c_str());
    jstring jcookie = methodInfo.env->NewStringUTF(DataStorage::getInstance()->getCookiesForRequest(url).c_str());
    
    CCLOG("To be sent to jni: %s", DataStorage::getInstance()->getCookiesForRequest(url).c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl, jcookie);
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
