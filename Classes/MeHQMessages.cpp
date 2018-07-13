//
//  MeHQMessages.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 21/05/2018.
//

#include "MeHQMessages.h"
#include "ChatDelegate.h"
#include "SceneManagerScene.h"
#include <AzoomeeChat/UI/AvatarWidget.h>
#include <AzoomeeChat/UI/MessageScene.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

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
    
    ui::Text* heading = ui::Text::create("My Last Messages", Style::Font::Regular, 115);
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
    Chat::ChatAPI::getInstance()->getTimelineSummary();
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

void MeHQMessages::buildEmptyCarousel()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    bool isPortrait = visibleSize.width < visibleSize.height;
    
    ui::ImageView* msgLogo = ui::ImageView::create("res/meHQ/mail.png");
    msgLogo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    msgLogo->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    this->addChild(msgLogo);
    
    Rect capInsents = Rect(100, 0, 286, 149);
    
    ui::Button* makePaintingButton = ui::Button::create("res/buttons/button_dark.png");
    makePaintingButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    makePaintingButton->setCapInsets(capInsents);
    makePaintingButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    makePaintingButton->setContentSize(Size(1000,250));
    makePaintingButton->setScale9Enabled(true);
    makePaintingButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
        }
    });
    
    Label* makePaintingButtonLabel = Label::createWithTTF("Send a message", Style::Font::Regular, makePaintingButton->getContentSize().height * 0.4f);
    makePaintingButtonLabel->setTextColor(Color4B::WHITE);
    makePaintingButtonLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    makePaintingButtonLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    makePaintingButton->addChild(makePaintingButtonLabel);
    
    this->addChild(makePaintingButton);
    
    ui::Text* heading = ui::Text::create(StringUtils::format("When you send messages, your%slast messages will appear here.", isPortrait ? "\n" : " "), Style::Font::Regular, 80);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    heading->setContentSize(Size(this->getContentSize().width, 200));
    this->addChild(heading);
    
    this->setContentSize(Size(visibleSize.width, 400 + 350 + msgLogo->getContentSize().height));
    
}


void MeHQMessages::createMessageList()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    bool isPortrait = visibleSize.width < visibleSize.height;
    
    float totalHeight = 200;
    
    this->removeAllChildren();
    
    ui::Text* heading = ui::Text::create("My Last Messages", Style::Font::Regular, 100);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
    heading->setContentSize(Size(this->getContentSize().width, 200));
    this->addChild(heading);
    
    if(_messages.size() > 0)
    {
        for(const Chat::MessageRef& message : _messages)
        {
            ui::Layout* messageLayout = ui::Layout::create();
            messageLayout->setContentSize(Size(visibleSize.width * 0.9, 400));
            messageLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
            //messageLayout->setBackGroundImage("res/decoration/message_bar_container.png");
            //messageLayout->setBackGroundImageScale9Enabled(true);
            
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
            avatar->setNormalizedPosition(Vec2(isPortrait ? 0.15 : 0.1,0.5));
            avatar->setContentSize(Size(300,300));
            messageLayout->addChild(avatar);
            
            float textPos = isPortrait ? 0.30 : 0.2;
            float maxWidth = this->getContentSize().width * (1 - textPos - 0.05);
            
            Label* senderName = Label::createWithTTF(message->senderName(), Style::Font::Regular, 97);
            senderName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            senderName->setNormalizedPosition(Vec2(textPos,0.66));
            
            reduceLabelTextToFitWidth(senderName, maxWidth);
            
            messageLayout->addChild(senderName);
            
            Label* messageText = Label::createWithTTF("", Style::Font::Regular, 82);
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
            messageText->setNormalizedPosition(Vec2(textPos,0.33));
            
            reduceLabelTextToFitWidth(messageText, maxWidth);
            
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
            
            totalHeight += messageLayout->getContentSize().height;
        }
    
        this->setContentSize(Size(visibleSize.width ,totalHeight));
    }
    else
    {
        buildEmptyCarousel();
    }
    
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
