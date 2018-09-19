#include "KidsLayer.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

KidsLayer* KidsLayer::createWithChildDetails(int setChildNumber)
{
    auto layer = KidsLayer::create();
    layer->childNumber = setChildNumber;
    layer->addFrame();
    layer->addChildName();
    layer->addOomee();
    layer->setToStartLayout();
    
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
    childFrame->setContentSize(Size(800,900));
    childFrame->setAnchorPoint(Vec2(0,0));
    
    this->setContentSize(childFrame->getContentSize());
    
    childFrame->setPosition(Vec2(0, 50));
    
    this->addChild(childFrame);
}

void KidsLayer::addChildName()
{
    Label* childNameLabel = createLabelChildNameSettings(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(childNumber));
    reduceLabelTextToFitWidth(childNameLabel,this->getContentSize().width*.95);
    childNameLabel->setPosition(this->getContentSize().width / 2, this->getContentSize().height - childNameLabel->getContentSize().height * 0.2);
    this->addChild(childNameLabel);
}

void KidsLayer::addOomee()
{
    const std::string& oomeeUrl = ParentDataProvider::getInstance()->getAvatarForAnAvailableChild(childNumber);
    //int oomeeNr = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
    
    //oomeeSprite = Sprite::create(StringUtils::format("res/childSelection/%s.png", ConfigStorage::getInstance()->getNameForOomee(oomeeNr).c_str()));
    auto oomee = RemoteImageSprite::create();
    oomee->initWithUrlAndSizeWithoutPlaceholder(oomeeUrl, Size(256, 256));
    oomee->setKeepAspectRatio(true);
    oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    oomeeSprite = oomee;
    this->addChild(oomeeSprite);
    
    setOomeeToLargeSize();
}

ui::Scale9Sprite* KidsLayer::createText9Sprite(std::string resource, float heightPercentage)
{
    Rect spriteRect = Rect(0, 0, 268, 107);
    Rect capInsents = Rect(100, 53, 1, 1);
    
    ui::Scale9Sprite* textBox9Sprite = ui::Scale9Sprite::create(resource, spriteRect, capInsents);
    textBox9Sprite->setContentSize(Size(this->getContentSize().width * 0.8f, 107));
    textBox9Sprite->setPosition(Vec2(this->getContentSize().width / 2, textBox9Sprite->getContentSize().height*heightPercentage));
    textBox9Sprite->setTag(1000);
    
    return textBox9Sprite;
}

//------------------SET OOMEE POSITION------------

void KidsLayer::setOomeeToLargeSize()
{
    oomeeSprite->setScale(1.8);
    oomeeSprite->setPosition(this->getContentSize().width / 2, this->getContentSize().height * 0.53f);
}

void KidsLayer::setOomeeToSmallSize()
{
    oomeeSprite->setScale(1.1);
    oomeeSprite->setPosition(this->getContentSize().width / 2, this->getContentSize().height * 0.67f);
}

//------------------PUBLIC CHANGE STATE-------------

void KidsLayer::setToStartLayout()
{
    removeObjects(true);
    
    Label* ShareWithFriendLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(SETTINGS_SHARE_KID_CODE), Style::Font::Regular, Style::Color::black, 40);
    ShareWithFriendLabel->setTag(1000);
    ShareWithFriendLabel->setPosition(this->getContentSize().width / 2, this->getContentSize().height - ShareWithFriendLabel->getContentSize().height * 2.0f);
    this->addChild(ShareWithFriendLabel);
    
    Label* AddAFriendLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(SETTINGS_ADD_FRIEND), Style::Font::Regular, Style::Color::black, 40);
    AddAFriendLabel->setTag(1000);
    AddAFriendLabel->setPosition(this->getContentSize().width/2,AddAFriendLabel->getContentSize().height * 5.8f);
    this->addChild(AddAFriendLabel);
}

void KidsLayer::setToAddAFriendTextBox()
{
    removeObjects(true);
    Label* detailsLabel = createLabelSettingsChat(StringMgr::getInstance()->getStringForKey(SETTINGS_ADD_FRIEND),Style::Color::black);
    detailsLabel->setPosition(this->getContentSize().width/2,oomeeSprite->getPositionY() - (oomeeSprite->getContentSize().height/2 * oomeeSprite->getScale()) - detailsLabel->getContentSize().height*1.5);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
}

void KidsLayer::setToCodeError(std::string code)
{
    removeObjects(false);
    
    Label* failedTextBoxLabel = createLabelKidCode(code,Style::Color::watermelon);
    
    ui::Scale9Sprite* failedTextBox = createText9Sprite("res/settings/textFieldFail.png",4);
    
    failedTextBoxLabel->setPosition(failedTextBox->getContentSize().width/2, failedTextBox->getContentSize().height/2);
    
    failedTextBox->addChild(failedTextBoxLabel);
    this->addChild(failedTextBox);
    
    Label* detailsLabel = createLabelSettingsChat(StringMgr::getInstance()->getStringForKey(SETTINGS_CODE_UNRECOGNISED),Style::Color::black);
    detailsLabel->setPosition(this->getContentSize().width/2,failedTextBox->getPositionY() + detailsLabel->getContentSize().height*1.25);
    detailsLabel->setTag(1000);
    this->addChild(detailsLabel);
    
}

void KidsLayer::setToCodeSuccess(std::string code)
{
    removeObjects(false);
    
    ui::Scale9Sprite* displayCodeTextBox = createText9Sprite("res/settings/textEntry.png",4.5);
    
    Label* failedTextBoxLabel = createLabelHeader(code);
    failedTextBoxLabel->setColor(Style::Color::greenishTeal);
    failedTextBoxLabel->setPosition(displayCodeTextBox->getContentSize().width/2, displayCodeTextBox->getContentSize().height/2-5);
    
    displayCodeTextBox->addChild(failedTextBoxLabel);
    this->addChild(displayCodeTextBox);
    
    Label* titleLabel = createLabelSettingsChat(StringMgr::getInstance()->getStringForKey(SETTINGS_CODE_ACCEPTED),Style::Color::black);
    titleLabel->setPosition(this->getContentSize().width/2,displayCodeTextBox->getPositionY() + titleLabel->getContentSize().height*2.25);
    titleLabel->setTag(1000);
    this->addChild(titleLabel);
    
    Label* detailsLabel = createLabelSettingsChat(StringMgr::getInstance()->getStringForKey(SETTINGS_CODE_ACCEPTED_DETAILS),Style::Color::black);
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

