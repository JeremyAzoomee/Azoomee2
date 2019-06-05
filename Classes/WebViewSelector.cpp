#include "WebViewSelector.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/ErrorCodes.h>
#include "SceneManagerScene.h"
#include "ContentHistoryManager.h"
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
    
    layer->setParams(url, orientation, closeButtonAnchor);
    
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
	ContentHistoryManager::getInstance()->onContentOpened();
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    if(stringEndsWith(_targetUrl, "m3u8")) //this if clause will probably need changes for later
    {
        const std::string& userSessionId = ChildDataProvider::getInstance()->getParentOrChildCdnSessionId();
        _targetUrl = replaceAll(_targetUrl, "{sessionId}", userSessionId);
		HttpRequestCreator* progressCheck = API::GetVideoProgress(ChildDataProvider::getInstance()->getParentOrChildId(), ContentHistoryManager::getInstance()->getLastOpenedContent()->getContentItemId(),this);
        progressCheck->execute();
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
    if ( !Super::init() )
    {
        return false;
    }

    
    return true;
}

void WebViewSelector::onEnter()
{
    Super::onEnter();
    
    loadWebView(_targetUrl, _orientation, _closeButtonAnchor);
}

void WebViewSelector::setParams(const std::string &url, Azoomee::Orientation orientation, const cocos2d::Vec2 &closeButtonAnchor)
{
    _targetUrl = url;
    _orientation = orientation;
    _closeButtonAnchor = closeButtonAnchor;
}

void WebViewSelector::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagGetVideoProgress)
    {
		rapidjson::Document data;
		data.Parse(body.c_str());
		int progress = 0;
		if(!data.HasParseError())
		{
			progress = getIntFromJson("contentProgress", data, 0);
		}
		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        auto iosWebView = NativeContentInterface_ios::createSceneWithURL(_targetUrl, _closeButtonAnchor, progress);
        Director::getInstance()->replaceScene(iosWebView);
#endif
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        auto androidWebViewCaller = WebViewNativeCaller_android::createSceneWithUrl(_targetUrl, _orientation, _closeButtonAnchor, progress);
        Director::getInstance()->replaceScene(androidWebViewCaller);
#endif
    }
}
void WebViewSelector::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	if(errorCode == ERROR_CODE_OFFLINE)
	{
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
	}
	else
	{
		onHttpRequestSuccess(requestTag, "", "");
	}
}


NS_AZOOMEE_END
