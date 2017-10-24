#include "IAPUpsaleLayer.h"
#include "cocos/ui/UIRichText.h"
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include "RoutePaymentSingleton.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include "IAPDetailsLayer_ios.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/UI/Style.h>
#include "COPPA_PrivacyLayer.h"

using namespace cocos2d;

#define SIDE_MARGIN_SIZE 130

NS_AZOOMEE_BEGIN

Layer* IAPUpsaleLayer::createRequiresPin()
{
    auto layer = IAPUpsaleLayer::create();
    layer->requiresPinCode = true;
    
    return layer;
}

bool IAPUpsaleLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createBackgroundLayer();
    createUpSaleLayer();
    addImage();
    addTitle();
    addALLBulletsAndLabel();
    addButtons();
    
    return true;
}

//---------------------- Create Background Layer -----------------------------

void IAPUpsaleLayer::createBackgroundLayer()
{
    backgroundLayer = LayerColor::create(Style::Color_4B::letterBoxSlideBackground,origin.x + visibleSize.width,origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void IAPUpsaleLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

//--------------------- CREATE UPSALES LAYER-------------------------

void IAPUpsaleLayer::createUpSaleLayer()
{
    UpsaleLayer = LayerColor::create(Color4B::BLACK,visibleSize.width,visibleSize.width*0.6);
    UpsaleLayer->setPosition(origin.x + visibleSize.width/2 - UpsaleLayer->getContentSize().width/2,origin.y + visibleSize.height/2 - UpsaleLayer->getContentSize().height/2);
    
    auto fullscreenGlow = Sprite::create("res/decoration/fullscreen_glow.png");
    fullscreenGlow->setScaleX(UpsaleLayer->getContentSize().width / fullscreenGlow->getContentSize().width);
    fullscreenGlow->setScaleY(UpsaleLayer->getContentSize().height / fullscreenGlow->getContentSize().height);
    fullscreenGlow->setPosition(UpsaleLayer->getContentSize().width / 2, UpsaleLayer->getContentSize().height / 2);
    UpsaleLayer->addChild(fullscreenGlow);

    this->addChild(UpsaleLayer);
}

void IAPUpsaleLayer::addImage()
{
    oomeesImage = Sprite::create("res/startATrialScene/oomees.png");
    oomeesImage->setPosition(UpsaleLayer->getContentSize().width - SIDE_MARGIN_SIZE - oomeesImage->getContentSize().width/2, UpsaleLayer->getContentSize().height *.6);
    UpsaleLayer->addChild(oomeesImage);
}

void IAPUpsaleLayer::addTitle()
{
    titleLabel = Label::createWithTTF("Start a FREE trial. Cancel anytime.", Style::Font::Regular, 130);
    titleLabel->setColor(Style::Color::brightAqua);
    titleLabel->setAnchorPoint(Vec2(0,0.5));
    titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    
    float GapFromOomeeImage = (UpsaleLayer->getContentSize().height - (oomeesImage->getPositionY() + oomeesImage->getContentSize().height/2))*.33;
    
    titleLabel->setPosition(SIDE_MARGIN_SIZE, oomeesImage->getPositionY() + oomeesImage->getContentSize().height/2 + GapFromOomeeImage);
    UpsaleLayer->addChild(titleLabel);
}

void IAPUpsaleLayer::addButtons()
{
    float GapFromOomeeImage = (oomeesImage->getPositionY() - oomeesImage->getContentSize().height/2)/2;
    
    startTrialButton = ElectricDreamsButton::createButtonWithText("Start your free 7-day trial", 300);
    startTrialButton->setPosition(UpsaleLayer->getContentSize().width - SIDE_MARGIN_SIZE - startTrialButton->getContentSize().width, GapFromOomeeImage);
    startTrialButton->setDelegate(this);
    startTrialButton->setMixPanelButtonName("IAPUpsaleSceneStartTrialButton");
    UpsaleLayer->addChild(startTrialButton);
    
    notNowButton = ElectricDreamsButton::createTextAsButton("Not Now", 59, true);
    notNowButton->setPosition(bulletTextXposition, startTrialButton->getCenterPosition().y - notNowButton->getContentSize().height/2);
    notNowButton->setDelegate(this);
    notNowButton->setMixPanelButtonName("IAPUpsaleSceneNotNowButton");
    UpsaleLayer->addChild(notNowButton);
    
    addOptionalSubscriptionLabel();
    
    privacyButton = ElectricDreamsButton::createTextAsButtonAqua("Your child's privacy", 40, true);
    privacyButton->setPosition(startTrialButton->getCenterPosition().x-privacyButton->getContentSize().width/2, privacyButton->getContentSize().height*2);
    privacyButton->setDelegate(this);
    privacyButton->setMixPanelButtonName("IAPUpsaleSceneChildPrivacyButton");
    UpsaleLayer->addChild(privacyButton);
    
    if(RoutePaymentSingleton::getInstance()->osIsIos())
    {
        learnMoreButton = ElectricDreamsButton::createTextAsButtonAqua("Learn more about your subscription", 40, true);
        learnMoreButton->setDelegate(this);
        learnMoreButton->setMixPanelButtonName("IAPUpsaleSceneLearnMoreButton");
        UpsaleLayer->addChild(learnMoreButton);
        
        restoreButton = ElectricDreamsButton::createTextAsButtonAqua("Restore your purchase", 40, true);
        restoreButton->setDelegate(this);
        restoreButton->setMixPanelButtonName("IAPUpsaleSceneRestoreButton");
        UpsaleLayer->addChild(restoreButton);
        
        float buttonSpacing = 40.0f;
        
        float totalWidthOfExtraButtons = learnMoreButton->getContentSize().width + restoreButton->getContentSize().width + privacyButton->getContentSize().width + buttonSpacing * 2;
        
        learnMoreButton->setPosition(startTrialButton->getCenterPosition().x + totalWidthOfExtraButtons/2 - learnMoreButton->getContentSize().width, learnMoreButton->getContentSize().height*2);
        restoreButton->setPosition(learnMoreButton->getPositionX()-restoreButton->getContentSize().width -buttonSpacing,learnMoreButton->getPositionY());
        privacyButton->setPosition(restoreButton->getPositionX() - privacyButton->getContentSize().width - buttonSpacing, learnMoreButton->getPositionY());
    }
}

void IAPUpsaleLayer::addALLBulletsAndLabel()
{
    addBulletAndLabel("New ", "videos & games added every week.", oomeesImage->getPositionY() + oomeesImage->getContentSize().height/4);
    addBulletAndLabel("A library ", "of the best kids’ entertainment,", oomeesImage->getPositionY());
    
    Label* secondLine = Label::createWithTTF("literally 1000s of things to do!", Style::Font::Regular, 80);
    secondLine->setColor(Color3B::WHITE);
    secondLine->setPosition(bulletTextXposition+secondLine->getContentSize().width/2, oomeesImage->getPositionY() - secondLine->getContentSize().height);
    UpsaleLayer->addChild(secondLine);
    
    addBulletAndLabel("Safe, ", "age-appropriate & 100% ad-free.", oomeesImage->getPositionY() - oomeesImage->getContentSize().height/4- secondLine->getContentSize().height);
}

void IAPUpsaleLayer::addBulletAndLabel(std::string BOLDtext, std::string regularText, float yPosition)
{
    Sprite* bullet = Sprite::create("res/startATrialScene/bulletTick.png");
    bullet->setPosition(SIDE_MARGIN_SIZE + bullet->getContentSize().width/2, yPosition);
    UpsaleLayer->addChild(bullet);
    
    bulletTextXposition = bullet->getPositionX() + bullet->getContentSize().width;
    
    cocos2d::ui::RichText* richTextLabel = cocos2d::ui::RichText::create();
    richTextLabel->setAnchorPoint(Vec2(0,0.5));
    
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, BOLDtext, Style::Font::Bold, 80));
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, regularText, Style::Font::Regular, 80));
    richTextLabel->setPosition(Vec2(bulletTextXposition,yPosition));
    UpsaleLayer->addChild(richTextLabel);
}

