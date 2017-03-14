#include "IAPUpsaleScene.h"
#include "ui/UIRichText.h"

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
    
    
    addTitle();
    addCancelAnytimeLabel();
    addButtons();
    addImage();
    addALLBulletsAndLabel();
    addOptionalSubscriptionLabel();
    
    return true;
}

//----------------- SCENE SETUP ---------------
void IAPUpsaleScene::addTitle()
{
    titleLabel = Label::createWithTTF("Start a trial to unlock full access", FONT_REGULAR, 130);
    titleLabel->setColor(COLOR_BRIGHT_AQUA);
    titleLabel->setAnchorPoint(Vec2(0,0.5));
    titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    titleLabel->setPosition(origin.x + SIDE_MARGIN_SIZE, origin.y + visibleSize.height - titleLabel->getContentSize().height * 1.5);
    this->addChild(titleLabel);
}

void IAPUpsaleScene::addCancelAnytimeLabel()
{
    cancelAnytimeLabel = Label::createWithTTF("Cancel anytime", FONT_REGULAR, 46);
    cancelAnytimeLabel->setColor(COLOR_BRIGHT_AQUA);
    cancelAnytimeLabel->setAnchorPoint(Vec2(1,0.5));
    cancelAnytimeLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    cancelAnytimeLabel->setPosition(origin.x + visibleSize.width - SIDE_MARGIN_SIZE, origin.y +cancelAnytimeLabel->getContentSize().height * 2.5);
    this->addChild(cancelAnytimeLabel);
}

void IAPUpsaleScene::addButtons()
{
    startTrialButton = ElectricDreamsButton::createButtonWithText("Start Your Free 7 Day Trial!", 300);
    startTrialButton->setPosition(origin.x + visibleSize.width - SIDE_MARGIN_SIZE - startTrialButton->getContentSize().width, cancelAnytimeLabel->getPositionY() + cancelAnytimeLabel->getContentSize().height*2);
    this->addChild(startTrialButton);
    
    notNowButton = ElectricDreamsButton::createOutlineButtonWithText("Not Now");
    notNowButton->setPosition(origin.x + SIDE_MARGIN_SIZE, startTrialButton->getCenterPosition().y - notNowButton->getContentSize().height/2);
    this->addChild(notNowButton);
    
    restoreButton = ElectricDreamsButton::createTextAsButton("Restore your account", 46, true);
    restoreButton->setPosition(origin.x + SIDE_MARGIN_SIZE, cancelAnytimeLabel->getPositionY()-restoreButton->getContentSize().height/2);
    this->addChild(restoreButton);
}

void IAPUpsaleScene::addImage()
{
    oomeesImage = Sprite::create("res/startATrialScene/oomees.png");
    
    float oomeeImageYposition = titleLabel->getPositionY() - titleLabel->getContentSize().height - oomeesImage->getContentSize().height/2;
    
    if(oomeeImageYposition < (titleLabel->getPositionY() - startTrialButton->getPositionY())/2 + startTrialButton->getPositionY() + startTrialButton->getContentSize().height/2)
        oomeeImageYposition = (titleLabel->getPositionY() - startTrialButton->getPositionY())/2 + startTrialButton->getPositionY() + startTrialButton->getContentSize().height/2;
        
    oomeesImage->setPosition(origin.x + visibleSize.width - SIDE_MARGIN_SIZE - oomeesImage->getContentSize().width/2, oomeeImageYposition);
    this->addChild(oomeesImage);
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
    
    auto richTextLabel = cocos2d::ui::RichText::create();
    richTextLabel->setAnchorPoint(Vec2(0,0.5));
    
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, BOLDtext, FONT_BOLD, 80));
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, regularText, FONT_REGULAR, 80));
    richTextLabel->::IAPUpsaleScene::setPosition(bullet->getPositionX() + bullet->getContentSize().width,yPosition);
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

}

