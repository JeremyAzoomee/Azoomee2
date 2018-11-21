#include "MessageBoxOnlineSafetySlidesLayer.h"
#include "../../Strings.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../ElectricDreamsTextStyles.h"
#include "../ElectricDreamsDecoration.h"

using namespace cocos2d;

#define MESSAGE_BOX_PADDING 100

namespace Azoomee
{
    
Layer* MessageBoxOnlineSafetySlidesLayer::createForParent(Layer* parentLayer)
{
    auto layer = MessageBoxOnlineSafetySlidesLayer::create();
    
    layer->isParent=true;
    layer->slideTitle = "titleAdult";
    layer->slideMainText = "mainTextAdult";
    layer->videoIDs = {"OxqWjHD8nMU","riCzbUj_ow8","vGl0xCHPD58","zg9AgR8I7QI","_e6idatqS28"};
    layer->titleFontSize = 70;
    layer->mainTextFontSize = 59;
    layer->textLineSpacing =20;
    layer->totalSlides = 5;
    layer->_parentLayer = parentLayer;
    layer->currentSlideNumber = 1;
    layer->onSizeChanged();
    
    return layer;
}
    
Layer* MessageBoxOnlineSafetySlidesLayer::createForChild(Layer* parentLayer)
{
    auto layer = MessageBoxOnlineSafetySlidesLayer::create();
    
    layer->totalSlides = 4;
    layer->slideTitle = "titleChild";
	layer->slideMainText = "mainTextChild";
    layer->videoIDs = {"14eaf3c0-c36c-4df5-a857-05f40c00884c","50957646-27d2-4723-921d-52c84e3396df","4ebd6de5-a75d-4d38-b28f-811900174b81","ed23035d-fea4-440e-a8d2-f5a33da4e794"};
    layer->titleFontSize = 90;
    layer->mainTextFontSize = 70;
    layer->textLineSpacing =30;
    layer->_parentLayer = parentLayer;
    layer->currentSlideNumber = 1;
    layer->onSizeChanged();
    
    return layer;
}

bool MessageBoxOnlineSafetySlidesLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    return true;
}

//---------------------- Message Box Functions------------------------
  
void MessageBoxOnlineSafetySlidesLayer::addUIObjects()
{
    //-------------MAIN TITLE --------------
    if(isParent)
        titleLabel = createLabelWith(_("Keeping your family safe online"), Style::Font::Regular(), Style::Color::black, 70);
    else
        titleLabel = createLabelWith("", Style::Font::Regular(), Style::Color::black, 5);
    
    //------------SLIDE TITLE--------------------
    slideTitleLabel = createLabelWith(_(StringUtils::format("%s%d", slideTitle.c_str(),currentSlideNumber)), Style::Font::Bold(), Style::Color::safetySlideTitleColor, titleFontSize);
    
    //-----------SLIDE IMAGE--------------------
    mainImage = Sprite::create(StringUtils::format("res/onlineSafetySlides/Oomees_SettingsPage-0%d.png",currentSlideNumber));
    
    //-----------SLIDE MAIN TEXT--------------------
    mainTextLabel = createLabelWith(_(StringUtils::format("%s%d", slideMainText.c_str(),currentSlideNumber)), Style::Font::Regular(), Style::Color::black, mainTextFontSize);
    mainTextLabel->setLineSpacing(textLineSpacing);
    
    //-----------SLIDE NATVIATION--------------------
    // Location in relation to the Image Location
    
    chevronLeftButton = ElectricDreamsButton::createChevronLeftButton();
    chevronLeftButton->setDelegate(this);
    chevronLeftButton->setMixPanelButtonName("MessageBox-OnlineSafety-LeftChevron");
    
    chevronRightButton = ElectricDreamsButton::createChevronRightButton();
    chevronRightButton->setDelegate(this);
    chevronRightButton->setMixPanelButtonName("MessageBox-OnlineSafety-RighChevron");
    
    /*watchSearchItUpButton = ElectricDreamsButton::createTextAsButtonWithColor(_(ONLINE_SAFETY_BUTTON_TEXT), 59, true, Style::Color::safetySlideTitleColor);
    watchSearchItUpButton->setCenterPosition(Vec2(mainImage->getPositionX(),mainImage->getPositionY()-mainImage->getContentSize().height/2-watchSearchItUpButton->getContentSize().height*2));
    watchSearchItUpButton->setDelegate(this);
    watchSearchItUpButton->setMixPanelButtonName("MessageBox-OnlineSafety-watchSearchItUp");*/
}

