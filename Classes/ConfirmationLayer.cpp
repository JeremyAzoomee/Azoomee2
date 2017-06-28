#include "ConfirmationLayer.h"
#include "ui/UIScale9Sprite.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Azoomee.h>

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
    topLine->drawLine(Vec2(0, this->getContentSize().height), Vec2(this->getContentSize().width,this->getContentSize().height), Color4F(150.96/255.0f, 150.96/255.0f, 150.06/255.0f,255/255.0f));
    this->addChild(topLine);
    
    DrawNode* bottomLine = DrawNode::create();
    bottomLine->setLineWidth(2);
    bottomLine->drawLine(Vec2(0, 0), Vec2(this->getContentSize().width,0), Color4F(150.96/255.0f, 150.96/255.0f, 150.06/255.0f,255/255.0f));
    this->addChild(bottomLine);
    
    childNameLabel = createLabelBody(setChildName);
    childNameLabel->setWidth(385);
    reduceLabelTextToFitWidth(childNameLabel,childNameLabel->getWidth());
    childNameLabel->setPosition(MARGIN+childNameLabel->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(childNameLabel);
    
    connectorSprite = Sprite::create("res/settings/connectIcon.png");
    connectorSprite->setPosition(MARGIN+385+connectorSprite->getContentSize().width/2,this->getContentSize().height/2);
    this->addChild(connectorSprite);
    
    friendNameLabel = createLabelBody(setFriendName);
    reduceLabelTextToFitWidth(childNameLabel,385);
    friendNameLabel->setPosition(connectorSprite->getPositionX()+connectorSprite->getContentSize().width/2+MARGIN+ friendNameLabel->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(friendNameLabel);
    
    friendCodeLabel = createLabelSettingsChat(StringUtils::format("  /  %s",setFriendCode.c_str()), Color3B(28, 244, 244));
    friendCodeLabel->setPosition(friendNameLabel->getPositionX()+friendNameLabel->getContentSize().width/2+ friendCodeLabel->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(friendCodeLabel);
}

void ConfirmationLayer::setToConfirm()
{
    clearUIItems();
    createRect(Color4F(246/255.0f, 187/255.0f, 66/255.0f,255/255.0f));
    
    Label* rejectRequest = createLabelBody(StringUtils::format("%s & %s are now connected",childNameLabel->getString().c_str(),friendNameLabel->getString().c_str()));
    rejectRequest->setTag(1000);
    rejectRequest->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(rejectRequest);
}

void ConfirmationLayer::setToReject()
{
    clearUIItems();
    createRect(Color4F(249/255.0f, 74/255.0f, 91/255.0f,255/255.0f));
    
    childNameLabel->setColor(Color3B(249,74,91));
    friendNameLabel->setColor(Color3B(249,74,91));
    childNameLabel->setVisible(true);
    friendNameLabel->setVisible(true);
    
    Label* rejectRequest = createLabelBody("Reject this request?");
    rejectRequest->setTag(1000);
    rejectRequest->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(rejectRequest);
}

void ConfirmationLayer::setToRejected()
{
    clearUIItems();
    createRect(Color4F(249/255.0f, 74/255.0f, 91/255.0f,255/255.0f));
    
    childNameLabel->setVisible(false);
    friendNameLabel->setVisible(false);
    
    Label* rejectRequest = createLabelBody("Rejected");
    rejectRequest->setTag(1000);
    rejectRequest->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    this->addChild(rejectRequest);
}

void ConfirmationLayer::setIdle()
{
    clearUIItems();
    childNameLabel->setColor(Color3B::WHITE);
    friendNameLabel->setColor(Color3B::WHITE);
    
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
