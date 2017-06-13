#include "SettingsKidsChildControlLayer.h"
#include "ChildOomeeLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace Azoomee;

Layer* SettingsKidsChildControlLayer::createController(SettingsKidsLayer* setParent, int setChildNumber)
{
    auto layer = SettingsKidsChildControlLayer::create();
    layer->parentLayer = setParent;
    layer->childNumber = setChildNumber;
    layer->addChildFrame();
    layer->addButtons();
    
    return layer;
}

bool SettingsKidsChildControlLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//----------------Add UI Objects-------------

void SettingsKidsChildControlLayer::addChildFrame()
{
    std::string oomeeUrl = ParentDataProvider::getInstance()->getAvatarForAnAvailableChildren(childNumber);
    int oomeeNr = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
    
    childFrameLayer = ChildOomeeLayer::createWithChildDetails(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber), oomeeNr);
    this->setContentSize(childFrameLayer->getContentSize());
    this->addChild(childFrameLayer);
    
}

void SettingsKidsChildControlLayer::addButtons()
{
    inviteButton = ElectricDreamsButton::createInviteaMainButton("Kid Code", this->getContentSize().width*.66);
    inviteButton->setCenterPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height*.25));
    inviteButton->setDelegate(this);
    childFrameLayer->addChild(inviteButton);
    
    acceptButton = ElectricDreamsButton::createInviteaMainButton("Add A Friend", this->getContentSize().width*.66);
    acceptButton->setCenterPosition(Vec2(this->getContentSize().width/2,inviteButton->getPositionY() - acceptButton->getContentSize().height));
    acceptButton->setDelegate(this);
    childFrameLayer->addChild(acceptButton);
    
    closeButton = ElectricDreamsButton::createWindowCloselButton();
    closeButton->setVisible(false);
    closeButton->setScale(0.75);
    closeButton->setPosition(this->getContentSize().width-closeButton->getContentSize().width,this->getContentSize().height-closeButton->getContentSize().height);
    closeButton->setDelegate(this);
    childFrameLayer->addChild(closeButton);

}

//----------------------- Delegate Functions ----------------------------

void SettingsKidsChildControlLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button ==inviteButton)
    {
        this->setLocalZOrder(220);
        parentLayer->scrollToPosition(childNumber);
        closeButton->setVisible(true);
    }
    else if(button == closeButton)
    {
        this->setLocalZOrder(200);
        closeButton->setVisible(false);
        parentLayer->scrollReset();
    }
}

