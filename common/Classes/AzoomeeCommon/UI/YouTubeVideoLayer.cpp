#include "YouTubeVideoLayer.h"

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
    
    backgroundLayer = LayerColor::create(Color4B(255,255,255,255),currentRunningScene->getContentSize().width, currentRunningScene->getContentSize().height);
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
    closeVideoButton = ElectricDreamsButton::createWindowCloselButton();
    closeVideoButton->setDelegate(this);
    closeVideoButton->setMixPanelButtonName("CloseVideo");
    backgroundLayer->addChild(closeVideoButton);
}
    
void YouTubeVideoLayer::createVideoWebview()
{
    //560 x 315
    std::string videoHTML = "<iframe width=\"560\" height=\"315\" src=\"https://www.youtube.com/embed///riCzbUj_ow8?rel=0&amp?ecver=2&autoplay=1;showinfo=0\" frameborder=\"0\" allowfullscreen></iframe>";
    
    /*std::string videoHTML = "<iframe width=\"140\" height=\"78.75\" src=\"https://www.youtube.com/embed/riCzbUj_ow8?rel=0&amp?ecver=2&autoplay=1;showinfo=0\" frameborder=\"0\" allowfullscreen></iframe>";*/
    
    videoWebview = experimental::ui::WebView::create();
    videoWebview->setContentSize(Size(2240,1260));
    videoWebview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoWebview->loadHTMLString(videoHTML);
    backgroundLayer->addChild(videoWebview);
}
//---------------------- Actions -----------------

void YouTubeVideoLayer::removeSelf(float dt)
{
    
    if(this)
    {
        this->removeChild(backgroundLayer);
        this->removeFromParent();
    }
}
    
void YouTubeVideoLayer::onSizeChanged()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    backgroundLayer->setContentSize(currentRunningScene->getContentSize());
    
    if(currentRunningScene->getContentSize().height < currentRunningScene->getContentSize().width)
        setToLandscape();
    else
        setToPortrait();

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
    
    videoWebview->setRotation(0);
    videoWebview->setContentSize(Size(videoViewWidth,videoViewHeight));
    videoWebview->setPosition(Vec2(currentRunningScene->getContentSize().width/2,currentRunningScene->getContentSize().height/2));
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
    
    videoWebview->setRotation(90);
    videoWebview->setContentSize(Size(videoViewWidth,videoViewHeight));
    videoWebview->setPosition(Vec2(currentRunningScene->getContentSize().width/2,currentRunningScene->getContentSize().height/2));
}
    
//----------------------- Delegate Functions ----------------------------

void YouTubeVideoLayer::buttonPressed(ElectricDreamsButton* button)
{
    videoWebview->loadURL("about:blank");
    this->scheduleOnce(schedule_selector(YouTubeVideoLayer::removeSelf), 0.1);
}
                    
}
