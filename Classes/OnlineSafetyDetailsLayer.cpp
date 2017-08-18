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
    
    slideTitleLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(StringUtils::format("%sAdult%d", ONLINE_SAFETY_SLIDE_TITLE,currentSlideNumber)), Style::Font::Bold, Style::Color::safetySlideTitleColor, 70);
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
    
    mainTextLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(StringUtils::format("%sAdult%d", ONLINE_SAFETY_SLIDE_MAIN_TEXT,currentSlideNumber)), Style::Font::Regular, Style::Color::black, 59);
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
    chevronLeftButton->setMixPanelButtonName("Settings-OnlineSafety-LeftChevron");
    backgroundLayer->addChild(chevronLeftButton);
    
    chevronRightButton = ElectricDreamsButton::createChevronRightButton();
    chevronRightButton->setCenterPosition(Vec2(visibleSize.width - MARGIN_TO_CHEVRON,mainImage->getPositionY()));
    chevronRightButton->setDelegate(this);
    chevronRightButton->setMixPanelButtonName("Settings-OnlineSafety-RighChevron");
    backgroundLayer->addChild(chevronRightButton);
    
    watchSearchItUpButton = ElectricDreamsButton::createTextAsButtonWithColor(StringMgr::getInstance()->getStringForKey(ONLINE_SAFETY_BUTTON_TEXT), 59, true, Style::Color::safetySlideTitleColor);
    watchSearchItUpButton->setCenterPosition(Vec2(mainImage->getPositionX(),mainImage->getPositionY()-mainImage->getContentSize().height/2-watchSearchItUpButton->getContentSize().height*2));
    watchSearchItUpButton->setDelegate(this);
    watchSearchItUpButton->setMixPanelButtonName("Settings-OnlineSafety-watchSearchItUp");
    backgroundLayer->addChild(watchSearchItUpButton);
}

void OnlineSafetyDetailsLayer::playVideo()
{
    videoLayer = LayerColor::create(Color4B(255,255,255,255),visibleSize.width, visibleSize.height);
    this->addChild(videoLayer);
    
    addListenerToVideoLayer(videoLayer);
    
    closeVideoButton = ElectricDreamsButton::createWhiteWindowCloselButton();
    closeVideoButton->setCenterPosition(Vec2(visibleSize.width - closeVideoButton->getContentSize().width, visibleSize.height - closeVideoButton->getContentSize().height));
    closeVideoButton->setDelegate(this);
    closeVideoButton->setMixPanelButtonName("Settings-OnlineSafety-CloseVideo");
    videoLayer->addChild(closeVideoButton);
    
    //One YouTube video per slide, just list the ID. starting at slide 1.
    std::vector<std::string> youtubeVideoId = {"OxqWjHD8nMU","riCzbUj_ow8","vGl0xCHPD58","zg9AgR8I7QI","_e6idatqS28"};
    std::string videoEmbeddedURL = "https://www.youtube.com/embed/" + youtubeVideoId.at(currentSlideNumber-1) + "?rel=0?ecver=2&autoplay=1";
    
    videoWebview = experimental::ui::WebView::create();
    videoWebview->setContentSize(Size(1920,1080));
    videoWebview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoWebview->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    videoWebview->loadURL(videoEmbeddedURL);
    videoLayer->addChild(videoWebview);
}

void OnlineSafetyDetailsLayer::setToCurrentSlideNumber()
{
    AnalyticsSingleton::getInstance()->settingsOnlineSafetySlideChangeEvent(currentSlideNumber);
    slideTitleLabel->setString(StringMgr::getInstance()->getStringForKey(StringUtils::format("%sAdult%d", ONLINE_SAFETY_SLIDE_TITLE,currentSlideNumber)));
    mainTextLabel->setString(StringMgr::getInstance()->getStringForKey(StringUtils::format("%sAdult%d", ONLINE_SAFETY_SLIDE_MAIN_TEXT,currentSlideNumber)));
    
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
