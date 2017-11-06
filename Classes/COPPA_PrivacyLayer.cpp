#include "COPPA_PrivacyLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "PrivacyAndTermsLayer.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool COPPA_PrivacyLayer::init()
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

void COPPA_PrivacyLayer::createBackgroundLayer()
{
    backgroundLayer = LayerColor::create(Style::Color_4B::letterBoxSlideBackground,origin.x + visibleSize.width,origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void COPPA_PrivacyLayer::addListenerToBackgroundLayer()
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

void COPPA_PrivacyLayer::createUpSaleLayer()
{
    detailsLayer = LayerColor::create(Color4B::BLACK,visibleSize.width,visibleSize.width * 0.6f);
    detailsLayer->setPosition(origin.x + visibleSize.width/2 - detailsLayer->getContentSize().width/2,origin.y + visibleSize.height/2 - detailsLayer->getContentSize().height/2);
    
    auto fullscreenGlow = Sprite::create("res/decoration/fullscreen_glow.png");
    fullscreenGlow->setScaleX(detailsLayer->getContentSize().width / fullscreenGlow->getContentSize().width);
    fullscreenGlow->setScaleY(detailsLayer->getContentSize().height / fullscreenGlow->getContentSize().height);
    fullscreenGlow->setPosition(detailsLayer->getContentSize().width / 2, detailsLayer->getContentSize().height / 2);
    detailsLayer->addChild(fullscreenGlow);
    
    this->addChild(detailsLayer);
}

void COPPA_PrivacyLayer::addButtons()
{
    backButton = ElectricDreamsButton::createBackButton();
    backButton->setCenterPosition(Vec2(backButton->getContentSize().width*.7, detailsLayer->getContentSize().height - backButton->getContentSize().height * 0.9f));
    backButton->setDelegate(this);
    backButton->setMixPanelButtonName("IAPDetailsLayerBackButton");
    detailsLayer->addChild(backButton);
    
    MarginSize = backButton->getContentSize().width * 1.4f;
}

void COPPA_PrivacyLayer::addTitle()
{
    titleLabel = Label::createWithTTF("Azoomee subscription and your child’s privacy", Style::Font::Regular, 100);
    titleLabel->setColor(Style::Color::brightAqua);
    titleLabel->setAnchorPoint(Vec2(0.0f , 0.5f));
    titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    
    titleLabel->setPosition(backButton->getContentSize().width * 1.3f, backButton->getPositionY() + backButton->getContentSize().height/2);
    detailsLayer->addChild(titleLabel);
}

void COPPA_PrivacyLayer::addALLBulletsAndLabel()
{
    float yPosition = detailsLayer->getContentSize().height- MarginSize * 1.2f;
    
    addBulletAndLabel("Purchasing an Azoomee subscription allows children to use Azoomee's messaging service (Chat). Chat allows parents and their children to send text messages, emojis, pre-made stickers, and other images to other Azoomee users.", yPosition);
    addBulletAndLabel("By using the Chat service, it may be possible for a child to share personal information including his or her name, location, images, or other contact details.",yPosition -MarginSize * 0.8f);
     addBulletAndLabel("Parental consent is required before a child can access Chat. By purchasing an Azoomee subscription, you are providing your consent as a parent or guardian for children to use Azoomee's messaging service (Chat).",yPosition -MarginSize * 1.4f);
    
    Label* privacyTitle = Label::createWithTTF("PRIVACY & SAFETY", Style::Font::Regular, 80);
    privacyTitle->setColor(Style::Color::brightAqua);
    privacyTitle->setHorizontalAlignment(TextHAlignment::LEFT);
    privacyTitle->setAnchorPoint(Vec2(0, 1));
    privacyTitle->setWidth(detailsLayer->getContentSize().width * 0.8f);
    privacyTitle->setPosition(Vec2(MarginSize/2,yPosition-MarginSize * 2.5f));
    detailsLayer->addChild(privacyTitle);
    
    Label* privacyBody = Label::createWithTTF("Azoomee values your child’s privacy and safety. We do not share or sell you or your child's personal information with 3rd parties, and we do not serve any advertising.", Style::Font::Regular, 50);
    privacyBody->setColor(Color3B::WHITE);
    privacyBody->setHorizontalAlignment(TextHAlignment::LEFT);
    privacyBody->setAnchorPoint(Vec2(0, 1));
    privacyBody->setWidth(detailsLayer->getContentSize().width * 0.9f);
    privacyBody->setPosition(Vec2(MarginSize/2,yPosition-MarginSize * 3.0f));
    detailsLayer->addChild(privacyBody);
    
    Label* privacyLinks = Label::createWithTTF("Please visit the Azoomee website to read our", Style::Font::Regular, 50);
    privacyLinks->setColor(Color3B::WHITE);
    privacyLinks->setHorizontalAlignment(TextHAlignment::LEFT);
    privacyLinks->setAnchorPoint(Vec2(0, 0));
    privacyLinks->setPosition(Vec2(MarginSize/2,yPosition-MarginSize * 3.7f));
    detailsLayer->addChild(privacyLinks);
    
    Layer* privacyAndTerms = PrivacyAndTermsLayer::createWhite();
    privacyAndTerms->setPosition(privacyLinks->getPositionX() + privacyLinks->getContentSize().width + 15,privacyLinks->getPositionY());
    detailsLayer->addChild(privacyAndTerms);
}

void COPPA_PrivacyLayer::addBulletAndLabel(std::string regularText, float yPosition)
{
    Sprite* bullet = Sprite::create("res/startATrialScene/bulletTick.png");
    bullet->setAnchorPoint(Vec2(0.5, 1));
    bullet->setPosition(MarginSize/2 + bullet->getContentSize().width/2, yPosition);
    detailsLayer->addChild(bullet);
    
    Label* newLabel = Label::createWithTTF(regularText, Style::Font::Regular, 50);
    newLabel->setColor(Color3B::WHITE);
    newLabel->setLineSpacing(16);
    newLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    newLabel->setAnchorPoint(Vec2(0, 1));
    newLabel->setWidth(detailsLayer->getContentSize().width * 0.8f);
    newLabel->setPosition(Vec2(bullet->getPositionX() + bullet->getContentSize().width,yPosition));
    detailsLayer->addChild(newLabel);
}

//----------------------- Delegate Functions ----------------------------

void COPPA_PrivacyLayer::buttonPressed(ElectricDreamsButton* button)
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}

NS_AZOOMEE_END
