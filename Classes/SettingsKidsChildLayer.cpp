#include "SettingsKidsChildLayer.h"
#include "ChildOomeeLayer.h"

using namespace Azoomee;

Layer* SettingsKidsChildLayer::createWithChildDetails(std::string setChildName, int setOomeeNo)
{
    auto layer = SettingsKidsChildLayer::create();
    layer->childName = setChildName;
    layer->oomeeNo = setOomeeNo;
    layer->addChildFrame();
    layer->addButtons();
    
    return layer;
}

bool SettingsKidsChildLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//----------------Add UI Objects-------------

void SettingsKidsChildLayer::addChildFrame()
{
    childFrameLayer = ChildOomeeLayer::createWithChildDetails(childName, oomeeNo);
    this->setContentSize(childFrameLayer->getContentSize());
    this->addChild(childFrameLayer);
    
}

void SettingsKidsChildLayer::addButtons()
{
    inviteButton = ElectricDreamsButton::createInviteaMainButton("Kid Code", this->getContentSize().width*.66);
    inviteButton->setCenterPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height*.25));
    childFrameLayer->addChild(inviteButton);
    
    acceptButton = ElectricDreamsButton::createInviteaMainButton("Add A Friend", this->getContentSize().width*.66);
    acceptButton->setCenterPosition(Vec2(this->getContentSize().width/2,inviteButton->getPositionY() - acceptButton->getContentSize().height));
    childFrameLayer->addChild(acceptButton);

}

//----------------------- Delegate Functions ----------------------------

void SettingsKidsChildLayer::buttonPressed(ElectricDreamsButton* button)
{

}

