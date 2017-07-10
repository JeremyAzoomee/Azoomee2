#include "OnlineSafetyDetailsLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>

NS_AZOOMEE_BEGIN

#define MARGIN_TO_CHEVRON 150

Layer* OnlineSafetyDetailsLayer::createWithHeight(float setLayerHeight)
{
    auto layer = OnlineSafetyDetailsLayer::create();
    layer->layerHeight = setLayerHeight;
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
    
    Sprite* topGradient = Sprite::create("res/onlineSafetySlides/topGradient.png");
    topGradient->setAnchorPoint(Vec2(0, 1.0));
    topGradient->setScaleX(visibleSize.width / topGradient->getContentSize().width);
    topGradient->setPosition(0,backgroundLayer->getContentSize().height);
    //topGradient->setColor(Color3B::BLACK);
    backgroundLayer->addChild(topGradient);
}

void OnlineSafetyDetailsLayer::addUIObjects()
{
    titleLabel = createLabelWith("Keeping your family safe online", Style::Font::Regular, Style::Color::black, 70);
    titleLabel->setAnchorPoint(Vec2(0.0,1));
    titleLabel->setPosition(MARGIN_TO_CHEVRON*2,layerHeight - titleLabel->getContentSize().height);
    backgroundLayer->addChild(titleLabel);
    
    slideTitleLabel = createLabelWith("3/5  Be careful with personal information", Style::Font::Bold, Color3B(63,170,170), 70);
    slideTitleLabel->setAnchorPoint(Vec2(0.0,1));
    slideTitleLabel->setPosition(MARGIN_TO_CHEVRON*2,titleLabel->getPositionY() - slideTitleLabel->getContentSize().height*1.5);
    backgroundLayer->addChild(slideTitleLabel);
    
    mainImage = Sprite::create("res/onlineSafetySlides/safetyIll03.png");
    mainImage->setPosition(visibleSize.width-MARGIN_TO_CHEVRON*1.5-mainImage->getContentSize().width/2,slideTitleLabel->getPositionY()-slideTitleLabel->getContentSize().height*1.5- mainImage->getContentSize().height/2);
    backgroundLayer->addChild(mainImage);
    
    mainTextLabel = createLabelWith("It's important for you to talk to your children about personal information and what is and isn't ok to share online.\n\nChildren should not share personal information that is private on Azoomee. This includes passwords, full name, address, phone number, email and school name.\n\nChildren should also be reminded to 'share with care' and pause to think before sending something.", Style::Font::Regular, Style::Color::black, 59);
    mainTextLabel->setLineSpacing(15);
    mainTextLabel->setAnchorPoint(Vec2(0.0,1));
    mainTextLabel->setWidth(mainImage->getPositionX()-mainImage->getContentSize().width/2-MARGIN_TO_CHEVRON*2);
    mainTextLabel->setPosition(MARGIN_TO_CHEVRON*2,slideTitleLabel->getPositionY()-slideTitleLabel->getContentSize().height*2);
    backgroundLayer->addChild(mainTextLabel);
    
    chevronLeftButton = ElectricDreamsButton::createChevronLeftButton();
    chevronLeftButton->setCenterPosition(Vec2(MARGIN_TO_CHEVRON,mainImage->getPositionY()));
    backgroundLayer->addChild(chevronLeftButton);
    
    chevronRightButton = ElectricDreamsButton::createChevronRightButton();
    chevronRightButton->setCenterPosition(Vec2(visibleSize.width - MARGIN_TO_CHEVRON,mainImage->getPositionY()));
    backgroundLayer->addChild(chevronRightButton);
    
    watchSearchItUpButton = ElectricDreamsButton::createTextAsButtonWithColor("Watch Search It Up", 59, true, Color3B(63,170,170));
    watchSearchItUpButton->setCenterPosition(Vec2(mainImage->getPositionX(),mainImage->getPositionY()-mainImage->getContentSize().height/2-watchSearchItUpButton->getContentSize().height*2));
    backgroundLayer->addChild(watchSearchItUpButton);

    
}


//----------------------- Delegate Functions ----------------------------

void OnlineSafetyDetailsLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == watchSearchItUpButton)
    {

    }
}

NS_AZOOMEE_END
