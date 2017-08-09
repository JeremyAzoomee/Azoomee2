#include "ForceUpdateAppLockScene.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include "ForceUpdateSingleton.h"

#define COLOR_BRIGHT_AQUA Color3B(28, 244, 244)

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Scene* ForceUpdateAppLockScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ForceUpdateAppLockScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool ForceUpdateAppLockScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addVisualComponentsToScene();
    addUpdateButtonToScene();

    return true;
}

void ForceUpdateAppLockScene::addVisualComponentsToScene()
{
    addSideWiresToScreen(this, 0, 2);
    
    auto titleLabel = Label::createWithTTF("Azoomee needs to be updated!", Style::Font::Regular, 130);
    titleLabel->setColor(COLOR_BRIGHT_AQUA);
    titleLabel->setAnchorPoint(Vec2(0.5,0.5));
    titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    titleLabel->setPosition(visibleSize.width/2 + origin.x, visibleSize.height + origin.y -titleLabel->getContentSize().height * 1.2f);
    this->addChild(titleLabel);
    
    auto subTitleLabel = Label::createWithTTF("It looks like you are using an old version of Azoomee.\nTo continue using Azoomee, please ask\na grown-up to update it to the latest version\nby tapping the button below.", Style::Font::Regular, 76);
    subTitleLabel->setColor(Color3B::WHITE);
    subTitleLabel->setAnchorPoint(Vec2(0.5,0.5));
    subTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    subTitleLabel->setPosition(visibleSize.width/2,titleLabel->getPositionY()-titleLabel->getContentSize().height - subTitleLabel->getContentSize().height);
    this->addChild(subTitleLabel);
}

void ForceUpdateAppLockScene::addUpdateButtonToScene()
{
    updateButton = ElectricDreamsButton::createButtonWithText("Update Azoomee", 225);
    updateButton->setPosition(visibleSize.width / 2 + origin.x - updateButton->getContentSize().width / 2, updateButton->getContentSize().height * 0.75f + origin.y);
    updateButton->setDelegate(this);
    updateButton->setMixPanelButtonName("UpdateAzoomeeButton");
    this->addChild(updateButton);
}

//----------------------- Delegate Functions ----------------------------

void ForceUpdateAppLockScene::buttonPressed(ElectricDreamsButton* button)
{
    Application::getInstance()->openURL(ForceUpdateSingleton::getInstance()->getUpdateUrlFromFile());
}

void ForceUpdateAppLockScene::onExit()
{
    Director::getInstance()->replaceScene(ForceUpdateAppLockScene::createScene()); //prevent this scene to be removed by any others.
    Node::onExit();
}

NS_AZOOMEE_END
