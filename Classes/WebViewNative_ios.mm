#include "WebViewNative_ios.h"
#include "BaseScene.h"
#include "ButtonController_ios.h"

USING_NS_CC;


Scene* WebViewNative_ios::createSceneWithURL(std::string url)
{
    auto scene = Scene::create();
    auto layer = WebViewNative_ios::create();
    scene->addChild(layer);
    
    layer->addWebViewAndButtonToScreen(url);

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
void WebViewNative_ios::addWebViewAndButtonToScreen(std::string url)
{
    //Please note: cookie handling in ios is automatic. Set-cookie values are getting set from the httprequest's response, and they are being stored in the shared cookie storage. This is not true on Android (furthermore we are not using the built-in browser).
    
    NSHTTPCookieStorage *cookieStorage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    for (NSHTTPCookie *each in cookieStorage.cookies) {
        NSLog(@"Cookies in storage: %@", each);
    }
    
    UIView *currentView = (UIView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    UIWebView *webview=[[UIWebView alloc]initWithFrame:CGRectMake(0, 0, currentView.frame.size.width, currentView.frame.size.height)];
    
    //If game is called, open the game directly, if video / audio, we open up jw player with the given url
    
    NSString *iosurl = [NSString stringWithCString:url.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *iosurlExtension = [iosurl substringFromIndex:MAX((int)[iosurl length]-4, 0)];
    
    NSLog(@"called url: %@", iosurl);
    NSLog(@"called url extension: %@", iosurlExtension);
    
    NSString *urlToCall;
    
    if([iosurlExtension isEqualToString:@"html"])
    {
        urlToCall = iosurl;
    }
    else
    {
        NSString *htmlFileAddress = [[NSBundle mainBundle] pathForResource:@"res/jwplayer/index" ofType:@"html"];
        urlToCall = [NSString stringWithFormat:@"%@?contentUrl=%@", htmlFileAddress, iosurl];
    }
    
    NSURL *nsurl=[NSURL URLWithString:urlToCall];
    NSMutableURLRequest *nsrequest=[NSMutableURLRequest requestWithURL:nsurl];
    
    [nsrequest setHTTPMethod:@"GET"];
    
    [webview loadRequest:nsrequest];
    
    [webview setExclusiveTouch:false];
    [currentView addSubview:webview];
    
    [[ButtonController_ios alloc] initWithCloseButton:currentView withWebview:webview];
}
