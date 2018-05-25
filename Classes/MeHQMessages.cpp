//
//  MeHQMessages.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 21/05/2018.
//

#include "MeHQMessages.h"
#include "ChatDelegate.h"
#include <AzoomeeChat/UI/AvatarWidget.h>
#include <AzoomeeChat/UI/MessageScene.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool MeHQMessages::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setContentSize(Size(Director::getInstance()->getVisibleSize().width, 200));
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    ui::Text* heading = ui::Text::create("My Last Messages", Style::Font::Regular, 150);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
    heading->setContentSize(Size(this->getContentSize().width, 200));
    this->addChild(heading);
    
    return true;
}

void MeHQMessages::onEnter()
{
    Super::onEnter();
    Chat::ChatAPI::getInstance()->registerObserver(this);
    _friendList = Chat::ChatAPI::getInstance()->getFriendList();
    //Director::getInstance()->getScheduler()->schedule([](float deltaT){
        Chat::ChatAPI::getInstance()->getTimelineSummary();
    //}, this, 0, 0, 0.1, false, "getTimelineSummary");
    
}

void MeHQMessages::onExit()
{
    Super::onExit();
    Chat::ChatAPI::getInstance()->removeObserver(this);
}

void MeHQMessages::onSizeChanged()
{
    
}

void MeHQMessages::setRefreshCallback(const RefreshLayoutCallback& callback)
{
    _refreshCallback = callback;
}

void MeHQMessages::createMessageList()
{
    float totalHeight = 200;
    
    this->removeAllChildren();
    
    ui::Text* heading = ui::Text::create("My Last Messages", Style::Font::Regular, 150);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    heading->setContentSize(Size(this->getContentSize().width, 200));
    this->addChild(heading);
    
    for(const Chat::MessageRef& message : _messages)
    {
        ui::Layout* messageLayout = ui::Layout::create();
        messageLayout->setContentSize(Size(this->getContentSize().width * 0.9, 500));
        //messageLayout->setLayoutType(ui::Layout::Type::RELATIVE);
        messageLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
        messageLayout->setBackGroundImage("res/decoration/message_bar_container.png");
        messageLayout->setBackGroundImageScale9Enabled(true);
        
        Chat::AvatarWidget* avatar = Chat::AvatarWidget::create();
        
        auto friendIt = std::find_if(_friendList.begin(), _friendList.end(), [&](const Chat::FriendRef& friendObj){
            return friendObj->friendId() == message->senderId();
        });
        
        if(friendIt == _friendList.end())
        {
            continue;
        }
        
        avatar->setAvatarForFriend(*friendIt);
        avatar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        avatar->setNormalizedPosition(Vec2(0.12,0.5));
        avatar->setContentSize(Size(300,300));
        messageLayout->addChild(avatar);
        
        ui::Text* senderName = ui::Text::create(message->senderName(), Style::Font::Regular, 120);
        senderName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        senderName->setNormalizedPosition(Vec2(0.25,0.66));
        messageLayout->addChild(senderName);
        
        ui::Text* messageText = ui::Text::create("", Style::Font::Regular, 80);
        std::string text = message->messageText();
        if(message->messageType() == Chat::Message::MessageTypeArt)
        {
            text = "Sent a picture";
        }
        else if(message->messageType() == Chat::Message::MessageTypeSticker)
        {
            text = "Sent a Sticker";
        }
        else if(message->messageType() == Chat::Message::MessageTypeContent)
        {
            text = "Sent a Link";
        }
        
        messageText->setString(text);
        messageText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        messageText->setNormalizedPosition(Vec2(0.25,0.33));
        messageLayout->addChild(messageText);
        
        
        this->addChild(messageLayout);
        
        messageLayout->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType)
        {
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                Azoomee::Chat::delegate = ChatDelegate::getInstance();
                
                const bool isParent = friendIt->get()->friendId() == ParentDataProvider::getInstance()->getLoggedInParentId();
                AnalyticsSingleton::getInstance()->setChatFriendIsParent(isParent);
                AnalyticsSingleton::getInstance()->genericButtonPressEvent(isParent ? "ChatScene - SelectedParent" : "ChatScene - SelectedFriend");
                
                AnalyticsSingleton::getInstance()->contentItemSelectedEvent("CHAT");
                
                const std::string& childId = ChildDataProvider::getInstance()->getParentOrChildId();
                const std::string& childName = ChildDataProvider::getInstance()->getLoggedInChildName();
                const std::string& childAvatar = ChildDataProvider::getInstance()->getParentOrChildAvatarId();
                Chat::FriendRef currentUser = Chat::Friend::create(childId, childName, childAvatar);
                
                Chat::FriendList participants = { currentUser, *friendIt };
                auto messageScene = Chat::MessageScene::create(participants);
                Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, messageScene));
            }
        });
        messageLayout->setTouchEnabled(true);
        
        totalHeight += messageLayout->getContentSize().height + 100;
    }
    
    this->setContentSize(Size(this->getContentSize().width ,totalHeight));
    
    if(_refreshCallback)
    {
        _refreshCallback();
    }
}
/// Get Timeline Summary response
void MeHQMessages::onChatAPIGetTimelineSummary(const Chat::MessageList& messageList)
{
    _messages = messageList;
    createMessageList();
}
/// API error from Chat request
void MeHQMessages::onChatAPIErrorRecieved(const std::string& requestTag, long errorCode)
{
    
}

NS_AZOOMEE_END
