#include "WebViewNative_ios.h"
#include "BaseScene.h"
#include "DataStorage.h"

USING_NS_CC;


Scene* WebViewNative_ios::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = WebViewNative_ios::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool WebViewNative_ios::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    addWebViewToScreen();
    addBackButtonToScreen();
    addListenerToBackButton();

    return true;
}

void WebViewNative_ios::addWebViewToScreen()
{
    
    //defaultCStringEncoding kills newlines added in C++, so we need to get the original string with commas and convert them to newlines on objective c level.
    NSString *cookiesString = [NSString stringWithCString:DataStorage::getInstance()->dataDownloadCookiesWithCommas.c_str() encoding:[NSString defaultCStringEncoding]];
    cookiesString = [cookiesString stringByReplacingOccurrencesOfString:@", " withString:@"\n"];
    
    UIView *currentView = (UIView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    UIWebView *webview=[[UIWebView alloc]initWithFrame:CGRectMake(30, 0, currentView.frame.size.width, currentView.frame.size.height)];
    
    NSString *htmlFileAddress = @"https://media.azoomee.ninja/free/f50a74dd-185f-4010-ab6f-b34858b96bcd/video_stream.m3u8";
    NSURL *nsurl=[NSURL URLWithString:htmlFileAddress];
    NSMutableURLRequest *nsrequest=[NSMutableURLRequest requestWithURL:nsurl];
    
    [nsrequest setHTTPMethod:@"GET"];
    [nsrequest addValue:cookiesString forHTTPHeaderField:@"Cookie"];
    
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
