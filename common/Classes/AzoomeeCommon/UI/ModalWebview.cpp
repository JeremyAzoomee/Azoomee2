#include "ModalWebview.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Layer* ModalWebview::createWithURL(std::string url)
{
    auto layer = ModalWebview::create();
    layer->createBackgroundLayer();
    layer->addListenerToBackgroundLayer();
    layer->createCloseButton();
    layer->createWebView(url);
    layer->addLoadingCircles();
    
    Director::getInstance()->getRunningScene()->addChild(layer);
    return layer;
}

bool ModalWebview::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

//---------------------- Create Background Layer -----------------------------

void ModalWebview::createBackgroundLayer()
{
    backgroundLayer = LayerColor::create(Color4B(255,255,255,255),visibleSize.width, visibleSize.height);
    backgroundLayer->setPosition(origin.x,origin.y);
    this->addChild(backgroundLayer);
}

void ModalWebview::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

void ModalWebview::createCloseButton()
{
    closeButton = ElectricDreamsButton::createWindowCloseButton();
    closeButton->setCenterPosition(Vec2(visibleSize.width - closeButton->getContentSize().width, visibleSize.height - closeButton->getContentSize().height));
    closeButton->setDelegate(this);
    closeButton->setMixPanelButtonName("ModalWebview-CloseButton");
    backgroundLayer->addChild(closeButton);
}

void ModalWebview::createWebView(std::string url)
{
    modalWebview = experimental::ui::WebView::create();
    modalWebview->setContentSize(Size(visibleSize.width*.8,visibleSize.height*.8));
    modalWebview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    modalWebview->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    modalWebview->setJavascriptInterfaceScheme("");
    modalWebview->loadURL(url);
    modalWebview->setOnDidFinishLoading(CC_CALLBACK_2(ModalWebview::callbackFromJS, this));
    modalWebview->setVisible(false);
    backgroundLayer->addChild(modalWebview);
}

void ModalWebview::addLoadingCircles()
{
    for(int i = 0; i < 3; i++)
    {
        auto loadingCircle = Sprite::create("res/modal/loading.png");
        loadingCircle->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        loadingCircle->setRotation(RandomHelper::random_int(0, 360));
        loadingCircle->setScale(0.6 + i * 0.2);
        loadingCircle->setTag(1000);
        
        backgroundLayer->addChild(loadingCircle);
        
        int direction = 1;
        if(CCRANDOM_0_1() < 0.5) direction = -1;
        
        loadingCircle->runAction(RepeatForever::create(RotateBy::create(CCRANDOM_0_1() + 1, 360 * direction)));
    }
}
    
void ModalWebview::callbackFromJS(cocos2d::experimental::ui::WebView* webview, const std::string &answer)
{
    while(backgroundLayer->getChildByTag(1000))
        backgroundLayer->removeChildByTag(1000);
    
    modalWebview->setVisible(true);
}

//---------------------- Actions -----------------

void ModalWebview::removeSelf()
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}

//----------------------- Delegate Functions ----------------------------

void ModalWebview::buttonPressed(ElectricDreamsButton* button)
{
    if(button == closeButton)
        removeSelf();
}
}
