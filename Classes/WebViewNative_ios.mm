#include "WebViewNative_ios.h"
#include "BaseScene.h"

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
    UIView *currentView = (UIView*)Director::getInstance()->getOpenGLView()->getEAGLView();
    
    UIWebView *webview=[[UIWebView alloc]initWithFrame:CGRectMake(30, 0, currentView.frame.size.width, currentView.frame.size.height)];
    //NSString *url=@"http://www.google.com";
    
    NSString *htmlFile = [[NSBundle mainBundle] pathForResource:@"res/previewGame/game/index" ofType:@"html"];
    NSURL *nsurl=[NSURL URLWithString:htmlFile];
    NSURLRequest *nsrequest=[NSURLRequest requestWithURL:nsurl];
    [webview loadRequest:nsrequest];
    
    NSLog(@"%@", htmlFile);
    
    //[webview loadHTMLString:htmlString baseURL: [[NSBundle mainBundle] bundleURL]];
    //NSURL *nsurl=[NSURL URLWithString:url];
    //NSURLRequest *nsrequest=[NSURLRequest requestWithURL:nsurl];
    //[webview loadRequest:nsrequest];
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
        CCLOG("touch captured");
        
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        cocos2d::Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        cocos2d::Size s = target->getBoundingBox().size;//getContentSize();
        cocos2d::Rect rect = cocos2d::Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            CCLOG("touch on target, %f, %f", touch->getLocation().x, touch->getLocation().y);
            this->removeWebViewAndBack();
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backButton);
}
