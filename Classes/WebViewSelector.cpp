#include "WebViewSelector.h"
#include <AzoomeeCommon/Data/Cookie/CookieDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/API/API.h>
#include "SceneManagerScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "NativeContentInterface_ios.h"
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

void WebViewSelector::loadWebView(const std::string& url, Orientation orientation, const Vec2& closeButtonAnchor)
{
    AnalyticsSingleton::getInstance()->contentItemWebviewStartedEvent();
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    _targetUrl = url;
    _orientation = orientation;
    _closeButtonAnchor = closeButtonAnchor;
    
    if(stringEndsWith(_targetUrl, "m3u8")) //this if clause will probably need changes for later
    {
        const std::string& userSessionId = ChildDataProvider::getInstance()->getParentOrChildCdnSessionId();
        _targetUrl = replaceAll(_targetUrl, "{sessionId}", userSessionId);
        HttpRequestCreator* offlineCheck = API::OfflineCheck(this);
        offlineCheck->execute();
    }
    else
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        auto iosWebView = NativeContentInterface_ios::createSceneWithURL(_targetUrl, _closeButtonAnchor);
        Director::getInstance()->replaceScene(iosWebView);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        auto androidWebViewCaller = WebViewNativeCaller_android::createSceneWithUrl(_targetUrl, _orientation, _closeButtonAnchor);
        Director::getInstance()->replaceScene(androidWebViewCaller);
#endif
    }
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

void WebViewSelector::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagOfflineCheck)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        auto iosWebView = NativeContentInterface_ios::createSceneWithURL(_targetUrl, _closeButtonAnchor);
        Director::getInstance()->replaceScene(iosWebView);
#endif
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        auto androidWebViewCaller = WebViewNativeCaller_android::createSceneWithUrl(_targetUrl, _orientation, _closeButtonAnchor);
        Director::getInstance()->replaceScene(androidWebViewCaller);
#endif
    }
}
void WebViewSelector::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    if(requestTag == API::TagOfflineCheck)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(OfflineHub));
    }
}


NS_AZOOMEE_END
