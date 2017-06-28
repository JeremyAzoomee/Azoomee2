#include "KidsLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

KidsLayer* KidsLayer::createWithChildDetails(int setChildNumber)
{
    auto layer = KidsLayer::create();
    layer->childNumber = setChildNumber;
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
    
    ui::Scale9Sprite* childFrame = ui::Scale9Sprite::create("res/settings/childFrame.png", spriteRect, capInsents);
    childFrame->setContentSize(Size(800,1200));
    childFrame->setAnchorPoint(Vec2(0,0));
    
    this->setContentSize(childFrame->getContentSize());
    
    childFrame->setPosition(Vec2(0, 0));
    
    this->addChild(childFrame);
}

void KidsLayer::addChildName()
{
    Label* childNameLabel = createLabelChildNameSettings(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber));
    reduceLabelTextToFitWidth(childNameLabel,this->getContentSize().width*.95);
    childNameLabel->setPosition(this->getContentSize().width/2,this->getContentSize().height-childNameLabel->getContentSize().height*1.7);
    this->addChild(childNameLabel);
}

void KidsLayer::addOomee()
{
    glowSprite = createGlow();
    this->addChild(glowSprite);
    
    std::string oomeeUrl = ParentDataProvider::getInstance()->getAvatarForAnAvailableChildren(childNumber);
    int oomeeNr = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
    
    oomeeSprite = Sprite::create(StringUtils::format("res/childSelection/%s.png", ConfigStorage::getInstance()->getNameForOomee(oomeeNr).c_str()));
    this->addChild(oomeeSprite);
    
    setOomeeToLargeSize();
}

ui::Scale9Sprite* KidsLayer::createText9Sprite(std::string resource, float heightPercentage)
{
    Rect spriteRect = Rect(0, 0, 268, 107);
    Rect capInsents = Rect(100, 53, 1, 1);
    
    ui::Scale9Sprite* textBox9Sprite = ui::Scale9Sprite::create(resource, spriteRect, capInsents);
    textBox9Sprite->setContentSize(Size(this->getContentSize().width*.8, 107));
    textBox9Sprite->setPosition(Vec2(this->getContentSize().width/2, textBox9Sprite->getContentSize().height*heightPercentage));
    textBox9Sprite->setTag(1000);
    
    return textBox9Sprite;
}

//------------------SET OOMEE POSITION------------

void KidsLayer::setOomeeToLargeSize()
{
    Vec2 position = Vec2(this->getContentSize().width / 2, this->getContentSize().height*.6);
    
    glowSprite->setPosition(position);
    glowSprite->setScale(.7);
    
    oomeeSprite->setScale(1.8);
    oomeeSprite->setPosition(position);
}

void KidsLayer::setOomeeToSmallSize()
{
    Vec2 position = Vec2(this->getContentSize().width / 2, this->getContentSize().height*.67);
    
    glowSprite->setPosition(position);
    glowSprite->setScale(.5);
    
    oomeeSprite->setScale(1.1);
    oomeeSprite->setPosition(position);
}

//------------------PUBLIC CHANGE STATE-------------

void KidsLayer::setToShowingCode()
{
    removeObjects(true);
    
    Label* detailsLabel = createLabelSettingsChat("Share your Kid Code\nwith all your friends",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,oomeeSprite->getPositionY() - (oomeeSprite->getContentSize().height/2 * oomeeSprite->getScale()) - detailsLabel->getContentSize().height);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
    
    Label* codeLabel = createLabelKidCode(ParentDataProvider::getInstance()->getInviteCodeForAvailableChildren(childNumber),Color3B(28, 244, 244));
    codeLabel->setPosition(this->getContentSize().width/2,detailsLabel->getPositionY()- detailsLabel->getContentSize().height/2 -codeLabel->getContentSize().height * .8);
    codeLabel->setTag(1000);
    this->addChild(codeLabel);
}

void KidsLayer::setToAddAFriend()
{
    removeObjects(true);
    
    Label* detailsLabel = createLabelSettingsChat("To add a friend",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,oomeeSprite->getPositionY() - (oomeeSprite->getContentSize().height/2 * oomeeSprite->getScale()) - detailsLabel->getContentSize().height*2.5);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
}

void KidsLayer::setToAddAFriendTextBox()
{
    removeObjects(true);
    
    Label* detailsLabel = createLabelSettingsChat("To add a friend",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,oomeeSprite->getPositionY() - (oomeeSprite->getContentSize().height/2 * oomeeSprite->getScale()) - detailsLabel->getContentSize().height*2.5);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
}

void KidsLayer::setToCodeError(std::string code)
{
    removeObjects(false);
    
    Label* failedTextBoxLabel = createLabelKidCode(code,Color3B(249, 74, 91));
    
    ui::Scale9Sprite* failedTextBox = createText9Sprite("res/settings/textFieldFail.png",4);
    
    failedTextBoxLabel->setPosition(failedTextBox->getContentSize().width/2, failedTextBox->getContentSize().height/2);
    
    failedTextBox->addChild(failedTextBoxLabel);
    this->addChild(failedTextBox);
    
    Label* detailsLabel = createLabelSettingsChat("Oh No! We don't recognise\nthis code",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,failedTextBox->getPositionY() + detailsLabel->getContentSize().height*1.25);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
    
}

void KidsLayer::setToCodeSuccess(std::string code)
{
    removeObjects(false);
    
    ui::Scale9Sprite* displayCodeTextBox = createText9Sprite("res/settings/textEntry.png",4.5);
    
    Label* failedTextBoxLabel = createLabelHeader(code);
    failedTextBoxLabel->setColor(Color3B(55, 188, 152));
    failedTextBoxLabel->setPosition(displayCodeTextBox->getContentSize().width/2, displayCodeTextBox->getContentSize().height/2-5);
    
    displayCodeTextBox->addChild(failedTextBoxLabel);
    this->addChild(displayCodeTextBox);
    
    Label* titleLabel = createLabelSettingsChat("Kid Code accepted",Color3B::WHITE);
    titleLabel->setPosition(this->getContentSize().width/2,displayCodeTextBox->getPositionY() + titleLabel->getContentSize().height*2.25);
    titleLabel->setTag(1000);
    this->addChild(titleLabel);
    
    Label* detailsLabel = createLabelSettingsChat("Your friend has been added!\nTheir parents will need to\nconfirm the connection.",Color3B::WHITE);
    detailsLabel->setPosition(this->getContentSize().width/2,displayCodeTextBox->getPositionY() - detailsLabel->getContentSize().height);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
}

void KidsLayer::removeObjects(bool oomeeToLargeSize)
{
    while(this->getChildByTag(1000))
        this->removeChildByTag(1000);
    
    if(oomeeToLargeSize)
        setOomeeToLargeSize();
    else
        setOomeeToSmallSize();
}

NS_AZOOMEE_END

