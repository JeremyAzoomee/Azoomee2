#include "OnlineSafetyDetailsLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "ui/UIWebView.h"

NS_AZOOMEE_BEGIN

#define MARGIN_TO_CHEVRON 150
#define TOTAL_SLIDES 5

Layer* OnlineSafetyDetailsLayer::createWithHeight(float setLayerHeight)
{
    auto layer = OnlineSafetyDetailsLayer::create();
    layer->layerHeight = setLayerHeight - 30;
    layer->currentSlideNumber = 1;
    layer->createBackground();
    layer->addUIObjects();
    
    return layer;
}

bool OnlineSafetyDetailsLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void OnlineSafetyDetailsLayer::addListenerToVideoLayer(Layer* listenerToLayer)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), listenerToLayer);
}

//----------------Add UI Objects-------------

void OnlineSafetyDetailsLayer::createBackground()
{
    backgroundLayer = LayerColor::create(Color4B(255,255,255,255),visibleSize.width, layerHeight);
    this->addChild(backgroundLayer);
}

void OnlineSafetyDetailsLayer::addUIObjects()
{
    //-------------MAIN TITLE DOES NOT CHANGE--------------
    titleLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(ONLINE_SAFETY_MAIN_TITLE), Style::Font::Regular, Style::Color::black, 70);
    titleLabel->setAnchorPoint(Vec2(0.0,1));
    titleLabel->setPosition(MARGIN_TO_CHEVRON*2,layerHeight - titleLabel->getContentSize().height);
    backgroundLayer->addChild(titleLabel);
    
    //------------SLIDE TITLE--------------------
    // Location in relation to the Main Title Location
    
    slideTitleLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(StringUtils::format("%s%d", ONLINE_SAFETY_SLIDE_TITLE,currentSlideNumber)), Style::Font::Bold, Color3B(9,154,154), 70);
    slideTitleLabel->setAnchorPoint(Vec2(0.0,1));
    slideTitleLabel->setPosition(MARGIN_TO_CHEVRON*2,titleLabel->getPositionY() - slideTitleLabel->getContentSize().height*1.5);
    backgroundLayer->addChild(slideTitleLabel);
    
    //-----------SLIDE IMAGE--------------------
    // Location in relation to the Main Title Location
    
    mainImage = Sprite::create(StringUtils::format("res/onlineSafetySlides/safetyIll0%d.png",currentSlideNumber));
    mainImage->setPosition(visibleSize.width-MARGIN_TO_CHEVRON*1.5-mainImage->getContentSize().width/2,slideTitleLabel->getPositionY()-slideTitleLabel->getContentSize().height*1.5- mainImage->getContentSize().height/2);
    backgroundLayer->addChild(mainImage);
    
    //-----------SLIDE MAIN TEXT--------------------
    // Location in relation to the Slide Title Location
    
    mainTextLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(StringUtils::format("%s%d", ONLINE_SAFETY_SLIDE_MAIN_TEXT,currentSlideNumber)), Style::Font::Regular, Style::Color::black, 59);
    mainTextLabel->setLineSpacing(15);
    mainTextLabel->setAnchorPoint(Vec2(0.0,1));
    mainTextLabel->setWidth(mainImage->getPositionX()-mainImage->getContentSize().width/2-MARGIN_TO_CHEVRON*2);
    mainTextLabel->setPosition(MARGIN_TO_CHEVRON*2,slideTitleLabel->getPositionY()-slideTitleLabel->getContentSize().height*2);
    backgroundLayer->addChild(mainTextLabel);
    
    //-----------SLIDE NATVIATION--------------------
    // Location in relation to the Image Location
    
    chevronLeftButton = ElectricDreamsButton::createChevronLeftButton();
    chevronLeftButton->setCenterPosition(Vec2(MARGIN_TO_CHEVRON,mainImage->getPositionY()));
    chevronLeftButton->setDelegate(this);
    backgroundLayer->addChild(chevronLeftButton);
    
    chevronRightButton = ElectricDreamsButton::createChevronRightButton();
    chevronRightButton->setCenterPosition(Vec2(visibleSize.width - MARGIN_TO_CHEVRON,mainImage->getPositionY()));
    chevronRightButton->setDelegate(this);
    backgroundLayer->addChild(chevronRightButton);
    
    watchSearchItUpButton = ElectricDreamsButton::createTextAsButtonWithColor(StringMgr::getInstance()->getStringForKey(ONLINE_SAFETY_BUTTON_TEXT), 59, true, Color3B(9,154,154));
    watchSearchItUpButton->setCenterPosition(Vec2(mainImage->getPositionX(),mainImage->getPositionY()-mainImage->getContentSize().height/2-watchSearchItUpButton->getContentSize().height*2));
    watchSearchItUpButton->setDelegate(this);
    backgroundLayer->addChild(watchSearchItUpButton);
}

