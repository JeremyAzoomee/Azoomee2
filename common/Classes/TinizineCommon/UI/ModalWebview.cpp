#include "ModalWebview.h"

using namespace cocos2d;

#define CIRCLE_TAG 1000
#define WEBVIEW_Z_ORDER 2000

NS_TZ_BEGIN

ModalWebview* ModalWebview::createWithURL(const std::string& url, const std::string& closeButtonFilename)
{
    auto layer = ModalWebview::create();
    layer->createCloseButton(closeButtonFilename);
    layer->createWebView(url);
    layer->addLoadingCircles();
    
    Director::getInstance()->getRunningScene()->addChild(layer, WEBVIEW_Z_ORDER);
    return layer;
}

bool ModalWebview::init()
{
    if ( !Super::init() )
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0, 1.0));
    setBackGroundColor(Color3B::WHITE);
    setBackGroundColorType(BackGroundColorType::SOLID);
    setBackGroundColorOpacity(255);
    setSwallowTouches(true);
    setTouchEnabled(true);
    
    this->setName("ModalWebview");
    
    return true;
}

//---------------------- Create Background Layer -----------------------------

void ModalWebview::createCloseButton(const std::string& closeButtonFilename)
{
    const Size& contentSize = Director::getInstance()->getVisibleSize();
    float size = MAX(contentSize.width, contentSize.height) * 0.05f;
    _closeButton = cocos2d::ui::Button::create(closeButtonFilename);
    _closeButton->setAnchorPoint(Vec2(1.5,1.5));
    _closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    _closeButton->ignoreContentAdaptWithSize(false);
    _closeButton->setContentSize(Size(size, size));
    _closeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->removeFromParent();
        }
    });
    addChild(_closeButton);
}

void ModalWebview::createWebView(const std::string& url)
{
    _modalWebview = experimental::ui::WebView::create();
    _modalWebview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _modalWebview->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _modalWebview->setContentSize(getContentSize() * 0.8f); // use abs size as percentage sizing doesnt seem to work on webview
    _modalWebview->setJavascriptInterfaceScheme("");
    _modalWebview->loadURL(url);
    // WebView does not have callback on Amazon
    // Due to Android having different webviews, we choose to be safe
    // and make all non iOS just load directly.
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        _modalWebview->setOnDidFinishLoading(CC_CALLBACK_2(ModalWebview::callbackFromJS, this));
        _modalWebview->setVisible(false);
    #else
        _modalWebview->setVisible(true);
    #endif
    
    addChild(_modalWebview);
}

void ModalWebview::addLoadingCircles()
{
    for(int i = 0; i < 3; i++)
    {
        auto loadingCircle = Sprite::create("res/modal/loading.png");
        loadingCircle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        loadingCircle->setRotation(RandomHelper::random_int(0, 360));
        loadingCircle->setScale(0.6f + i * 0.2f);
        loadingCircle->setTag(CIRCLE_TAG);
        
        addChild(loadingCircle);
        
        int direction = 1;
        if(CCRANDOM_0_1() < 0.5f)
        {
            direction = -1;
        }
        
        loadingCircle->runAction(RepeatForever::create(RotateBy::create(CCRANDOM_0_1() + 1, 360 * direction)));
    }
}
    
void ModalWebview::callbackFromJS(cocos2d::experimental::ui::WebView* webview, const std::string &answer)
{
    while(getChildByTag(CIRCLE_TAG))
    {
       removeChildByTag(CIRCLE_TAG);
    }
    
    _modalWebview->setVisible(true);
}

void ModalWebview::onSizeChanged()
{
    Super::onSizeChanged();
    _modalWebview->setContentSize(getContentSize() * 0.8f);
}

NS_TZ_END
