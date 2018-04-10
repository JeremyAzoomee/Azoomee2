#include "WebViewSelector.h"
#include <AzoomeeCommon/Data/Cookie/CookieDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "WebViewNative_ios.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "WebViewNativeCaller_android.h"
#endif

using namespace cocos2d;

NS_AZOOMEE_BEGIN

cocos2d::Scene* WebViewSelector::createSceneWithUrl(const std::string& url, Orientation orientation, const Vec2& closeButtonAnchor)
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = WebViewSelector::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    layer->loadWebView(url, orientation, closeButtonAnchor);
    
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

inline bool ends_with(const std::string& value, const std::string& ending)
{
    if(ending.size() > value.size())
    {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void WebViewSelector::loadWebView(const std::string& url, Orientation orientation, const Vec2& closeButtonAnchor)
{
    AnalyticsSingleton::getInstance()->contentItemWebviewStartedEvent();
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    // Change the video URLs to test direct video stream
    std::string mediaurl;
    if(ends_with(url, "html"))
    {
        // This is a game
        mediaurl = url;
    }
    else
    {
        // Video
        const std::string& userSessionId = ChildDataProvider::getInstance()->getParentOrChildCdnSessionId();
        mediaurl = "https://tv-media.azoomee.com/netgemstream/" + userSessionId + "/distribution/gb/229a4bec-63bd-437d-abcd-6951a6e6ae99/video_stream.m3u8";
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto iosWebView = WebViewNative_ios::createSceneWithURL(mediaurl, closeButtonAnchor);
    Director::getInstance()->replaceScene(iosWebView);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto androidWebViewCaller = WebViewNativeCaller_android::createSceneWithUrl(mediaurl, orientation, closeButtonAnchor);
    Director::getInstance()->replaceScene(androidWebViewCaller);
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

NS_AZOOMEE_END
