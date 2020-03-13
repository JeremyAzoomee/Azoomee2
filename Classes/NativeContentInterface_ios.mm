#include "NativeContentInterface_ios.h"
#include <TinizineCommon/Data/Child/ChildManager.h>
#include <TinizineCommon/Utils/StringFunctions.h>
#include "WebViewController_ios.h"
#include "MediaPlayer_ios.h"
#include "WebViewSelector.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

//#define REMOVE_WEBVIEW_ON_BG YES

#ifdef REMOVE_WEBVIEW_ON_BG
WebViewController *webViewController = nil;
#endif

cocos2d::Scene* NativeContentInterface_ios::createSceneWithURL(const std::string &url, const Vec2& closeButtonAnchor, int videoProgressSeconds)
{
    auto scene = cocos2d::Scene::create();
    auto layer = NativeContentInterface_ios::create();
    scene->addChild(layer);
    layer->loadContentBasedOnUrl(url, closeButtonAnchor, videoProgressSeconds);

    return scene;
}
bool NativeContentInterface_ios::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    return true;
}

void NativeContentInterface_ios::onEnterTransitionDidFinish()
{
    Director::getInstance()->purgeCachedData();
    this->setName(WebViewSelector::kIosWebviewName);
}

void NativeContentInterface_ios::onExit()
{
#ifdef REMOVE_WEBVIEW_ON_BG
    if(webViewController != nil)
    {
        [webViewController release];
        webViewController = nil;
    }
#endif
    Layer::onExit();
}

//-------------------------------------------All methods are private after this line---------------------------------------

void NativeContentInterface_ios::loadContentBasedOnUrl(const std::string &url, const Vec2& closeButtonAnchor, int videoProgressSeconds)
{
    if(StringFunctions::stringEndsWith(url, "m3u8"))
    {
        addMediaPlayerToScreen(url, videoProgressSeconds);
    }
    else
    {
        addWebViewToScreen(url, closeButtonAnchor);
    }
}

void NativeContentInterface_ios::removeWebViewFromScreen()
{
#ifdef REMOVE_WEBVIEW_ON_BG
	if(webViewController != nil)
	{
		[webViewController removeWebViewWhileInBackground];
	}
#endif
}

void NativeContentInterface_ios::reAddWebViewToScreen()
{
#ifdef REMOVE_WEBVIEW_ON_BG
	if(webViewController != nil)
	{
		[webViewController addWebViewToScreen];
		[webViewController createButtons];
	}
#endif
}

void NativeContentInterface_ios::addMediaPlayerToScreen(const std::string &url, int videoProgressSeconds)
{
    UIView *currentView = (UIView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    
    MediaPlayerController *mediaPlayer = [[MediaPlayerController alloc] init];
    [currentView addSubview:mediaPlayer.view];
    
    NSString *iosurl = [NSString stringWithCString:url.c_str() encoding:[NSString defaultCStringEncoding]];
	
	[mediaPlayer startBuildingMediaPlayer:iosurl progressSeconds:videoProgressSeconds];
    [mediaPlayer release];
}

void NativeContentInterface_ios::addWebViewToScreen(const std::string &url, const Vec2& closeButtonAnchor)
{
    //Please note: cookie handling in ios is automatic. Set-cookie values are getting set from the httprequest's response, and they are being stored in the shared cookie storage. This is not true on Android (furthermore we are not using the built-in browser).
    
    NSHTTPCookieStorage *cookieStorage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    for (NSHTTPCookie *each in cookieStorage.cookies) {
        NSLog(@"Cookies in storage: %@", each);
    }
    
    const std::string& userid = ChildManager::getInstance()->getLoggedInChild()->getId();
    
    //If game is called, open the game directly, if video / audio, we open up jw player with the given url
    
    NSString *iosurl = [NSString stringWithCString:url.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *iosuserid = [NSString stringWithCString:userid.c_str() encoding:[NSString defaultCStringEncoding]];
    NSLog(@"called url: %@", iosurl);
    
    NSString *iosurlExtension = [iosurl substringFromIndex:MAX((int)[iosurl length]-4, 0)];
    NSLog(@"called url extension: %@", iosurlExtension);
    
    UIView *currentView = (UIView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    
#ifdef REMOVE_WEBVIEW_ON_BG
    webViewController = [[WebViewController alloc] init];
    [currentView addSubview:webViewController.view];
    [webViewController startBuildingWebView:iosurl userid:iosuserid closeButtonAnchorX:closeButtonAnchor.x closeButtonAnchorY:closeButtonAnchor.y];
#else
    WebViewController* webViewController = [[WebViewController alloc] init];
    [currentView addSubview:webViewController.view];
    [webViewController startBuildingWebView:iosurl userid:iosuserid closeButtonAnchorX:closeButtonAnchor.x closeButtonAnchorY:closeButtonAnchor.y];
    [WebViewController release];
#endif
}

NS_AZ_END
