#include "FTUScene.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "SceneManagerScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include "BackEndCaller.h"
#include "DynamicNodeHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Scene* FTUScene::createScene()
{
    auto scene = Scene::create();
    auto layer = FTUScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool FTUScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addBackground();
    addLayer();
    addLabels();
    addImages();
    addButtons();

    return true;
}

//----------------- SCENE SETUP ---------------
void FTUScene::addBackground()
{
    LayerColor* backgroundLayer = LayerColor::create(Style::Color_4B::letterBoxSlideBackground,origin.x + visibleSize.width,origin.y + visibleSize.height);
    this->addChild(backgroundLayer);
}

void FTUScene::addLayer()
{
    ftuLayer = LayerColor::create(Color4B::BLACK,visibleSize.width,visibleSize.width*0.6);
    ftuLayer->setPosition(origin.x + visibleSize.width/2 - ftuLayer->getContentSize().width/2,origin.y + visibleSize.height/2 - ftuLayer->getContentSize().height/2);
    
    auto fullscreenGlow = Sprite::create("res/decoration/fullscreen_glow.png");
    fullscreenGlow->setScaleX(ftuLayer->getContentSize().width / fullscreenGlow->getContentSize().width);
    fullscreenGlow->setScaleY(ftuLayer->getContentSize().height / fullscreenGlow->getContentSize().height);
    fullscreenGlow->setPosition(ftuLayer->getContentSize().width / 2, ftuLayer->getContentSize().height / 2);
    ftuLayer->addChild(fullscreenGlow);
    
    this->addChild(ftuLayer);
}

void FTUScene::addLabels()
{
    titleLabel = Label::createWithTTF("Welcome to the world of Azoomee", Style::Font::Regular, 130);
    titleLabel->setColor(Style::Color::brightAqua);
    titleLabel->setAnchorPoint(Vec2(0.5,0.5));
    titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    titleLabel->setPosition(ftuLayer->getContentSize().width/2,ftuLayer->getContentSize().height-titleLabel->getContentSize().height*1.2);
    ftuLayer->addChild(titleLabel);
    
    subTitleLabel = Label::createWithTTF("1000s of kids’ shows, games & audiobooks updated every week", Style::Font::Regular, 76);
    subTitleLabel->setColor(Color3B::WHITE);
    subTitleLabel->setAnchorPoint(Vec2(0.5,0.5));
    subTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    subTitleLabel->setPosition(ftuLayer->getContentSize().width/2,titleLabel->getPositionY()-titleLabel->getContentSize().height/2 - subTitleLabel->getContentSize().height*.3);
    ftuLayer->addChild(subTitleLabel);
}

void FTUScene::addImages()
{
    Sprite* thumbsImage = Sprite::create("res/FTU_Assets/thumbs.png");
    thumbsImage->setPosition(ftuLayer->getContentSize().width/2, subTitleLabel->getPositionY() -thumbsImage->getContentSize().height/2 - subTitleLabel->getContentSize().height);
    ftuLayer->addChild(thumbsImage);
    
    Sprite* gradient = Sprite::create("res/FTU_Assets/blurryMask.png");
    gradient->setScaleX(ftuLayer->getContentSize().width / gradient->getContentSize().width);
    gradient->setScaleY(1.3);
    gradient->setPosition(ftuLayer->getContentSize().width/2,gradient->getContentSize().height/2*1.3
                          );
    ftuLayer->addChild(gradient);
}

void FTUScene::addButtons()
{
    getStartedButton = ElectricDreamsButton::createButtonWithText("Start a FREE trial", 225);
    getStartedButton->setPosition(ftuLayer->getContentSize().width-getStartedButton->getContentSize().width*1.35, getStartedButton->getContentSize().height*.75);
    getStartedButton->setDelegate(this);
    getStartedButton->setMixPanelButtonName("FTUGetStartedButton");
    ftuLayer->addChild(getStartedButton);
    
    notNowButton = ElectricDreamsButton::createTextAsButtonAqua("Take a tour or log in", 60, true);
    notNowButton->setPosition(getStartedButton->getContentSize().width*.35,getStartedButton->getPositionY());
    notNowButton->setDelegate(this);
    notNowButton->setMixPanelButtonName("FTUNotNowButton");
    ftuLayer->addChild(notNowButton);
    
}

//----------------------- Delegate Functions ----------------------------

void FTUScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == notNowButton)
        BackEndCaller::getInstance()->anonymousDeviceLogin();
    else
        DynamicNodeHandler::getInstance()->startSignupFlow();
}

NS_AZOOMEE_END
