#include "IAPUpsaleLayer.h"
#include "cocos/ui/UIRichText.h"
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include "PaymentSingleton.h"
#include "MessageBox.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>

USING_NS_CC;
using namespace Azoomee;


#define SIDE_MARGIN_SIZE 130

#define FONT_MEDIUM "fonts/Sofia Pro Soft Medium.otf"
#define FONT_REGULAR "fonts/Sofia Pro Soft Regular.otf"
#define FONT_BOLD "fonts/Sofia Pro Soft Bold.otf"

#define COLOR_BRIGHT_AQUA Color3B(28, 244, 244)

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
    
    createBackGroundLayer();
    createUpSaleLayer();
    addImage();
    addTitle();
    addButtons();
    addALLBulletsAndLabel();
    addOptionalSubscriptionLabel();
    
    return true;
}

//---------------------- Create BackGround Layer -----------------------------

void IAPUpsaleLayer::createBackGroundLayer()
{
    backgroundLayer = LayerColor::create(Color4B(48,38,38,255),origin.x + visibleSize.width,origin.y + visibleSize.height);
    
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
    titleLabel = Label::createWithTTF("Start a trial to unlock full access", FONT_REGULAR, 130);
    titleLabel->setColor(COLOR_BRIGHT_AQUA);
    titleLabel->setAnchorPoint(Vec2(0,0.5));
    titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    
    float GapFromOomeeImage = (UpsaleLayer->getContentSize().height - (oomeesImage->getPositionY() + oomeesImage->getContentSize().height/2))*.33;
    
    titleLabel->setPosition(SIDE_MARGIN_SIZE, oomeesImage->getPositionY() + oomeesImage->getContentSize().height/2 + GapFromOomeeImage);
    UpsaleLayer->addChild(titleLabel);
}

void IAPUpsaleLayer::addButtons()
{
    float GapFromOomeeImage = (oomeesImage->getPositionY() - oomeesImage->getContentSize().height/2)/2;
    
    startTrialButton = ElectricDreamsButton::createButtonWithText("Start Your Free 7 Day Trial!", 300);
    startTrialButton->setPosition(UpsaleLayer->getContentSize().width - SIDE_MARGIN_SIZE - startTrialButton->getContentSize().width, GapFromOomeeImage);
    startTrialButton->setDelegate(this);
    startTrialButton->setMixPanelButtonName("IAPUpsaleSceneStartTrialButton");
    UpsaleLayer->addChild(startTrialButton);
    
    notNowButton = ElectricDreamsButton::createOutlineButtonWithText("Not Now");
    notNowButton->setPosition(SIDE_MARGIN_SIZE, startTrialButton->getCenterPosition().y - notNowButton->getContentSize().height/2);
    notNowButton->setDelegate(this);
    notNowButton->setMixPanelButtonName("IAPUpsaleSceneNotNowButton");
    UpsaleLayer->addChild(notNowButton);
    
    addCancelAnytimeLabel();
    
    /* Not needed for amazon, left here incase for apple.
    restoreButton = ElectricDreamsButton::createTextAsButton("Restore your account", 46, true);
    restoreButton->setPosition(SIDE_MARGIN_SIZE, cancelAnytimeLabel->getPositionY()-restoreButton->getContentSize().height/2);
    restoreButton->setDelegate(this);
    UpsaleLayer->addChild(restoreButton);*/
}

void IAPUpsaleLayer::addCancelAnytimeLabel()
{
    cancelAnytimeLabel = Label::createWithTTF("Cancel anytime", FONT_REGULAR, 46);
    cancelAnytimeLabel->setColor(COLOR_BRIGHT_AQUA);
    cancelAnytimeLabel->setAnchorPoint(Vec2(1,0.5));
    cancelAnytimeLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    cancelAnytimeLabel->setPosition(UpsaleLayer->getContentSize().width - SIDE_MARGIN_SIZE,startTrialButton->getPositionY() - cancelAnytimeLabel->getContentSize().height*1.5);
    UpsaleLayer->addChild(cancelAnytimeLabel);
}

void IAPUpsaleLayer::addALLBulletsAndLabel()
{
    addBulletAndLabel("1000+ ", "TV Shows, games & audiobooks", oomeesImage->getPositionY() + oomeesImage->getContentSize().height/4);
    addBulletAndLabel("Safe, ", "age-appropriate & 100% ad-free", oomeesImage->getPositionY());
    addBulletAndLabel("New ", "content every week", oomeesImage->getPositionY() - oomeesImage->getContentSize().height/4);
}

void IAPUpsaleLayer::addBulletAndLabel(std::string BOLDtext, std::string regularText, float yPosition)
{
    Sprite* bullet = Sprite::create("res/startATrialScene/bulletTick.png");
    bullet->setPosition(SIDE_MARGIN_SIZE + bullet->getContentSize().width/2, yPosition);
    UpsaleLayer->addChild(bullet);
    
    cocos2d::ui::RichText* richTextLabel = cocos2d::ui::RichText::create();
    richTextLabel->setAnchorPoint(Vec2(0,0.5));
    
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, BOLDtext, FONT_BOLD, 80));
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, regularText, FONT_REGULAR, 80));
    richTextLabel->setPosition(Vec2(bullet->getPositionX() + bullet->getContentSize().width,yPosition));
    UpsaleLayer->addChild(richTextLabel);
}

void IAPUpsaleLayer::addOptionalSubscriptionLabel()
{
    auto optionalLabel = Label::createWithTTF("Optional subscription of £4.99 after trial.", FONT_REGULAR, 46);
    optionalLabel->setColor(Color3B::WHITE);
    optionalLabel->setAnchorPoint(Vec2(0,0.5));
    optionalLabel->setHorizontalAlignment(TextHAlignment::RIGHT);
    optionalLabel->setPosition(startTrialButton->getPositionX(), cancelAnytimeLabel->getPositionY());
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
            askForPin();
        else
            PaymentSingleton::getInstance()->startIAPPayment();
    }
    else if(button == notNowButton)
    {
        removeSelf();
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
    PaymentSingleton::getInstance()->startIAPPayment();
}
