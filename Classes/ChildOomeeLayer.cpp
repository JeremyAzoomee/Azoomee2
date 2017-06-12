#include "ChildOomeeLayer.h"
#include "ui/UIScale9Sprite.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>

using namespace Azoomee;

Layer* ChildOomeeLayer::createWithChildDetails(std::string setChildName, int setOomeeNo)
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
    reduceLabelTextToFitWidth(childNameLabel,this->getContentSize().width*.8);
    childNameLabel->setPosition(this->getContentSize().width/2,this->getContentSize().height-childNameLabel->getContentSize().height*1.2);
    this->addChild(childNameLabel);
}

void ChildOomeeLayer::addOomee()
{
    Vec2 position = Vec2(this->getContentSize().width / 2, this->getContentSize().height*.6);
    
    Sprite* glow = createGlow();
    glow->setPosition(position);
    glow->setScale(.5);
    this->addChild(glow);
    
    auto oomee = Sprite::create(StringUtils::format("res/childSelection/%s.png", ConfigStorage::getInstance()->getNameForOomee(oomeeNo).c_str()));
    oomee->setScale(1.8);
    oomee->setPosition(position);
    this->addChild(oomee);
}