void MessageBoxOnlineSafetySlidesLayer::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWhiteWindowCloselButton();
    cancelButton->setMixPanelButtonName("messageBoxCancelButton");
    cancelButton->setDelegate(this);
}
    
void MessageBoxOnlineSafetySlidesLayer::setToCurrentSlideNumber()
{
    AnalyticsSingleton::getInstance()->settingsOnlineSafetySlideChangeEvent(currentSlideNumber);
    slideTitleLabel->setString(_(StringUtils::format("%s%d", slideTitle.c_str(),currentSlideNumber)));
    mainTextLabel->setString(_(StringUtils::format("%s%d", slideMainText.c_str(),currentSlideNumber)));
    
    mainImage->setTexture(StringUtils::format("res/onlineSafetySlides/Oomees_SettingsPage-0%d.png",currentSlideNumber));
}

void MessageBoxOnlineSafetySlidesLayer::moveSlideNumberBy(int moveBy)
{
    currentSlideNumber = currentSlideNumber + moveBy;
    
    if(currentSlideNumber > totalSlides)
        currentSlideNumber = 1;
    else if(currentSlideNumber < 1)
        currentSlideNumber = totalSlides;
    
    setToCurrentSlideNumber();
}
    
void MessageBoxOnlineSafetySlidesLayer::createMessageWindow()
{
    windowLayer = createWhiteWindowLayer(currentRunningSceneSize.width - MESSAGE_BOX_PADDING*2, currentRunningSceneSize.height -MESSAGE_BOX_PADDING*2);
    windowLayer->setPosition(currentRunningSceneSize.width/2- windowLayer->getContentSize().width/2,currentRunningSceneSize.height/2-windowLayer->getContentSize().height/2);
    this->addChild(windowLayer);
}
    
//------------- LANDSCAPE SPECIFIC CREATION-------------

void MessageBoxOnlineSafetySlidesLayer::addObjectsToWindowLandscape()
{
    float nextItemHeight = windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75;
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, nextItemHeight));
    windowLayer->addChild(cancelButton);
    
    // Add Title
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height/2 - titleLabel->getContentSize().height;
    titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    titleLabel->setAnchorPoint(Vec2(0.0,0.0));
    titleLabel->setPosition(MESSAGE_BOX_PADDING*2, nextItemHeight);
    titleLabel->setWidth(windowLayer->getContentSize().width);
    windowLayer->addChild(titleLabel);

    // Add Slide Title
    nextItemHeight = nextItemHeight-titleLabel->getContentSize().height/2 - slideTitleLabel->getContentSize().height;
    slideTitleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    slideTitleLabel->setAnchorPoint(Vec2(0.0,0.0));
    slideTitleLabel->setPosition(MESSAGE_BOX_PADDING*2, nextItemHeight);
    slideTitleLabel->setWidth(windowLayer->getContentSize().width);
    windowLayer->addChild(slideTitleLabel);
    
    // Add Slide Title
    nextItemHeight = nextItemHeight-slideTitleLabel->getContentSize().height/2;
    mainTextLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    mainTextLabel->setAnchorPoint(Vec2(0.0,1.0));
    mainTextLabel->setPosition(MESSAGE_BOX_PADDING*2, nextItemHeight);
    mainTextLabel->setWidth(windowLayer->getContentSize().width/2);
    windowLayer->addChild(mainTextLabel);
    
    // Add OomeeImage
    mainImage->setPosition(windowLayer->getContentSize().width - MESSAGE_BOX_PADDING - mainImage->getContentSize().width/2,windowLayer->getContentSize().height/2);
    windowLayer->addChild(mainImage);
    
    // Add Left chevron
    chevronLeftButton->setCenterPosition(Vec2(MESSAGE_BOX_PADDING,windowLayer->getContentSize().height/2));
    windowLayer->addChild(chevronLeftButton);
    
    // Add right chevron
    chevronRightButton->setCenterPosition(Vec2(windowLayer->getContentSize().width - MESSAGE_BOX_PADDING,windowLayer->getContentSize().height/2));
    windowLayer->addChild(chevronRightButton);
    
}
    
