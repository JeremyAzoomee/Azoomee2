#include "WebViewNative_ios.h"
#include "BaseScene.h"

USING_NS_CC;


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

//-------------------------------------------All methods are private after this line---------------------------------------

void WebViewNative_ios::startLoadingUrl(std::string url)
{
    addWebViewToScreen(url);
    addBackButtonToScreen();
    addListenerToBackButton();
}

void WebViewNative_ios::addWebViewToScreen(std::string url)
{

    //Please note: cookie handling in ios is automatic. Set-cookie values are getting set from the httprequest's response, and they are being stored in the shared cookie storage. This is not true on Android (furthermore we are not using the built-in browser).
    
    NSHTTPCookieStorage *cookieStorage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    for (NSHTTPCookie *each in cookieStorage.cookies) {
        NSLog(@"Cookies in storage: %@", each);
    }
    
    UIView *currentView = (UIView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    UIWebView *webview=[[UIWebView alloc]initWithFrame:CGRectMake(30, 0, currentView.frame.size.width, currentView.frame.size.height)];
    
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
    
}

void WebViewNative_ios::addBackButtonToScreen()
{
    cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
    
    backButton = Sprite::create("CloseNormal.png");
    backButton->setScale(3);
    backButton->setPosition(origin.x + backButton->getBoundingBox().size.width / 2, origin.y + backButton->getBoundingBox().size.height / 2);
    this->addChild(backButton);
}

void WebViewNative_ios::removeWebViewAndBack()
{
    UIView *currentView = (UIView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    
    for(UIView *subview in currentView.subviews)
    {
        [subview removeFromSuperview];
    }
    
    auto baseScene = BaseScene::createScene();
    Director::getInstance()->replaceScene(baseScene);
}

void WebViewNative_ios::addListenerToBackButton()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        cocos2d::Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        cocos2d::Size s = target->getBoundingBox().size;//getContentSize();
        cocos2d::Rect rect = cocos2d::Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            this->removeWebViewAndBack();
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backButton);
}
