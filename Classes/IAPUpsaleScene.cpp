#include "IAPUpsaleScene.h"
#include "cocos/ui/UIRichText.h"
#include "ElectricDreamsDecoration.h"
#include "PaymentSingleton.h"

USING_NS_CC;

#define SIDE_MARGIN_SIZE 130

#define FONT_MEDIUM "fonts/Sofia Pro Soft Medium.otf"
#define FONT_REGULAR "fonts/Sofia Pro Soft Regular.otf"
#define FONT_BOLD "fonts/Sofia Pro Soft Bold.otf"

#define COLOR_BRIGHT_AQUA Color3B(28, 244, 244)

Scene* IAPUpsaleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = IAPUpsaleScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool IAPUpsaleScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addFullScreenGlowToScreen(this);
    addImage();
    addTitle();
    addButtons();
    addALLBulletsAndLabel();
    addOptionalSubscriptionLabel();
    
    PaymentSingleton::getInstance()->isAmazonDevice();
    
    return true;
}

//----------------- SCENE SETUP ---------------
void IAPUpsaleScene::addImage()
{
    oomeesImage = Sprite::create("res/startATrialScene/oomees.png");
    oomeesImage->setPosition(origin.x + visibleSize.width - SIDE_MARGIN_SIZE - oomeesImage->getContentSize().width/2, origin.y + visibleSize.height *.6);
    this->addChild(oomeesImage);
}

void IAPUpsaleScene::addTitle()
{
    titleLabel = Label::createWithTTF("Start a trial to unlock full access", FONT_REGULAR, 130);
    titleLabel->setColor(COLOR_BRIGHT_AQUA);
    titleLabel->setAnchorPoint(Vec2(0,0.5));
    titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    
    float GapFromOomeeImage = (origin.y + visibleSize.height - (oomeesImage->getPositionY() + oomeesImage->getContentSize().height/2))*.33;
    
    titleLabel->setPosition(origin.x + SIDE_MARGIN_SIZE, oomeesImage->getPositionY() + oomeesImage->getContentSize().height/2 + GapFromOomeeImage);
    this->addChild(titleLabel);
}

void IAPUpsaleScene::addButtons()
{
    float GapFromOomeeImage = ((oomeesImage->getPositionY() - oomeesImage->getContentSize().height/2) - origin.y)*.33;
    
    startTrialButton = ElectricDreamsButton::createButtonWithText("Start Your Free 7 Day Trial!", 300);
    startTrialButton->setPosition(origin.x + visibleSize.width - SIDE_MARGIN_SIZE - startTrialButton->getContentSize().width, oomeesImage->getPositionY() - oomeesImage->getContentSize().height/2 - GapFromOomeeImage - startTrialButton->getContentSize().height/2);
    this->addChild(startTrialButton);
    
    notNowButton = ElectricDreamsButton::createOutlineButtonWithText("Not Now");
    notNowButton->setPosition(origin.x + SIDE_MARGIN_SIZE, startTrialButton->getCenterPosition().y - notNowButton->getContentSize().height/2);
    this->addChild(notNowButton);
    
    addCancelAnytimeLabel();
    
    restoreButton = ElectricDreamsButton::createTextAsButton("Restore your account", 46, true);
    restoreButton->setPosition(origin.x + SIDE_MARGIN_SIZE, cancelAnytimeLabel->getPositionY()-restoreButton->getContentSize().height/2);
    this->addChild(restoreButton);
}

void IAPUpsaleScene::addCancelAnytimeLabel()
{
    cancelAnytimeLabel = Label::createWithTTF("Cancel anytime", FONT_REGULAR, 46);
    cancelAnytimeLabel->setColor(COLOR_BRIGHT_AQUA);
    cancelAnytimeLabel->setAnchorPoint(Vec2(1,0.5));
    cancelAnytimeLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    cancelAnytimeLabel->setPosition(origin.x + visibleSize.width - SIDE_MARGIN_SIZE, origin.y + cancelAnytimeLabel->getContentSize().height*1.5);
    this->addChild(cancelAnytimeLabel);
}

void IAPUpsaleScene::addALLBulletsAndLabel()
{
    addBulletAndLabel("1000+ ", "TV Shows, games & audiobooks", oomeesImage->getPositionY() + oomeesImage->getContentSize().height/4);
    addBulletAndLabel("Safe, ", "age-appropriate & 100% ad-free", oomeesImage->getPositionY());
    addBulletAndLabel("New ", "content every week", oomeesImage->getPositionY() - oomeesImage->getContentSize().height/4);
}

void IAPUpsaleScene::addBulletAndLabel(std::string BOLDtext, std::string regularText, float yPosition)
{
    Sprite* bullet = Sprite::create("res/startATrialScene/bulletTick.png");
    bullet->setPosition(origin.x + SIDE_MARGIN_SIZE + bullet->getContentSize().width/2, yPosition);
    this->addChild(bullet);
    
    cocos2d::ui::RichText* richTextLabel = cocos2d::ui::RichText::create();
    richTextLabel->setAnchorPoint(Vec2(0,0.5));
    
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, BOLDtext, FONT_BOLD, 80));
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, regularText, FONT_REGULAR, 80));
    richTextLabel->setPosition(Vec2(bullet->getPositionX() + bullet->getContentSize().width,yPosition));
    this->addChild(richTextLabel);
}

void IAPUpsaleScene::addOptionalSubscriptionLabel()
{
    auto optionalLabel = Label::createWithTTF("Optional subscription of £4.99 after trial.", FONT_REGULAR, 46);
    optionalLabel->setColor(Color3B::WHITE);
    optionalLabel->setAnchorPoint(Vec2(0,0.5));
    optionalLabel->setHorizontalAlignment(TextHAlignment::RIGHT);
    optionalLabel->setPosition(startTrialButton->getPositionX(), cancelAnytimeLabel->getPositionY());
    this->addChild(optionalLabel);
}

//--------------DELEGATE FUNCTIONS---------

void IAPUpsaleScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == startTrialButton)
    {
        
    }
    if(button == restoreButton)
    {
        
    }

    if(button == notNowButton)
    {
        
    }
}

