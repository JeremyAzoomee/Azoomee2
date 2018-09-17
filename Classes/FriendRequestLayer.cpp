//
//  FriendRequestLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/09/2018.
//

#include "FriendRequestLayer.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/MessageBox.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool FriendRequestLayer::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColor(Color3B::WHITE);
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    
    return true;
}

void FriendRequestLayer::onEnter()
{
    //ui::Scale9Sprite* stencil = ui::Scale9Sprite::create("res/settings/rounded_rect.png");
    //stencil->setContentSize(this->getContentSize());
    
    //_contentClippingNode = ClippingNode::create(stencil);
    //_contentClippingNode->addChild(LayerColor::create(Color4B::WHITE, this->getContentSize().width, this->getContentSize().height));
    //_contentClippingNode->setContentSize(this->getContentSize());
    
    //this->addChild(_contentClippingNode);
    
    _senderText = Label::createWithTTF(StringUtils::format("%s\n%s",_senderName.c_str(), _senderInviteCode.c_str()), Style::Font::Medium, 48);
    _senderText->setNormalizedPosition(Vec2(0.25,0.66));
    _senderText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _senderText->setHorizontalAlignment(TextHAlignment::CENTER);
    _senderText->setTextColor(Color4B::BLACK);
    this->addChild(_senderText);
    //_contentClippingNode->addChild(_senderText);
    
    _recipientText = Label::createWithTTF(StringUtils::format("%s\n%s",_recipientName.c_str(), _recipientInviteCode.c_str()), Style::Font::Medium, 48);
    _recipientText->setNormalizedPosition(Vec2(0.75,0.66));
    _recipientText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _recipientText->setHorizontalAlignment(TextHAlignment::CENTER);
    _recipientText->setTextColor(Color4B::BLACK);
    this->addChild(_recipientText);
    //_contentClippingNode->addChild(_recipientText);
    
    _bodyText = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(SETTINGS_FRIEND_REQUEST_BODY), Style::Font::Medium, 48);
    _bodyText->setNormalizedPosition(Vec2(0.5,0.66));
    _bodyText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bodyText->setHorizontalAlignment(TextHAlignment::CENTER);
    _bodyText->setTextColor(Color4B(Style::Color::battleshipGrey));
    this->addChild(_bodyText);
    //_contentClippingNode->addChild(_bodyText);
    
    
    _rejectButton = ui::Layout::create();
    _rejectButton->setBackGroundColor(Style::Color::watermelon);
    _rejectButton->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rejectButton->setContentSize(Size((this->getContentSize().width * 0.5f) - 4, this->getContentSize().height * 0.33f));
    _rejectButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _rejectButton->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    _rejectButton->setTouchEnabled(true);
    _rejectButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            SettingsMessageBoxRejectFR* messageBox = SettingsMessageBoxRejectFR::create();
            messageBox->setDelegate(this);
            Director::getInstance()->getRunningScene()->addChild(messageBox,100);
        }
    });
    this->addChild(_rejectButton);
    //_contentClippingNode->addChild(_rejectButton);
    
    Label* rejectLabel = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(BUTTON_REJECT), Style::Font::Medium, 59);
    rejectLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rejectLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    rejectLabel->setTextColor(Color4B::WHITE);
    rejectLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _rejectButton->addChild(rejectLabel);
    
    _confirmButton = ui::Layout::create();
    _confirmButton->setBackGroundColor(Style::Color::skyBlue);
    _confirmButton->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _confirmButton->setContentSize(Size((this->getContentSize().width * 0.5f) - 4, this->getContentSize().height * 0.33f));
    _confirmButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _confirmButton->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
    _confirmButton->setTouchEnabled(true);
    _confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            HttpRequestCreator* request = API::FriendRequestReaction(true, _respondentId, _requestId, _senderName, this);
            request->execute();
        }
    });
    this->addChild(_confirmButton);
    //_contentClippingNode->addChild(_confirmButton);
    
    Label* confirmLabel = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(BUTTON_CONFIRM), Style::Font::Medium, 59);
    confirmLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    confirmLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    confirmLabel->setTextColor(Color4B::WHITE);
    confirmLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _confirmButton->addChild(confirmLabel);
    
    _rejectedBanner = ui::Layout::create();
    _rejectedBanner->setBackGroundColor(Style::Color::watermelon);
    _rejectedBanner->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rejectedBanner->setContentSize(Size(this->getContentSize().width, this->getContentSize().height * 0.33f));
    _rejectedBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _rejectedBanner->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->addChild(_rejectedBanner);
    //_contentClippingNode->addChild(_rejectedBanner);
    
    Label* rejectedLabel = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(SETTINGS_REJECTED), Style::Font::Medium, 59);
    rejectedLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rejectedLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    rejectedLabel->setTextColor(Color4B::WHITE);
    rejectedLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _rejectedBanner->addChild(rejectedLabel);
    
    _confirmedBanner = ui::Layout::create();
    _confirmedBanner->setBackGroundColor(Style::Color::skyBlue);
    _confirmedBanner->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _confirmedBanner->setContentSize(Size(this->getContentSize().width, this->getContentSize().height * 0.33f));
    _confirmedBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _confirmedBanner->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->addChild(_confirmedBanner);
    //_contentClippingNode->addChild(_confirmedBanner);
    
    Label* confirmedLabel = Label::createWithTTF(StringUtils::format(StringMgr::getInstance()->getStringForKey(SETTINGS_FRIENDSHIP_CONFIRMED).c_str(), _recipientName.c_str()) ,Style::Font::Medium , 59);
    confirmedLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    confirmedLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    confirmedLabel->setTextColor(Color4B::WHITE);
    confirmedLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _confirmedBanner->addChild(confirmedLabel);
    
    changeToState(InviteState::PENDING);
    
    Super::onEnter();
}

