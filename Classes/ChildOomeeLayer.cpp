#include "ChildOomeeLayer.h"
#include "ui/UIScale9Sprite.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>

using namespace Azoomee;

ChildOomeeLayer* ChildOomeeLayer::createWithChildDetails(std::string setChildName, int setOomeeNo)
{
    auto layer = ChildOomeeLayer::create();
    layer->childName = setChildName;
    layer->oomeeNo = setOomeeNo;
    layer->addFrame();
    layer->addChildName();
    layer->addOomee();
    
    return layer;
}

bool ChildOomeeLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//----------------Add UI Objects-------------

void ChildOomeeLayer::addFrame()
{
    Rect spriteRect = Rect(0, 0, 206, 218);
    Rect capInsents = Rect(50, 50, 100, 1);
    
    ui::Scale9Sprite* childFrame = ui::Scale9Sprite::create("res/settings/child_frame.png", spriteRect, capInsents);
    childFrame->setContentSize(Size(800,1200));
    childFrame->setAnchorPoint(Vec2(0,0));
    
    this->setContentSize(childFrame->getContentSize());
    
    childFrame->setPosition(Vec2(0, 0));
    
    this->addChild(childFrame);
}

void ChildOomeeLayer::addChildName()
{
    Label* childNameLabel = createLabelChildNameSettings(childName);
    reduceLabelTextToFitWidth(childNameLabel,this->getContentSize().width*.95);
    childNameLabel->setPosition(this->getContentSize().width/2,this->getContentSize().height-childNameLabel->getContentSize().height*1.7);
    this->addChild(childNameLabel);
}

void ChildOomeeLayer::addOomee()
{
    Vec2 position = Vec2(this->getContentSize().width / 2, this->getContentSize().height*.6);
    
    glowSprite = createGlow();
    glowSprite->setPosition(position);
    glowSprite->setScale(.5);
    this->addChild(glowSprite);
    
    oomeeSprite = Sprite::create(StringUtils::format("res/childSelection/%s.png", ConfigStorage::getInstance()->getNameForOomee(oomeeNo).c_str()));
    oomeeSprite->setScale(1.8);
    oomeeSprite->setPosition(position);
    this->addChild(oomeeSprite);
}

void ChildOomeeLayer::setToShowingCode()
{
    Label* detailsLabel = createLabelSettingsChat("Share your Kid Code\nwith all your friends",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,oomeeSprite->getPositionY() - (oomeeSprite->getContentSize().height/2 * oomeeSprite->getScale()) - detailsLabel->getContentSize().height);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
    
    Label* codeLabel = createLabelHeader("49KW03B3");
    codeLabel->setPosition(this->getContentSize().width/2,detailsLabel->getPositionY()- detailsLabel->getContentSize().height/2 -codeLabel->getContentSize().height);
    codeLabel->setTag(1000);
    this->addChild(codeLabel);
}

void ChildOomeeLayer::setToAddAFriend()
{
    Label* detailsLabel = createLabelSettingsChat("To add a friend",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,oomeeSprite->getPositionY() - (oomeeSprite->getContentSize().height/2 * oomeeSprite->getScale()) - detailsLabel->getContentSize().height*2.5);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
}

void ChildOomeeLayer::resetToIdle()
{
    while(this->getChildByTag(1000))
        this->removeChildByTag(1000);
}

