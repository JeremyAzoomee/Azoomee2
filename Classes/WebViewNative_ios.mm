#include "WebViewNative_ios.h"
#include "BaseScene.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "WebViewController_ios.h"
#include "MediaPlayer_ios.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

WebViewController *webViewController;

Scene* WebViewNative_ios::createSceneWithURL(std::string url)
{
    auto scene = Scene::create();
    auto layer = WebViewNative_ios::create();
    scene->addChild(layer);
    layer->startLoadingUrl(url);

    return scene;
}
bool WebViewNative_ios::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    return true;
}

void WebViewNative_ios::onEnterTransitionDidFinish()
{
    Director::getInstance()->purgeCachedData();
    this->setName("iosWebView");
}

//-------------------------------------------All methods are private after this line---------------------------------------

void WebViewNative_ios::startLoadingUrl(std::string url)
{
    //addWebViewToScreen(url);
    addMediaPlayerToScreen();
}

void WebViewNative_ios::removeWebViewFromScreen()
{
    [webViewController removeWebViewWhileInBackground];
}

void WebViewNative_ios::reAddWebViewToScreen()
{
    [webViewController addWebViewToScreen];
    [webViewController createButton];
}

void WebViewNative_ios::addMediaPlayerToScreen()
{
    UIView *currentView = (UIView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    
    MediaPlayerController *mediaPlayer = [[MediaPlayerController alloc] init];
    [currentView addSubview:mediaPlayer.view];
    
    std::string userSessionId = ChildDataProvider::getInstance()->getParentOrChildCdnSessionId();
    std::string mediaURL = "https://tv-media.azoomee.com/netgemstream/"+userSessionId+"/distribution/gb/229a4bec-63bd-437d-abcd-6951a6e6ae99/video_stream.m3u8";
    NSString *iosurl = [NSString stringWithCString:mediaURL.c_str() encoding:[NSString defaultCStringEncoding]];
    
    [mediaPlayer startBuildingMediaPlayer:iosurl];
}

void WebViewNative_ios::addWebViewToScreen(std::string url)
{

    //Please note: cookie handling in ios is automatic. Set-cookie values are getting set from the httprequest's response, and they are being stored in the shared cookie storage. This is not true on Android (furthermore we are not using the built-in browser).
    
    NSHTTPCookieStorage *cookieStorage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    for (NSHTTPCookie *each in cookieStorage.cookies) {
        NSLog(@"Cookies in storage: %@", each);
    }
    
    std::string userid = ChildDataProvider::getInstance()->getLoggedInChildId();
    
    //If game is called, open the game directly, if video / audio, we open up jw player with the given url
    
    NSString *iosurl = [NSString stringWithCString:url.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *iosuserid = [NSString stringWithCString:userid.c_str() encoding:[NSString defaultCStringEncoding]];
    NSLog(@"called url: %@", iosurl);
    
    NSString *iosurlExtension = [iosurl substringFromIndex:MAX((int)[iosurl length]-4, 0)];
    NSLog(@"called url extension: %@", iosurlExtension);
    
    UIView *currentView = (UIView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    
    webViewController = [[WebViewController alloc] init];
    [currentView addSubview:webViewController.view];
    
    [webViewController startBuildingWebView:iosurl userid:iosuserid];
    
    
    currentView = nil;
    cookieStorage = nil;
    iosurl = nil;
    iosuserid = nil;
}

NS_AZOOMEE_END
