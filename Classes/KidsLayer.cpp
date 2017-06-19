#include "KidsLayer.h"
#include "ui/UIScale9Sprite.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>

using namespace Azoomee;

KidsLayer* KidsLayer::createWithChildDetails(std::string setChildName, int setOomeeNo)
{
    auto layer = KidsLayer::create();
    layer->childName = setChildName;
    layer->oomeeNo = setOomeeNo;
    layer->addFrame();
    layer->addChildName();
    layer->addOomee();
    
    return layer;
}

bool KidsLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//----------------Add UI Objects-------------

void KidsLayer::addFrame()
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

void KidsLayer::addChildName()
{
    Label* childNameLabel = createLabelChildNameSettings(childName);
    reduceLabelTextToFitWidth(childNameLabel,this->getContentSize().width*.95);
    childNameLabel->setPosition(this->getContentSize().width/2,this->getContentSize().height-childNameLabel->getContentSize().height*1.7);
    this->addChild(childNameLabel);
}

void KidsLayer::addOomee()
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

void KidsLayer::setToShowingCode()
{
    resetToIdle();
    
    Label* detailsLabel = createLabelSettingsChat("Share your Kid Code\nwith all your friends",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,oomeeSprite->getPositionY() - (oomeeSprite->getContentSize().height/2 * oomeeSprite->getScale()) - detailsLabel->getContentSize().height);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
    
    Label* codeLabel = createLabelHeader("49KW03B3");
    codeLabel->setPosition(this->getContentSize().width/2,detailsLabel->getPositionY()- detailsLabel->getContentSize().height/2 -codeLabel->getContentSize().height * .8);
    codeLabel->setTag(1000);
    this->addChild(codeLabel);
}

void KidsLayer::setToAddAFriend()
{
    resetToIdle();
    
    Label* detailsLabel = createLabelSettingsChat("To add a friend",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,oomeeSprite->getPositionY() - (oomeeSprite->getContentSize().height/2 * oomeeSprite->getScale()) - detailsLabel->getContentSize().height*2.5);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
}

void KidsLayer::setToAddAFriendTextBox()
{
    resetToIdle();
    
    Label* detailsLabel = createLabelSettingsChat("To add a friend",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,oomeeSprite->getPositionY() - (oomeeSprite->getContentSize().height/2 * oomeeSprite->getScale()) - detailsLabel->getContentSize().height*2.5);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
}

void KidsLayer::setToCodeError(std::string code)
{
    resetToIdle();
    
    Vec2 position = Vec2(this->getContentSize().width / 2, this->getContentSize().height*.67);
    
    glowSprite->setPosition(position);
    glowSprite->setScale(.3);
    
    oomeeSprite->setScale(1.1);
    oomeeSprite->setPosition(position);
    
    Label* failedTextBoxLabel = createLabelHeader(code);
    failedTextBoxLabel->setColor(Color3B(249, 74, 91));
    
    Rect spriteRect = Rect(0, 0, 268, 107);
    Rect capInsents = Rect(100, 53, 1, 1);
    
    
    ui::Scale9Sprite* failedTextBox = ui::Scale9Sprite::create("res/settings/textFieldFail.png", spriteRect, capInsents);
    failedTextBox->setContentSize(Size(this->getContentSize().width*.8, 107));
    failedTextBox->setPosition(Vec2(this->getContentSize().width/2, failedTextBox->getContentSize().height*4));
    
    failedTextBoxLabel->setPosition(failedTextBox->getContentSize().width/2, failedTextBox->getContentSize().height/2-5);
    
    failedTextBox->addChild(failedTextBoxLabel);
    failedTextBox->setTag(1000);
    this->addChild(failedTextBox);
    
    Label* detailsLabel = createLabelSettingsChat("Oh No! We don't recognise\nthis code",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,failedTextBox->getPositionY() + detailsLabel->getContentSize().height*1.25);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
    
}

void KidsLayer::setToCodeSuccess(std::string code)
{
    resetToIdle();
    
    Vec2 position = Vec2(this->getContentSize().width / 2, this->getContentSize().height*.67);
    
    glowSprite->setPosition(position);
    glowSprite->setScale(.3);
    
    oomeeSprite->setScale(1.1);
    oomeeSprite->setPosition(position);
    
    Label* failedTextBoxLabel = createLabelHeader(code);
    failedTextBoxLabel->setColor(Color3B(55, 188, 152));
    
    Rect spriteRect = Rect(0, 0, 268, 107);
    Rect capInsents = Rect(100, 53, 1, 1);
    
    ui::Scale9Sprite* failedTextBox = ui::Scale9Sprite::create("res/settings/textEntry.png", spriteRect, capInsents);
    failedTextBox->setContentSize(Size(this->getContentSize().width*.8, 107));
    failedTextBox->setPosition(Vec2(this->getContentSize().width/2, failedTextBox->getContentSize().height*4.5));
    
    failedTextBoxLabel->setPosition(failedTextBox->getContentSize().width/2, failedTextBox->getContentSize().height/2-5);
    
    failedTextBox->addChild(failedTextBoxLabel);
    failedTextBox->setTag(1000);
    this->addChild(failedTextBox);
    
    Label* titleLabel = createLabelSettingsChat("Kid Code accepted",Color3B::WHITE);
    titleLabel->setPosition(this->getContentSize().width/2,failedTextBox->getPositionY() + titleLabel->getContentSize().height*2.25);
    titleLabel->setTag(1000);
    this->addChild(titleLabel);
    
    Label* detailsLabel = createLabelSettingsChat("Your friend has been added!\nTheir parents will need to\nconfirm the connection.",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,failedTextBox->getPositionY() - detailsLabel->getContentSize().height);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
}

void KidsLayer::resetToIdle()
{
    while(this->getChildByTag(1000))
        this->removeChildByTag(1000);
    
    Vec2 position = Vec2(this->getContentSize().width / 2, this->getContentSize().height*.6);
    
    glowSprite->setPosition(position);
    glowSprite->setScale(.5);
    
    oomeeSprite->setScale(1.8);
    oomeeSprite->setPosition(position);
}

