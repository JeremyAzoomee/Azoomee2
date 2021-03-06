#include "NativeContentInterface_ios.h"
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "WebViewController_ios.h"
#include "MediaPlayer_ios.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

WebViewController *webViewController = nil;

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
    this->setName(ConfigStorage::kIosWebviewName);
}

void NativeContentInterface_ios::onExit()
{
    if(webViewController != nil)
    {
        [webViewController release];
        webViewController = nil;
    }
    Layer::onExit();
}

//-------------------------------------------All methods are private after this line---------------------------------------

void NativeContentInterface_ios::loadContentBasedOnUrl(const std::string &url, const Vec2& closeButtonAnchor, int videoProgressSeconds)
{
    if(stringEndsWith(url, "m3u8"))
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
	if(webViewController != nil)
	{
		[webViewController removeWebViewWhileInBackground];
	}
}

void NativeContentInterface_ios::reAddWebViewToScreen()
{
	if(webViewController != nil)
	{
		[webViewController addWebViewToScreen];
		[webViewController createButtons];
	}
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
    
    const std::string& userid = ChildManager::getInstance()->getParentOrChildId();
    
    //If game is called, open the game directly, if video / audio, we open up jw player with the given url
    
    NSString *iosurl = [NSString stringWithCString:url.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *iosuserid = [NSString stringWithCString:userid.c_str() encoding:[NSString defaultCStringEncoding]];
    NSLog(@"called url: %@", iosurl);
    
    NSString *iosurlExtension = [iosurl substringFromIndex:MAX((int)[iosurl length]-4, 0)];
    NSLog(@"called url extension: %@", iosurlExtension);
    
    UIView *currentView = (UIView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    
    webViewController = [[WebViewController alloc] init];
    [currentView addSubview:webViewController.view];
    
    [webViewController startBuildingWebView:iosurl userid:iosuserid closeButtonAnchorX:closeButtonAnchor.x closeButtonAnchorY:closeButtonAnchor.y];
    //[webViewController release];
    
    //currentView = nil;
    //cookieStorage = nil;
    //iosurl = nil;
    //iosuserid = nil;
}

NS_AZOOMEE_END
