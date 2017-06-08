#include "SettingsLayer.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>

#define LINE_WIDTH 4
#define TAB_SPACING 50
#define SELECTED_TAB_Z 111
#define IDLE_TAB_Z 100

using namespace Azoomee;


bool SettingsLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createSettingsLayer();
    createSettingsController();
    //AwaitingAdultPinLayer::create()->setDelegate(this);
    
    return true;
}

void SettingsLayer::createSettingsLayer()
{
    backgroundLayer = LayerColor::create(Color4B::BLACK,origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void SettingsLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

//---------------------- Create Settings Tabs -----------------------------

void SettingsLayer::createSettingsController()
{
    createCancelButton();
    createLine();
    createTabs();
}

void SettingsLayer::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWindowCloselButton();
    cancelButton->setCenterPosition(Vec2(origin.x + visibleSize.width - cancelButton->getContentSize().width, origin.y + visibleSize.height - cancelButton->getContentSize().height));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("CancelSettingsButton");
    backgroundLayer->addChild(cancelButton);
}

void SettingsLayer::createLine()
{
    linePositionY = origin.y+visibleSize.height-cancelButton->getContentSize().height*2;
    
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->setLineWidth(LINE_WIDTH);
    newDrawNode->drawLine(Vec2(0, linePositionY), Vec2(visibleSize.width, linePositionY), Color4F(28.0/255, 244.0/255, 244.0/255, 1.0));
    backgroundLayer->addChild(newDrawNode,110);
    
}

void SettingsLayer::createTabs()
{
    childrenButton = ElectricDreamsButton::createTabButton("Your Kids");
    childrenButton->setPosition(TAB_SPACING*2+childrenButton->getContentSize().width/2,linePositionY + childrenButton->getContentSize().height/2-LINE_WIDTH);
    childrenButton->setDelegate(this);
    backgroundLayer->addChild(childrenButton,IDLE_TAB_Z);
    
    confirmationButton = ElectricDreamsButton::createTabButton("Confirmation");
    confirmationButton->setPosition(childrenButton->getPositionX()+childrenButton->getContentSize().width/2+ TAB_SPACING+confirmationButton->getContentSize().width/2,linePositionY + confirmationButton->getContentSize().height/2-LINE_WIDTH);
    confirmationButton->setDelegate(this);
    backgroundLayer->addChild(confirmationButton,IDLE_TAB_Z);
    
    accountButton = ElectricDreamsButton::createTabButton("Account");
    accountButton->setPosition(confirmationButton->getPositionX()+confirmationButton->getContentSize().width/2+TAB_SPACING+accountButton->getContentSize().width/2,linePositionY + accountButton->getContentSize().height/2-LINE_WIDTH);
    accountButton->setDelegate(this);
    backgroundLayer->addChild(accountButton,SELECTED_TAB_Z);
}

//---------------------- Actions -----------------

void SettingsLayer::removeSelf()
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}


//----------------------- Delegate Functions ----------------------------

void SettingsLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
    {
        AudioMixer::getInstance()->resumeBackgroundMusic();
        removeSelf();
    }
    else if(button == childrenButton)
    {
        childrenButton->setLocalZOrder(SELECTED_TAB_Z);
        confirmationButton->setLocalZOrder(IDLE_TAB_Z);
        accountButton->setLocalZOrder(IDLE_TAB_Z);
    }
    else if(button == confirmationButton)
    {
        childrenButton->setLocalZOrder(IDLE_TAB_Z);
        confirmationButton->setLocalZOrder(SELECTED_TAB_Z);
        accountButton->setLocalZOrder(IDLE_TAB_Z);
    }
    else if(button == accountButton)
    {
        childrenButton->setLocalZOrder(IDLE_TAB_Z);
        confirmationButton->setLocalZOrder(IDLE_TAB_Z);
        accountButton->setLocalZOrder(SELECTED_TAB_Z);
    }
}

void SettingsLayer::AdultPinCancelled(AwaitingAdultPinLayer* layer)
{
    removeSelf();
}

void SettingsLayer::AdultPinAccepted(AwaitingAdultPinLayer* layer)
{
    createSettingsController();
}
