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
    
    return true;
}

void FriendRequestLayer::onEnter()
{
	const Size& contentSize = this->getContentSize();
	
    ui::Scale9Sprite* stencil = ui::Scale9Sprite::create("res/settings/rounded_rect.png");
	stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    stencil->setContentSize(contentSize);
    
    _contentClippingNode = ClippingNode::create(stencil);
    _contentClippingNode->setContentSize(contentSize);
	_contentClippingNode->setAlphaThreshold(0.5f);
    
    this->addChild(_contentClippingNode);
	
	ui::Layout* contentLayout = ui::Layout::create();
	contentLayout->setBackGroundColor(Color3B::WHITE);
	contentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	contentLayout->setContentSize(contentSize);
	_contentClippingNode->addChild(contentLayout);
	
    _senderText = Label::createWithTTF(StringUtils::format("%s\n%s",_friendRequest->getSenderName().c_str(), ""), Style::Font::Medium(), 48);//sender code not available in friend request obj, leave as empty string for formatting, might go in at some point.
    _senderText->setNormalizedPosition(Vec2(0.25,0.66));
    _senderText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _senderText->setHorizontalAlignment(TextHAlignment::CENTER);
    _senderText->setTextColor(Color4B::BLACK);
    contentLayout->addChild(_senderText);
    
    _recipientText = Label::createWithTTF(StringUtils::format("%s\n%s",_friendRequest->getFriendName().c_str(), _friendRequest->getInviteeCode().c_str()), Style::Font::Medium(), 48);
    _recipientText->setNormalizedPosition(Vec2(0.75,0.66));
    _recipientText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _recipientText->setHorizontalAlignment(TextHAlignment::CENTER);
    _recipientText->setTextColor(Color4B::BLACK);
    contentLayout->addChild(_recipientText);
    
    _arrowIcon = Sprite::create("res/settings/FriendshipIconWhite.png");
    _arrowIcon->setNormalizedPosition(Vec2(0.5,0.66));
    _arrowIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _arrowIcon->setColor(Style::Color::battleshipGrey);
    contentLayout->addChild(_arrowIcon);
    
    
    _rejectButton = ui::Layout::create();
    _rejectButton->setBackGroundColor(Style::Color::watermelon);
    _rejectButton->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rejectButton->setContentSize(Size((contentSize.width * 0.5f) - 4, contentSize.height * 0.33f));
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
    contentLayout->addChild(_rejectButton);
    
    Label* rejectLabel = Label::createWithTTF(_("Reject"), Style::Font::Medium(), 59);
    rejectLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rejectLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    rejectLabel->setTextColor(Color4B::WHITE);
    rejectLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _rejectButton->addChild(rejectLabel);
    
    _confirmButton = ui::Layout::create();
    _confirmButton->setBackGroundColor(Style::Color::skyBlue);
    _confirmButton->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _confirmButton->setContentSize(Size((contentSize.width * 0.5f) - 4, contentSize.height * 0.33f));
    _confirmButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _confirmButton->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
    _confirmButton->setTouchEnabled(true);
    _confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            HttpRequestCreator* request = API::FriendRequestReaction(true, _friendRequest->getRespondentId(), _friendRequest->getId(), _friendRequest->getSenderName(), this);
            request->execute();
        }
    });
    contentLayout->addChild(_confirmButton);
    
    Label* confirmLabel = Label::createWithTTF(_("Confirm"), Style::Font::Medium(), 59);
    confirmLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    confirmLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    confirmLabel->setTextColor(Color4B::WHITE);
    confirmLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _confirmButton->addChild(confirmLabel);
    
    _rejectedBanner = ui::Layout::create();
    _rejectedBanner->setBackGroundColor(Style::Color::watermelon);
    _rejectedBanner->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rejectedBanner->setContentSize(Size(contentSize.width, contentSize.height * 0.33f));
    _rejectedBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _rejectedBanner->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    contentLayout->addChild(_rejectedBanner);
    
    Label* rejectedLabel = Label::createWithTTF(_("This request has been rejected"), Style::Font::Medium(), 59);
    rejectedLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rejectedLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    rejectedLabel->setTextColor(Color4B::WHITE);
    rejectedLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	rejectedLabel->setVerticalAlignment(TextVAlignment::CENTER);
	rejectedLabel->setOverflow(Label::Overflow::SHRINK);
	rejectedLabel->setDimensions(_rejectedBanner->getContentSize().width * 0.8f, _rejectedBanner->getContentSize().height * 0.7f);
    _rejectedBanner->addChild(rejectedLabel);
    
    _confirmedBanner = ui::Layout::create();
    _confirmedBanner->setBackGroundColor(Style::Color::skyBlue);
    _confirmedBanner->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _confirmedBanner->setContentSize(Size(contentSize.width, contentSize.height * 0.33f));
    _confirmedBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _confirmedBanner->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    contentLayout->addChild(_confirmedBanner);
    
    Label* confirmedLabel = Label::createWithTTF(StringUtils::format(_("%s can now chat with this friend!").c_str(), _friendRequest->getFriendName().c_str()) ,Style::Font::Medium() , 59);
    confirmedLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    confirmedLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    confirmedLabel->setTextColor(Color4B::WHITE);
    confirmedLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	confirmedLabel->setVerticalAlignment(TextVAlignment::CENTER);
	confirmedLabel->setOverflow(Label::Overflow::SHRINK);
	confirmedLabel->setDimensions(_confirmedBanner->getContentSize().width * 0.8f, _confirmedBanner->getContentSize().height * 0.7f);
    _confirmedBanner->addChild(confirmedLabel);
    
    changeToState(InviteState::PENDING);
    
    Super::onEnter();
}

void FriendRequestLayer::setChildDetails(const FriendRequestRef& friendRequest)
{
	_friendRequest = friendRequest;
}

void FriendRequestLayer::changeToState(Azoomee::InviteState state)
{
    _state = state;
	
	_confirmedBanner->setVisible(_state == InviteState::CONFIRMED);
	_rejectedBanner->setVisible(_state == InviteState::REJECTED);
	_rejectButton->setVisible(_state == InviteState::PENDING);
	_confirmButton->setVisible(_state == InviteState::PENDING);
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
            HttpRequestCreator* request = API::FriendRequestReaction(false, _friendRequest->getRespondentId(), _friendRequest->getId(), _friendRequest->getSenderName(), this);
            request->execute();
            break;
        }
        default:
            break;
    }
}

NS_AZOOMEE_END
