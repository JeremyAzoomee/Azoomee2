#include "ForceUpdateAppLockScene.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include "ForceUpdateSingleton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ForceUpdateAppLockScene::init()
{
    if ( !Super::init() )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    addVisualComponentsToScene();
    addUpdateButtonToScene();

    return true;
}

void ForceUpdateAppLockScene::addVisualComponentsToScene()
{
    addSideWiresToScreen(this, 0, 1);
    
    auto titleLabel = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(FORCE_UPDATE_TITLE), Style::Font::Regular, 130);
    titleLabel->setColor(Style::Color::brightAqua);
    titleLabel->setAnchorPoint(Vec2(0.5,1.0));
    titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    titleLabel->setWidth(_visibleSize.width * 0.8f);
    titleLabel->setPosition(_visibleSize.width/2, _visibleSize.height - 100);
    this->addChild(titleLabel);
    
    auto subTitleLabel = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(FORCE_UPDATE_SUB_TITLE), Style::Font::Regular, 76);
    subTitleLabel->setColor(Color3B::WHITE);
    subTitleLabel->setAnchorPoint(Vec2(0.5,1.0));
    subTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    subTitleLabel->setWidth(_visibleSize.width * 0.8f);
    subTitleLabel->setPosition((_visibleSize.width/2),titleLabel->getPositionY() - titleLabel->getContentSize().height - 100);
    this->addChild(subTitleLabel);
}

void ForceUpdateAppLockScene::addUpdateButtonToScene()
{
    updateButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(FORCE_UPDATE_BUTTON), 225);
    updateButton->setPosition(_visibleSize.width / 2 - updateButton->getContentSize().width / 2, updateButton->getContentSize().height * 0.75f);
    updateButton->setDelegate(this);
    updateButton->setMixPanelButtonName("UpdateAzoomeeButton");
    this->addChild(updateButton);
}

void ForceUpdateAppLockScene::onSizeChanged()
{
    removeAllChildrenWithCleanup(true);
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    addVisualComponentsToScene();
    addUpdateButtonToScene();
}

//----------------------- Delegate Functions ----------------------------

void ForceUpdateAppLockScene::buttonPressed(ElectricDreamsButton* button)
{
    Application::getInstance()->openURL(ForceUpdateSingleton::getInstance()->getUpdateUrlFromFile());
}

void ForceUpdateAppLockScene::onExit()
{
    Director::getInstance()->replaceScene(ForceUpdateAppLockScene::create()); //prevent this scene to be removed by any others.
    Super::onExit();
}

NS_AZOOMEE_END
