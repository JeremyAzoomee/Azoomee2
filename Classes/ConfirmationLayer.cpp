#include "ConfirmationLayer.h"
#include "ui/UIScale9Sprite.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

#define MARGIN 36

bool ConfirmationLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//---------------------ADD DETAILS---------------------------------

void ConfirmationLayer::addDetailsToLayer(std::string setChildName, std::string setFriendName, std::string setFriendCode)
{
    DrawNode* topLine = DrawNode::create();
    topLine->setLineWidth(2);
    topLine->drawLine(Vec2(0, this->getContentSize().height), Vec2(this->getContentSize().width,this->getContentSize().height), Style::Color_4F::settingsTopBottomLines);
    this->addChild(topLine);
    
    DrawNode* bottomLine = DrawNode::create();
    bottomLine->setLineWidth(2);
    bottomLine->drawLine(Vec2(0, 0), Vec2(this->getContentSize().width,0), Style::Color_4F::settingsTopBottomLines);
    this->addChild(bottomLine);
    
    childNameLabel = createLabelBody(setChildName,Color3B::BLACK);
    childNameLabel->setWidth(385);
    reduceLabelTextToFitWidth(childNameLabel,childNameLabel->getWidth());
    childNameLabel->setPosition(MARGIN+childNameLabel->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(childNameLabel);
    
    connectorSprite = Sprite::create("res/settings/connectIcon.png");
    connectorSprite->setPosition(MARGIN+385+connectorSprite->getContentSize().width/2,this->getContentSize().height/2);
    this->addChild(connectorSprite);
    
    friendNameLabel = createLabelBody(setFriendName,Color3B::BLACK);
    reduceLabelTextToFitWidth(childNameLabel,385);
    friendNameLabel->setPosition(connectorSprite->getPositionX()+connectorSprite->getContentSize().width/2+MARGIN+ friendNameLabel->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(friendNameLabel);
    
    friendCodeLabel = createLabelSettingsChat(StringUtils::format("  /  %s",setFriendCode.c_str()), Color3B::BLACK);
    friendCodeLabel->setPosition(friendNameLabel->getPositionX()+friendNameLabel->getContentSize().width/2+ friendCodeLabel->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(friendCodeLabel);
}

void ConfirmationLayer::setToConfirm()
{
    clearUIItems();
    createRect(Style::Color_4F::macaroniAndCheese);
    
    Label* rejectRequest = createLabelBody(StringUtils::format("%s & %s are now friends and can chat!",childNameLabel->getString().c_str(),friendNameLabel->getString().c_str()),Color3B::BLACK);
    rejectRequest->setTag(1000);
    rejectRequest->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(rejectRequest);
}

void ConfirmationLayer::setToReject()
{
    clearUIItems();
    createRect(Style::Color_4F::watermelon);
    
    childNameLabel->setColor(Style::Color::watermelon);
    friendNameLabel->setColor(Style::Color::watermelon);
    childNameLabel->setVisible(true);
    friendNameLabel->setVisible(true);
    
    Label* rejectRequest = createLabelBody("Reject this friendship?",Color3B::BLACK);
    rejectRequest->setTag(1000);
    rejectRequest->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(rejectRequest);
}

void ConfirmationLayer::setToRejected()
{
    clearUIItems();
    createRect(Style::Color_4F::watermelon);
    
    childNameLabel->setVisible(false);
    friendNameLabel->setVisible(false);
    
    Label* rejectRequest = createLabelBody("Rejected",Style::Color::watermelon);
    rejectRequest->setTag(1000);
    rejectRequest->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(rejectRequest);
}

void ConfirmationLayer::setIdle()
{
    clearUIItems();
    childNameLabel->setColor(Color3B::BLACK);
    friendNameLabel->setColor(Color3B::BLACK);
    
    childNameLabel->setVisible(true);
    friendNameLabel->setVisible(true);
    friendCodeLabel->setVisible(true);
    connectorSprite->setVisible(true);
}

//-----------PRIVATE FUNCTIONS-----------------

void ConfirmationLayer::clearUIItems()
{
    while(this->getChildByTag(1000))
        this->removeChildByTag(1000);

    childNameLabel->setVisible(false);
    friendNameLabel->setVisible(false);
    friendCodeLabel->setVisible(false);
    connectorSprite->setVisible(false);
}

void ConfirmationLayer::createRect(Color4F withColour)
{
    DrawNode* confirmRect = DrawNode::create();
    confirmRect->setLineWidth(2);
    confirmRect->setTag(1000);
    confirmRect->drawRect(Vec2(0, 0), Vec2(this->getContentSize().width, this->getContentSize().height), withColour);
    this->addChild(confirmRect);
}

NS_AZOOMEE_END
