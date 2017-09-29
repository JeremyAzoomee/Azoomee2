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
    
    this->setName("ModalWebview");
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

//---------------------- Create Background Layer -----------------------------

void ModalWebview::createBackgroundLayer()
{
    _backgroundLayer = LayerColor::create(Color4B(255,255,255,255),_visibleSize.width, _visibleSize.height);
    _backgroundLayer->setPosition(_origin.x,_origin.y);
    this->addChild(_backgroundLayer);
}

void ModalWebview::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), _backgroundLayer);
}

void ModalWebview::createCloseButton()
{
    _closeButton = ElectricDreamsButton::createWindowCloselButton();
    _closeButton->setCenterPosition(Vec2(_visibleSize.width - _closeButton->getContentSize().width, _visibleSize.height - _closeButton->getContentSize().height));
    _closeButton->setDelegate(this);
    _closeButton->setMixPanelButtonName("ModalWebview-CloseButton");
    _backgroundLayer->addChild(_closeButton);
}

void ModalWebview::createWebView(std::string url)
{
    _modalWebview = experimental::ui::WebView::create();
    _modalWebview->setContentSize(Size(_visibleSize.width*.8,_visibleSize.height*.8));
    _modalWebview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _modalWebview->setPosition(Vec2(_visibleSize.width/2,_visibleSize.height/2));
    _modalWebview->setJavascriptInterfaceScheme("");
    _modalWebview->loadURL(url);
    _modalWebview->setOnDidFinishLoading(CC_CALLBACK_2(ModalWebview::callbackFromJS, this));
    _modalWebview->setVisible(false);
    _backgroundLayer->addChild(_modalWebview);
}

void ModalWebview::addLoadingCircles()
{
    for(int i = 0; i < 3; i++)
    {
        auto loadingCircle = Sprite::create("res/modal/loading.png");
        loadingCircle->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);
        loadingCircle->setRotation(RandomHelper::random_int(0, 360));
        loadingCircle->setScale(0.6 + i * 0.2);
        loadingCircle->setTag(1000);
        
        _backgroundLayer->addChild(loadingCircle);
        
        int direction = 1;
        if(CCRANDOM_0_1() < 0.5) direction = -1;
        
        loadingCircle->runAction(RepeatForever::create(RotateBy::create(CCRANDOM_0_1() + 1, 360 * direction)));
    }
}
    
void ModalWebview::callbackFromJS(cocos2d::experimental::ui::WebView* webview, const std::string &answer)
{
    while(_backgroundLayer->getChildByTag(1000))
        _backgroundLayer->removeChildByTag(1000);
    
    _modalWebview->setVisible(true);
}

//---------------------- Actions -----------------

void ModalWebview::removeSelf()
{
    this->removeChild(_backgroundLayer);
    this->removeFromParent();
}

void ModalWebview::onSizeChanged()
{
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    _backgroundLayer->setContentSize(currentRunningScene->getContentSize());
    
    _closeButton->setCenterPosition(Vec2(_visibleSize.width - _closeButton->getContentSize().width, _visibleSize.height - _closeButton->getContentSize().height));
    
    _modalWebview->setContentSize(Size(_visibleSize.width*.8,_visibleSize.height*.8));
    _modalWebview->setPosition(Vec2(_visibleSize.width/2,_visibleSize.height/2));
}

//----------------------- Delegate Functions ----------------------------

void ModalWebview::buttonPressed(ElectricDreamsButton* button)
{
    if(button == _closeButton)
        removeSelf();
}
}
