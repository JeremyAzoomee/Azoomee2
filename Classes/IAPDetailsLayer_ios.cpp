#include "IAPDetailsLayer_ios.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "PrivacyAndTermsLayer.h"

USING_NS_CC;

#define FONT_MEDIUM "fonts/Sofia Pro Soft Medium.otf"
#define FONT_REGULAR "fonts/Sofia Pro Soft Regular.otf"
#define FONT_BOLD "fonts/Sofia Pro Soft Bold.otf"

#define COLOR_BRIGHT_AQUA Color3B(28, 244, 244)

bool IAPDetailsLayer_ios::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    createBackgroundLayer();
    createUpSaleLayer();
    addButtons();
    addTitle();
    addALLBulletsAndLabel();
    
    return true;
}

//---------------------- Create Background Layer -----------------------------

void IAPDetailsLayer_ios::createBackgroundLayer()
{
    backgroundLayer = LayerColor::create(Color4B(48,38,38,255),origin.x + visibleSize.width,origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void IAPDetailsLayer_ios::addListenerToBackgroundLayer()
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

void IAPDetailsLayer_ios::createUpSaleLayer()
{
    IAPDetailsLayer = LayerColor::create(Color4B::BLACK,visibleSize.width,visibleSize.width*0.6);
    IAPDetailsLayer->setPosition(origin.x + visibleSize.width/2 - IAPDetailsLayer->getContentSize().width/2,origin.y + visibleSize.height/2 - IAPDetailsLayer->getContentSize().height/2);
    
    auto fullscreenGlow = Sprite::create("res/decoration/fullscreen_glow.png");
    fullscreenGlow->setScaleX(IAPDetailsLayer->getContentSize().width / fullscreenGlow->getContentSize().width);
    fullscreenGlow->setScaleY(IAPDetailsLayer->getContentSize().height / fullscreenGlow->getContentSize().height);
    fullscreenGlow->setPosition(IAPDetailsLayer->getContentSize().width / 2, IAPDetailsLayer->getContentSize().height / 2);
    IAPDetailsLayer->addChild(fullscreenGlow);
    
    this->addChild(IAPDetailsLayer);
}

void IAPDetailsLayer_ios::addButtons()
{
    backButton = ElectricDreamsButton::createBackButton();
    backButton->setCenterPosition(Vec2(backButton->getContentSize().width*.7, IAPDetailsLayer->getContentSize().height - backButton->getContentSize().height*.9));
    backButton->setDelegate(this);
    IAPDetailsLayer->addChild(backButton);
    
    MarginSize = backButton->getContentSize().width * 1.4;
}

void IAPDetailsLayer_ios::addTitle()
{
    titleLabel = Label::createWithTTF("AZOOMEE PREMIUM SUBSCRIPTION", FONT_REGULAR, 130);
    titleLabel->setColor(COLOR_BRIGHT_AQUA);
    titleLabel->setAnchorPoint(Vec2(0,0.5));
    titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    
    titleLabel->setPosition(backButton->getContentSize().width*1.3, backButton->getPositionY() + backButton->getContentSize().height/2);
    IAPDetailsLayer->addChild(titleLabel);
}

void IAPDetailsLayer_ios::addALLBulletsAndLabel()
{
    float yPosition = IAPDetailsLayer->getContentSize().height- MarginSize * 1.2;
    
    addBulletAndLabel("Unlimited access for £4.99 per month to everything while subscribed", yPosition);
    addBulletAndLabel("Payment will be charged to iTunes Account at confirmation of purchase",yPosition -MarginSize*.4);
    addBulletAndLabel("Subscription automatically renews each month unless auto-renew is turned off at least 24-hours before the end of the current period", yPosition -MarginSize*.8);
    addBulletAndLabel("Your account will be charged for renewal within 24-hours prior to the end of the current period at £4.99", yPosition -MarginSize*1.3);
    addBulletAndLabel("You can manage your subscription and turn auto-renewal on or off by going to your App Store Account Settings after purchase", yPosition -MarginSize*1.8);
    addBulletAndLabel("Any unused portion of a free trial period, if offered, will be forfeited when you purchase a subscription, where applicable", yPosition -MarginSize*2.3);
    
    Label* privacyTitle = Label::createWithTTF("PRIVACY & SAFETY", FONT_REGULAR, 80);
    privacyTitle->setColor(COLOR_BRIGHT_AQUA);
    privacyTitle->setHorizontalAlignment(TextHAlignment::LEFT);
    privacyTitle->setAnchorPoint(Vec2(0, 1));
    privacyTitle->setWidth(IAPDetailsLayer->getContentSize().width*.8);
    privacyTitle->setPosition(Vec2(MarginSize/2,yPosition-MarginSize*3));
    IAPDetailsLayer->addChild(privacyTitle);
    
    Label* privacyBody = Label::createWithTTF("Azoomee values your child’s privacy and safety. We do not share or sell you or your child's personal information with 3rd parties, and we do not serve any advertising.", FONT_REGULAR, 50);
    privacyBody->setColor(Color3B::WHITE);
    privacyBody->setHorizontalAlignment(TextHAlignment::LEFT);
    privacyBody->setAnchorPoint(Vec2(0, 1));
    privacyBody->setWidth(IAPDetailsLayer->getContentSize().width*.9);
    privacyBody->setPosition(Vec2(MarginSize/2,yPosition-MarginSize*3.3));
    IAPDetailsLayer->addChild(privacyBody);
    
    Label* privacyLinks = Label::createWithTTF("Please visit the Azoomee website to read our", FONT_REGULAR, 50);
    privacyLinks->setColor(Color3B::WHITE);
    privacyLinks->setHorizontalAlignment(TextHAlignment::LEFT);
    privacyLinks->setAnchorPoint(Vec2(0, 0));
    privacyLinks->setPosition(Vec2(MarginSize/2,yPosition-MarginSize*4));
    IAPDetailsLayer->addChild(privacyLinks);
    
    Layer* privacyAndTerms = PrivacyAndTermsLayer::create();
    privacyAndTerms->setPosition(privacyLinks->getPositionX() + privacyLinks->getContentSize().width + 15,privacyLinks->getPositionY());
    IAPDetailsLayer->addChild(privacyAndTerms);
    
}

void IAPDetailsLayer_ios::addBulletAndLabel(std::string regularText, float yPosition)
{
    Sprite* bullet = Sprite::create("res/startATrialScene/bulletTick.png");
    bullet->setAnchorPoint(Vec2(0.5, 1));
    bullet->setPosition(MarginSize/2 + bullet->getContentSize().width/2, yPosition);
    IAPDetailsLayer->addChild(bullet);
    
    Label* newLabel = Label::createWithTTF(regularText, FONT_REGULAR, 50);
    newLabel->setColor(Color3B::WHITE);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    newLabel->setAnchorPoint(Vec2(0, 1));
    newLabel->setWidth(IAPDetailsLayer->getContentSize().width*.8);
    newLabel->setPosition(Vec2(bullet->getPositionX() + bullet->getContentSize().width,yPosition));
    IAPDetailsLayer->addChild(newLabel);
}

//----------------------- Delegate Functions ----------------------------

void IAPDetailsLayer_ios::buttonPressed(ElectricDreamsButton* button)
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}
