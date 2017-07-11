#include "OnlineSafetyDetailsLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>

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

//----------------Add UI Objects-------------

void OnlineSafetyDetailsLayer::createBackground()
{
    backgroundLayer = LayerColor::create(Color4B(216,216,216,255),visibleSize.width, layerHeight);
    this->addChild(backgroundLayer);
    
    /*Sprite* topGradient = Sprite::create("res/onlineSafetySlides/topGradient.png");
    topGradient->setAnchorPoint(Vec2(0, 1.0));
    topGradient->setScaleX(visibleSize.width / topGradient->getContentSize().width);
    topGradient->setPosition(0,backgroundLayer->getContentSize().height);
    //topGradient->setColor(Color3B::BLACK);
    backgroundLayer->addChild(topGradient);*/
}

void OnlineSafetyDetailsLayer::addUIObjects()
{
    titleLabel = createLabelWith("Keeping your family safe online", Style::Font::Regular, Style::Color::black, 70);
    titleLabel->setAnchorPoint(Vec2(0.0,1));
    titleLabel->setPosition(MARGIN_TO_CHEVRON*2,layerHeight - titleLabel->getContentSize().height);
    backgroundLayer->addChild(titleLabel);
    
    slideTitleLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(StringUtils::format("%s%d", ONLINE_SAFETY_SLIDE_TITLE,currentSlideNumber)), Style::Font::Bold, Color3B(9,154,154), 70);
    slideTitleLabel->setAnchorPoint(Vec2(0.0,1));
    slideTitleLabel->setPosition(MARGIN_TO_CHEVRON*2,titleLabel->getPositionY() - slideTitleLabel->getContentSize().height*1.5);
    backgroundLayer->addChild(slideTitleLabel);
    
    mainImage = Sprite::create(StringUtils::format("res/onlineSafetySlides/safetyIll0%d.png",currentSlideNumber));
    mainImage->setPosition(visibleSize.width-MARGIN_TO_CHEVRON*1.5-mainImage->getContentSize().width/2,slideTitleLabel->getPositionY()-slideTitleLabel->getContentSize().height*1.5- mainImage->getContentSize().height/2);
    backgroundLayer->addChild(mainImage);
    
    mainTextLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(StringUtils::format("%s%d", ONLINE_SAFETY_SLIDE_MAIN_TEXT,currentSlideNumber)), Style::Font::Regular, Style::Color::black, 59);
    mainTextLabel->setLineSpacing(15);
    mainTextLabel->setAnchorPoint(Vec2(0.0,1));
    mainTextLabel->setWidth(mainImage->getPositionX()-mainImage->getContentSize().width/2-MARGIN_TO_CHEVRON*2);
    mainTextLabel->setPosition(MARGIN_TO_CHEVRON*2,slideTitleLabel->getPositionY()-slideTitleLabel->getContentSize().height*2);
    backgroundLayer->addChild(mainTextLabel);
    
    chevronLeftButton = ElectricDreamsButton::createChevronLeftButton();
    chevronLeftButton->setCenterPosition(Vec2(MARGIN_TO_CHEVRON,mainImage->getPositionY()));
    chevronLeftButton->setDelegate(this);
    backgroundLayer->addChild(chevronLeftButton);
    
    chevronRightButton = ElectricDreamsButton::createChevronRightButton();
    chevronRightButton->setCenterPosition(Vec2(visibleSize.width - MARGIN_TO_CHEVRON,mainImage->getPositionY()));
    chevronRightButton->setDelegate(this);
    backgroundLayer->addChild(chevronRightButton);
    
    watchSearchItUpButton = ElectricDreamsButton::createTextAsButtonWithColor("Watch Search It Up", 59, true, Color3B(9,154,154));
    watchSearchItUpButton->setCenterPosition(Vec2(mainImage->getPositionX(),mainImage->getPositionY()-mainImage->getContentSize().height/2-watchSearchItUpButton->getContentSize().height*2));
    backgroundLayer->addChild(watchSearchItUpButton);
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
    {

    }
    else if(button == chevronLeftButton)
    {
        moveSlideNumberBy(-1);
    }
    else if(button == chevronRightButton)
    {
        moveSlideNumberBy(1);
    }
}

NS_AZOOMEE_END