//------------- PORTRAIT SPECIFIC CREATION-------------
    
void MessageBoxOnlineSafetySlidesLayer::addObjectsToWindowPortrait()
{
    float nextItemHeight = windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75;
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, nextItemHeight));
    windowLayer->addChild(cancelButton);
    
    // Add Title
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height/2;
    titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    titleLabel->setAnchorPoint(Vec2(0.5,1.0));
    titleLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    titleLabel->setWidth(windowLayer->getContentSize().width - MESSAGE_BOX_PADDING*2);
    windowLayer->addChild(titleLabel);
    
    // Add Slide Title
    nextItemHeight = nextItemHeight-titleLabel->getContentSize().height - MESSAGE_BOX_PADDING/2;
    slideTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    slideTitleLabel->setAnchorPoint(Vec2(0.5,1.0));
    slideTitleLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    slideTitleLabel->setWidth(windowLayer->getContentSize().width - MESSAGE_BOX_PADDING*2);
    windowLayer->addChild(slideTitleLabel);
    
    // Add OomeeImage
    nextItemHeight = nextItemHeight- slideTitleLabel->getContentSize().height - MESSAGE_BOX_PADDING/2 - mainImage->getContentSize().height/2;
    mainImage->setPosition(windowLayer->getContentSize().width/2 ,nextItemHeight);
    windowLayer->addChild(mainImage);
    
    // Add Slide Title
    nextItemHeight = nextItemHeight-mainImage->getContentSize().height/2 - MESSAGE_BOX_PADDING;
    mainTextLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    mainTextLabel->setAnchorPoint(Vec2(0.5,1.0));
    mainTextLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    mainTextLabel->setWidth(windowLayer->getContentSize().width - MESSAGE_BOX_PADDING*2);
    windowLayer->addChild(mainTextLabel);
    
    // Add Left chevron
    chevronLeftButton->setCenterPosition(Vec2(MESSAGE_BOX_PADDING,mainImage->getPositionY()));
    windowLayer->addChild(chevronLeftButton);
    
    // Add right chevron
    chevronRightButton->setCenterPosition(Vec2(windowLayer->getContentSize().width - MESSAGE_BOX_PADDING,mainImage->getPositionY()));
    windowLayer->addChild(chevronRightButton);
}

//---------------------- Actions -----------------
    
void MessageBoxOnlineSafetySlidesLayer::onSizeChanged()
{
    setCurrentRunningSceneSize();
    
    if(windowLayer)
        windowLayer->removeAllChildren();
    
    createCancelButton();
    addUIObjects();
    
    createMessageWindow();
    
    if(currentRunningSceneSize.width>currentRunningSceneSize.height)
    {
        addObjectsToWindowLandscape();
    }
    else
    {
        addObjectsToWindowPortrait();
    }
}

//----------------------- Delegate Functions ----------------------------

void MessageBoxOnlineSafetySlidesLayer::buttonPressed(ElectricDreamsButton* button)
{
    /*if(button == watchSearchItUpButton)
        playVideo();
    else*/ if(button == chevronLeftButton)
        moveSlideNumberBy(-1);
    else if(button == chevronRightButton)
        moveSlideNumberBy(1);
    else if(button == cancelButton)
        dynamic_cast<MessageBox*>(_parentLayer)->sendDelegateMessageBoxButtonPressed("OnlineSafetySlideLayer", MessageBox::kCancel);
}
    
}
