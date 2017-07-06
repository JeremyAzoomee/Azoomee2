#include "DisplayChildNameLayer.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "ui/UIScale9Sprite.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQHistoryManager.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Scene* DisplayChildNameLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = DisplayChildNameLayer::create();
    scene->addChild(layer);

    return scene;
}

bool DisplayChildNameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    delayTime = 0.4;
    scaleTime = 0.5;
    
    if(!HQHistoryManager::getInstance()->noHistory())
    {
        delayTime = 0.01;
        scaleTime = 0.01;
    }
    
    createChildNameToLayer();
    createLoggedInAsToLayer();
    createInviteCodeLayer();
    setLayerSizeAndAnimate();
    
    return true;
}

//-----------------SETUP FUNCTIONS-----------

void DisplayChildNameLayer::createChildNameToLayer()
{
    std::string childName = ChildDataProvider::getInstance()->getLoggedInChildName();
    
    childNameLabel = createLabelBody(childName);
    childNameLabel->setPosition(childNameLabel->getContentSize().width/2, childNameLabel->getContentSize().height*2);
    childNameLabel->setOpacity(0);
}

void DisplayChildNameLayer::createLoggedInAsToLayer()
{
    loggedInAsLabel = createLabelWith("Kid Code", FONT_REGULAR, Style::Color::brightAqua, 40);
    loggedInAsLabel->setPosition(loggedInAsLabel->getContentSize().width/2, childNameLabel->getContentSize().height*1.1);
    loggedInAsLabel->setOpacity(0);
}

void DisplayChildNameLayer::createInviteCodeLayer()
{
    inviteCodeLabel = createLabelWith(ParentDataProvider::getInstance()->getInviteCodeForAvailableChildren(ChildDataProvider::getInstance()->getLoggedInChildNumber()), FONT_KIDCODE_REGULAR, Style::Color::white, 48);
    inviteCodeLabel->setPosition(inviteCodeLabel->getContentSize().width/2, inviteCodeLabel->getContentSize().height/2);
    inviteCodeLabel->setOpacity(0);

}

void DisplayChildNameLayer::setLayerSizeAndAnimate()
{
    this->setContentSize(Size(childNameLabel->getContentSize().width, childNameLabel->getContentSize().height*4));
    
    this->addChild(childNameLabel);
    this->addChild(loggedInAsLabel);
    this->addChild(inviteCodeLabel);
    
    childNameLabel->runAction(Sequence::create(DelayTime::create(0.1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    loggedInAsLabel->runAction(Sequence::create(DelayTime::create(0.1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    inviteCodeLabel->runAction(Sequence::create(DelayTime::create(0.1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
}

NS_AZOOMEE_END
