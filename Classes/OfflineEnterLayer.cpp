#include "OfflineEnterLayer.h"
#include "OfflineHubScene.h"
#include "OfflineText.h"

USING_NS_CC;

Scene* OfflineEnterLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineEnterLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineEnterLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(Size(2732, 2048));
    createBlackBackground();
    createText();
    createEnterButton();
    
    this->schedule(schedule_selector(OfflineEnterLayer::update));
    
    return true;
}

void OfflineEnterLayer::buttonPressed(ElectricDreamsButton *button)
{
    Director::getInstance()->replaceScene(OfflineHubScene::createScene());
}

//All methods beyond this line are private--------------------------------------------

void OfflineEnterLayer::createBlackBackground()
{
    auto blackbg = LayerColor::create(Color4B::BLACK, this->getContentSize().width, this->getContentSize().height);
    this->addChild(blackbg);
}

void OfflineEnterLayer::createText()
{
    auto offlineText = OfflineText::create();
    offlineText->setCascadeOpacityEnabled(true);
    offlineText->createForLogin();
    this->addChild(offlineText);
    offlineText->createForLogin();
}

void OfflineEnterLayer::createEnterButton()
{
    auto enterButton = ElectricDreamsButton::createButtonWithText("Enter offline");
    
    Size buttonContentSize = enterButton->getContentSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    enterButton->setDelegate(this);
    enterButton->setName("enterButton");
    enterButton->setPosition(visibleOrigin.x + visibleSize.width / 2 - buttonContentSize.width / 2, visibleOrigin.y + visibleSize.height * 0.2);
    enterButton->setCascadeOpacityEnabled(true);
    this->addChild(enterButton);
}


void OfflineEnterLayer::update(float delta)
{
    //if(this->getOpacity() > 0) this->getChildByName("enterButton")->setVisible(true);
    //else this->getChildByName("enterButton")->setVisible(false);
}
