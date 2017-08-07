#include "YouTubeVideoLayer.h"
#include "ElectricDreamsDecoration.h"

using namespace cocos2d;

namespace Azoomee
{

YouTubeVideoLayer* YouTubeVideoLayer::createWith(std::string YouTubeID)
{
    auto layer = YouTubeVideoLayer::create();

    return layer;
}

bool YouTubeVideoLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    createBackgroundLayer();
    addListenerToBackgroundLayer();
    createCloseButton();
    createVideoWebview();
    onSizeChanged();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void YouTubeVideoLayer::createBackgroundLayer()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    backgroundLayer = LayerColor::create(Color4B(0,0,0,255),currentRunningScene->getContentSize().width, currentRunningScene->getContentSize().height);
    addSideWiresToScreen(backgroundLayer);
    
    this->addChild(backgroundLayer);
    
    
    
    Director::getInstance()->getRunningScene()->addChild(this);
}

void YouTubeVideoLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}
    
void YouTubeVideoLayer::createCloseButton()
{
    closeVideoButton = ElectricDreamsButton::createWhiteWindowCloselButton();
    closeVideoButton->setDelegate(this);
    closeVideoButton->setMixPanelButtonName("CloseVideo");
    backgroundLayer->addChild(closeVideoButton);
}
    
void YouTubeVideoLayer::createVideoWebview()
{
    _webViewStartedLoading = true;
    videoWebview = experimental::ui::WebView::create();
    videoWebview->setContentSize(Size(560,315));
    videoWebview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoWebview->loadFile("res/videoScreen/index.html");
    backgroundLayer->addChild(videoWebview);
    videoWebview->setVisible(true);
    
    videoWebview->setOnDidFinishLoading(CC_CALLBACK_0(YouTubeVideoLayer::webViewDidFinishLoading, this));
    
    addLoadingLayerOnTopOfWebView();
}
    
void YouTubeVideoLayer::webViewDidFinishLoading()
{
    if(!_webViewStartedLoading) return;
    
    _webViewStartedLoading = false;
    videoWebview->evaluateJS("addFrameToScreen(\"riCzbUj_ow8\")");
    this->removeChild(_loadingLayer);
    videoWebview->setVisible(true);
}
    
void YouTubeVideoLayer::addLoadingLayerOnTopOfWebView()
{
    if(this->getChildByName("loadingLayer")) return;
    
    _loadingLayer = LayerColor::create(Color4B(0,0,0,150), videoWebview->getContentSize().width, videoWebview->getContentSize().height);
    _loadingLayer->setPosition(this->getContentSize().width / 2 - _loadingLayer->getContentSize().width / 2, this->getContentSize().height / 2 - _loadingLayer->getContentSize().height / 2);
    _loadingLayer->setName("loadingLayer");
    
    _loadingLayer->setOpacity(0);
    this->addChild(_loadingLayer);
    
    _loadingLayer->runAction(FadeTo::create(0.5, 200));
    
    for(int i = 0; i < 3; i++)
    {
        auto loadingCircle = Sprite::create("res/modal/loading.png");
        loadingCircle->setPosition(_loadingLayer->getContentSize().width / 2, _loadingLayer->getContentSize().height / 2);
        loadingCircle->setOpacity(0);
        loadingCircle->setRotation(RandomHelper::random_int(0, 360));
        loadingCircle->setScale(0.6 + i * 0.2);
        
        _loadingLayer->addChild(loadingCircle);
        
        int direction = 1;
        if(CCRANDOM_0_1() < 0.5) direction = -1;
        
        loadingCircle->runAction(RepeatForever::create(RotateBy::create(CCRANDOM_0_1() + 1, 360 * direction)));
        loadingCircle->runAction(FadeTo::create(0.5, 255));
    }
}
//---------------------- Actions -----------------

void YouTubeVideoLayer::removeSelf(float dt)
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}
    
void YouTubeVideoLayer::onSizeChanged()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    backgroundLayer->setContentSize(currentRunningScene->getContentSize());
    
    if(currentRunningScene->getContentSize().height < currentRunningScene->getContentSize().width)
        setToLandscape();
    else
        setToPortrait();
    
    addLoadingLayerOnTopOfWebView();
    videoWebview->reload();
    _webViewStartedLoading = true;

}
    
void YouTubeVideoLayer::setToLandscape()
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    auto currentRunningSceneContentSize = currentRunningScene->getContentSize();
    
    closeVideoButton->setCenterPosition(Vec2(origin.x + visibleSize.width - closeVideoButton->getContentSize().width, origin.y + visibleSize.height - closeVideoButton->getContentSize().height));
    
    float videoViewWidth = currentRunningScene->getContentSize().width *.8;
    float videoViewHeight = videoViewWidth * (315.0f/560.0f);
    
    if(videoViewHeight > currentRunningScene->getContentSize().height *.8)
    {
        videoViewHeight = currentRunningScene->getContentSize().height *.8;
        videoViewWidth = videoViewHeight * (560.0f/315.0f);
    }
    
    //videoWebview->setContentSize(Size(videoViewWidth,videoViewHeight));
    videoWebview->setContentSize(Size(1120,630));
    videoWebview->setScalesPageToFit(true);
    videoWebview->setPosition(Vec2(currentRunningScene->getContentSize().width/2,currentRunningScene->getContentSize().height/2));
    videoWebview->setVisible(true);
}

void YouTubeVideoLayer::setToPortrait()
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    auto currentRunningSceneContentSize = currentRunningScene->getContentSize();
    
    closeVideoButton->setCenterPosition(Vec2(origin.x + visibleSize.width - closeVideoButton->getContentSize().width, origin.y + closeVideoButton->getContentSize().height));
    
    float videoViewWidth = currentRunningScene->getContentSize().height *.8;
    float videoViewHeight = videoViewWidth * (315.0f/560.0f);
    
    if(videoViewHeight > currentRunningScene->getContentSize().width *.8)
    {
        videoViewHeight = currentRunningScene->getContentSize().width *.8;
        videoViewWidth = videoViewHeight * (560.0f/315.0f);
    }
    
    videoWebview->setContentSize(Size(560,315));
    videoWebview->setScalesPageToFit(true);
    videoWebview->setPosition(Vec2(currentRunningScene->getContentSize().width/2,currentRunningScene->getContentSize().height/2));
    videoWebview->setVisible(true);
}
    
//----------------------- Delegate Functions ----------------------------

void YouTubeVideoLayer::buttonPressed(ElectricDreamsButton* button)
{
    videoWebview->loadURL("about:blank");
    this->scheduleOnce(schedule_selector(YouTubeVideoLayer::removeSelf), 0.1);
}
                    
}