void FriendRequestLayer::setChildDetails(const std::string& senderName, const std::string& senderInviteCode, const std::string& recipientName, const std::string& recipientInviteCode, const std::string& respondentId, const std::string& requestId)
{
    _senderName = senderName;
    _senderInviteCode = senderInviteCode;
    _recipientName = recipientName;
    _recipientInviteCode = recipientInviteCode;
    _respondentId = respondentId;
    _requestId = requestId;
}

void FriendRequestLayer::changeToState(Azoomee::InviteState state)
{
    _state = state;
    
    switch (_state) {
        case InviteState::PENDING:
        {
            _confirmedBanner->setVisible(false);
            _rejectedBanner->setVisible(false);
            _rejectButton->setVisible(true);
            _confirmButton->setVisible(true);
            break;
        }
        case InviteState::CONFIRMED:
        {
            _confirmedBanner->setVisible(true);
            _rejectedBanner->setVisible(false);
            _rejectButton->setVisible(false);
            _confirmButton->setVisible(false);
            break;
        }
        case InviteState::REJECTED:
        {
            _confirmedBanner->setVisible(false);
            _rejectedBanner->setVisible(true);
            _rejectButton->setVisible(false);
            _confirmButton->setVisible(false);
            break;
        }
    }
}

// Delegate Functions

void FriendRequestLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(body.find("approval\":\"REJECTED") != std::string::npos)
    {
        AnalyticsSingleton::getInstance()->settingsConfirmationRejectedSuccess();
        changeToState(InviteState::REJECTED);
    }
    else if(body.find("approval\":\"APPROVED")!= std::string::npos)
    {
        AnalyticsSingleton::getInstance()->settingsConfirmationApprovedSuccess();
        changeToState(InviteState::CONFIRMED);
    }
}

void FriendRequestLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    AnalyticsSingleton::getInstance()->settingsConfirmationError(errorCode);
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
}

void FriendRequestLayer::onButtonPressed(SettingsMessageBox* pSender, SettingsMessageBoxButtonType type)
{
    switch(type)
    {
        case SettingsMessageBoxButtonType::CANCEL:
        {
            pSender->removeFromParent();
            break;
        }
        case SettingsMessageBoxButtonType::REJECT:
        {
            pSender->removeFromParent();
            HttpRequestCreator* request = API::FriendRequestReaction(false, _respondentId, _requestId, _senderName, this);
            request->execute();
            break;
        }
        default:
            break;
    }
}

NS_AZOOMEE_END