void OnlineSafetyDetailsLayer::playVideo()
{
    videoLayer = LayerColor::create(Color4B(255,255,255,255),visibleSize.width, visibleSize.height);
    this->addChild(videoLayer);
    
    addListenerToVideoLayer(videoLayer);
    
    closeVideoButton = ElectricDreamsButton::createWindowCloselButton();
    closeVideoButton->setCenterPosition(Vec2(visibleSize.width - closeVideoButton->getContentSize().width, visibleSize.height - closeVideoButton->getContentSize().height));
    closeVideoButton->setDelegate(this);
    videoLayer->addChild(closeVideoButton);
    
    //auto newHTML = "<div style=\"position:relative;height:0;padding-bottom:56.25%\"><iframe src=\"https://www.youtube.com/embed/lpFV3e6SPwE?rel=0?ecver=2&autoplay=1\" width=\"640\" height=\"360\" frameborder=\"0\" style=\"position:absolute;width:100%;height:100%;left:0\" allowfullscreen></iframe></div>";
    
    //auto newHTML = "<html><head><title>Test</title></head><body><div style=\"position:relative;height:0;padding-bottom:56.25%\"><iframe src=\"https://www.youtube.com/embed/lpFV3e6SPwE?rel=0?ecver=2&autoplay=1\" width=\"640\" height=\"360\" frameborder=\"0\" style=\"position:absolute;width:100%;height:100%;left:0\" allowfullscreen></iframe></div></body></html>";
    
    std::string videoEmbeddedURL = "";
    
    switch (currentSlideNumber) {
        case 1:
        {
            videoEmbeddedURL = "https://www.youtube.com/embed/lpFV3e6SPwE?rel=0?ecver=2&autoplay=1";
            break;
        }
        case 2:
        {
            videoEmbeddedURL = "https://www.youtube.com/embed/lpFV3e6SPwE?rel=0?ecver=2&autoplay=1";
            break;
        }
        case 3:
        {
            videoEmbeddedURL = "https://www.youtube.com/embed/lpFV3e6SPwE?rel=0?ecver=2&autoplay=1";
            break;
        }
        case 4:
        {
            videoEmbeddedURL = "https://www.youtube.com/embed/lpFV3e6SPwE?rel=0?ecver=2&autoplay=1";
            break;
        }
        case 5:
        {
            videoEmbeddedURL = "https://www.youtube.com/embed/lpFV3e6SPwE?rel=0?ecver=2&autoplay=1";
            break;
        }
        default:
            break;
    }
    
    videoWebview = experimental::ui::WebView::create();
    videoWebview->setContentSize(Size(1920,1080));
    videoWebview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoWebview->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    //videoWebview->loadHTMLString(newHTML);
    videoWebview->loadURL(videoEmbeddedURL);
    videoLayer->addChild(videoWebview);
}

void OnlineSafetyDetailsLayer::setToCurrentSlideNumber()
{
    slideTitleLabel->setString(StringMgr::getInstance()->getStringForKey(StringUtils::format("%s%d", ONLINE_SAFETY_SLIDE_TITLE,currentSlideNumber)));
    mainTextLabel->setString(StringMgr::getInstance()->getStringForKey(StringUtils::format("%s%d", ONLINE_SAFETY_SLIDE_MAIN_TEXT,currentSlideNumber)));
    
    mainImage->setTexture(StringUtils::format("res/onlineSafetySlides/safetyIll0%d.png",currentSlideNumber));
}

void OnlineSafetyDetailsLayer::moveSlideNumberBy(int moveBy)
{
    currentSlideNumber = currentSlideNumber + moveBy;
    
    if(currentSlideNumber > TOTAL_SLIDES)
        currentSlideNumber = 1;
    else if(currentSlideNumber < 1)
        currentSlideNumber = 5;
    
    setToCurrentSlideNumber();
}

//----------------------- Delegate Functions ----------------------------

void OnlineSafetyDetailsLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == watchSearchItUpButton)
        playVideo();
    else if(button == chevronLeftButton)
        moveSlideNumberBy(-1);
    else if(button == chevronRightButton)
        moveSlideNumberBy(1);
    else if(button == closeVideoButton)
    {
        videoWebview->loadURL("about:blank");
        videoLayer->removeFromParent();
    }
}

NS_AZOOMEE_END
