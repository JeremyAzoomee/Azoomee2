//
//  MeHQMessages.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 21/05/2018.
//

#include "MeHQMessages.h"
#include "ChatDelegate.h"
#include "SceneManagerScene.h"
#include "IAPFlowController.h"
#include "DynamicNodeHandler.h"
#include "HQDataProvider.h"
#include <AzoomeeChat/UI/AvatarWidget.h>
#include <AzoomeeChat/UI/MessageScene.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>

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
    
    ui::Text* heading = ui::Text::create("Last Messages", Style::Font::Regular, 100);
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
    if(isPortrait && (visibleSize.height / visibleSize.width) < 16.0/10.5)
    {
        isPortrait = false;
    }
    
    float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreen();
    float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    float totalHeight = 200;
    
    Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
    float unitWidth = (visibleSize.width - 2 * sideMargin) / unitsOnScreen;
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    ui::Layout* messageLayout = ui::Layout::create();
    messageLayout->setContentSize(Size(visibleSize.width - 2 * sideMargin, isPortrait ? 300 : contentItemSize.height * unitMultiplier));
    messageLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
    Chat::FriendRef fakeAcc = Chat::Friend::create("", "Azoomee",ConfigStorage::getInstance()->getUrlForOomee(4));
    
    Chat::AvatarWidget* avatar = Chat::AvatarWidget::create();
    avatar->setAvatarForFriend(fakeAcc);
    avatar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    avatar->setNormalizedPosition(Vec2(isPortrait ? 0.15 : 0.1,0.5));
    avatar->setContentSize(isPortrait ? Size(200,200) : Size(300,300));
    messageLayout->addChild(avatar);
    
    float textPos = isPortrait ? 0.30 : 0.2;
    float maxWidth = this->getContentSize().width * ((isPortrait ? 1 : 0.75) - textPos - 0.05);
    
    Label* senderName = Label::createWithTTF("Azoomee", Style::Font::Regular, isPortrait ? 77 : 97);
    senderName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    senderName->setNormalizedPosition(Vec2(textPos,0.66));
    
    reduceLabelTextToFitWidth(senderName, maxWidth);
    
    messageLayout->addChild(senderName);
    
    Label* messageText = Label::createWithTTF("Welcome to your Me Page!", Style::Font::Regular, isPortrait ? 62 : 82);
    messageText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    messageText->setNormalizedPosition(Vec2(textPos,0.33));
    
    reduceLabelTextToFitWidth(messageText, maxWidth);
    
    messageLayout->addChild(messageText);
    
    this->addChild(messageLayout);
    
    totalHeight += messageLayout->getContentSize().height;
    
    ui::Scale9Sprite* bgFrame = ui::Scale9Sprite::create("res/meHQ/chat_bg.png");
    bgFrame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    bgFrame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    bgFrame->setContentSize(Size((visibleSize.width - 2 * sideMargin) * (isPortrait ? 1.0f : 0.75f) - contentItemMargin, messageLayout->getContentSize().height));
    bgFrame->setColor(Style::Color::darkTeal);
    messageLayout->addChild(bgFrame, -1);
    
    if(!isPortrait)
    {
        ui::Button* chatButton = ui::Button::create("res/meHQ/send_message_button.png");
        chatButton->setScale( ((contentItemSize.width - contentItemMargin) * unitMultiplier) / chatButton->getContentSize().width);
        chatButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                const HQDataObjectRef &currentObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kChatHQName);
                
                if(!currentObject->getHqEntitlement())
                {
                    AnalyticsSingleton::getInstance()->registerCTASource("lockedHQ","",currentObject->getHqType());
                    IAPEntryContext context = IAPEntryContext::LOCKED_CHAT;
                    DynamicNodeHandler::getInstance()->startIAPFlow(context);
                }
                else
                {
                    Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
                }
            }
        });
    
        chatButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        chatButton->setNormalizedPosition(Vec2((visibleSize.width - sideMargin) / visibleSize.width, 0.5));
        messageLayout->addChild(chatButton);
    }
    else
    {
        ui::Button* chatButton = ui::Button::create("res/meHQ/send_messgae_button_no_icon.png");
        chatButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
        chatButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        chatButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                const HQDataObjectRef &currentObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kChatHQName);
                
                if(!currentObject->getHqEntitlement())
                {
                    AnalyticsSingleton::getInstance()->registerCTASource("lockedHQ","",currentObject->getHqType());
                    IAPEntryContext context = IAPEntryContext::LOCKED_CHAT;
                    DynamicNodeHandler::getInstance()->startIAPFlow(context);
                }
                else
                {
                    Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
                }
            }
        });
        this->addChild(chatButton);
        totalHeight += chatButton->getContentSize().height + 100;
    }
    
    this->setContentSize(Size(visibleSize.width ,totalHeight));
}


void MeHQMessages::createMessageList()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    bool isPortrait = visibleSize.width < visibleSize.height;
    
    float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    
    float totalHeight = 200;
    
    this->removeAllChildren();
    
    ui::Text* heading = ui::Text::create("Last Messages", Style::Font::Regular, 100);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
    heading->setContentSize(Size(this->getContentSize().width, 200));
    this->addChild(heading);
    
    if(_messages.size() > 0)
    {
        int i = 0;
        float messageLayoutHeight = 0;
        for(const Chat::MessageRef& message : _messages)
        {
            if(i != 0)
            {
                ui::Layout* divider = ui::Layout::create();
                divider->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,0)));
                divider->setContentSize(Size(visibleSize.width - (10 * sideMargin), 6));
                divider->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
                divider->setBackGroundColor(Style::Color::white);
                divider->setOpacity(50);
                this->addChild(divider);
                messageLayoutHeight += 6;
                totalHeight += 6;
            }
            i++;
            
            ui::Layout* messageLayout = ui::Layout::create();
            messageLayout->setContentSize(Size(visibleSize.width - (2 * sideMargin), 300));
            messageLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
            
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
            avatar->setContentSize(Size(200,200));
            messageLayout->addChild(avatar);
            
            float textPos = isPortrait ? 0.30 : 0.2;
            float maxWidth = this->getContentSize().width * (1 - textPos - 0.05);
            
            Label* senderName = Label::createWithTTF(message->senderName(), Style::Font::Regular, 77);
            senderName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            senderName->setNormalizedPosition(Vec2(textPos,0.66));
            
            reduceLabelTextToFitWidth(senderName, maxWidth);
            
            messageLayout->addChild(senderName);
            
            Label* messageText = Label::createWithTTF("", Style::Font::Regular, 62);
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
            messageLayoutHeight += messageLayout->getContentSize().height;
        }
    
        this->setContentSize(Size(visibleSize.width ,totalHeight));
        
        ui::Scale9Sprite* bgFrame = ui::Scale9Sprite::create("res/meHQ/chat_bg.png");
        bgFrame->setPosition(visibleSize.width / 2, 50);
        bgFrame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        bgFrame->setContentSize(Size(visibleSize.width - (2 * sideMargin), messageLayoutHeight));
        bgFrame->setColor(Style::Color::darkTeal);
        this->addChild(bgFrame, -1);
        
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