void IAPUpsaleLayer::addOptionalSubscriptionLabel()
{
    optionalLabel = Label::createWithTTF("Then £4.99/month. No commitment, cancel anytime.", Style::Font::Regular, 40);
    optionalLabel->setColor(Style::Color::brightAqua);
    optionalLabel->setAnchorPoint(Vec2(0.5,0.5));
    optionalLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    optionalLabel->setPosition(startTrialButton->getPositionX()+startTrialButton->getContentSize().width/2, startTrialButton->getPositionY() - optionalLabel->getContentSize().height*1.5);
    UpsaleLayer->addChild(optionalLabel);
}

//---------------------- Actions -----------------

void IAPUpsaleLayer::removeSelf()
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}

//----------------------- Delegate Functions ----------------------------

void IAPUpsaleLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == startTrialButton)
    {
        if(requiresPinCode)
        {
            restoreButtonPressed = false;
            askForPin();
        }
        else
            RoutePaymentSingleton::getInstance()->startInAppPayment();
    }
    else if(button == notNowButton)
    {
        AudioMixer::getInstance()->resumeBackgroundMusic();
        removeSelf();
    }
    else if(button == restoreButton)
    {
        if(requiresPinCode)
        {
            restoreButtonPressed = true;
            askForPin();
        }
        else
            RoutePaymentSingleton::getInstance()->refreshAppleReceiptFromButton();
    }
    else if(button == learnMoreButton)
    {
        IAPDetailsLayer_ios::create();
    }
    else if(button == privacyButton)
    {
        COPPA_PrivacyLayer::create();
    }
}

//----------------- PIN CODE FUNCTIONS--------------------

void IAPUpsaleLayer::askForPin()
{
    AwaitingAdultPinLayer::create()->setDelegate(this);
}

void IAPUpsaleLayer::AdultPinCancelled(AwaitingAdultPinLayer* layer)
{
}

void IAPUpsaleLayer::AdultPinAccepted(AwaitingAdultPinLayer* layer)
{
    if(restoreButtonPressed)
        RoutePaymentSingleton::getInstance()->refreshAppleReceiptFromButton();
    else
        RoutePaymentSingleton::getInstance()->startInAppPayment();
}

NS_AZOOMEE_END
