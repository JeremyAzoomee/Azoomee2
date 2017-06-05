#include "FTUScene.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

#define COLOR_BRIGHT_AQUA Color3B(28, 244, 244)

using namespace Azoomee;


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
    LayerColor* backgroundLayer = LayerColor::create(Color4B(48,38,38,255),origin.x + visibleSize.width,origin.y + visibleSize.height);
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
    Label* titleLabel = Label::createWithTTF("Welcome to the world of Azoomee", FONT_REGULAR, 130);
    titleLabel->setColor(COLOR_BRIGHT_AQUA);
    titleLabel->setAnchorPoint(Vec2(0,0.5));
    titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    titleLabel->setPosition(ftuLayer->getContentSize().width/2,ftuLayer->getContentSize().height-titleLabel->getContentSize().height);
    ftuLayer->addChild(titleLabel);
    
    
}

void FTUScene::addImages()
{
    
}

void FTUScene::addButtons()
{
    
}

//----------------------- Delegate Functions ----------------------------

void FTUScene::buttonPressed(ElectricDreamsButton* button)
{

}
