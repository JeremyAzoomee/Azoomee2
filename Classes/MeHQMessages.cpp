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
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeChat/UI/AvatarWidget.h>
#include <AzoomeeChat/UI/MessageScene.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
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
	
	Sprite* icon = Sprite::create("res/meHQ/title_icon_last_messages.png");
	icon->setAnchorPoint(Vec2(1.5f,0.35f));
	icon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	
    ui::Text* heading = ui::Text::create(_("LAST MESSAGES"), Style::Font::Regular(), 75);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(icon->getContentSize().width* 0.75f,0,0,50)));
    heading->setContentSize(Size(this->getContentSize().width, 200));
    this->addChild(heading);
    heading->addChild(icon);
    
    return true;
}

void MeHQMessages::onEnter()
{
	TutorialController::getInstance()->registerDelegate(this);
    Super::onEnter();
    Chat::ChatAPI::getInstance()->registerObserver(this);
	if(HQHistoryManager::getInstance()->getHistorySize() == 1)
	{
		Chat::ChatAPI::getInstance()->requestFriendList();
	}
	else
	{
		_friendList = Chat::ChatAPI::getInstance()->getFriendList();
		Chat::ChatAPI::getInstance()->getTimelineSummary();
	}
}

void MeHQMessages::onExit()
{
	TutorialController::getInstance()->unRegisterDelegate(this);
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
    bool is3x4Device = false;
    if(isPortrait && (visibleSize.height / visibleSize.width) < HQDataProvider::getInstance()->k16x10LowerBound)
    {
        isPortrait = false;
        is3x4Device = true;
    }
    
    const float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreenMeHQ();
    const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    float totalHeight = 200;
    
    Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
    float unitWidth = (visibleSize.width - 2 * sideMargin) / unitsOnScreen;
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    ui::Layout* messageLayout = ui::Layout::create();
    messageLayout->setContentSize(Size(visibleSize.width - 2 * sideMargin, isPortrait ? 300 : contentItemSize.height * unitMultiplier));
    messageLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    messageLayout->setTouchEnabled(true);
    messageLayout->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
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
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChatEntryPointScene));
            }
        }
    });
    
    Chat::FriendRef fakeAcc = Chat::Friend::create("", "Azoomee",ConfigStorage::getInstance()->getUrlForOomee(4));
    
    Chat::AvatarWidget* avatar = Chat::AvatarWidget::create();
    avatar->setAvatarForFriend(fakeAcc);
    avatar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    avatar->setNormalizedPosition(Vec2(isPortrait ? 0.15 : 0.1,0.5));
    avatar->setContentSize(isPortrait ? Size(200,200) : Size(300,300));
    messageLayout->addChild(avatar);
    
    float textPos = isPortrait ? 0.30 : 0.2;
    float maxWidth = this->getContentSize().width * ((isPortrait ? 1 : 0.75) - textPos - 0.05);
    
    Label* senderName = Label::createWithTTF("Azoomee", Style::Font::Regular(), isPortrait ? 77 : is3x4Device ? 77 : 97);
    senderName->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    senderName->setPosition(Vec2(messageLayout->getContentSize().width * textPos,messageLayout->getContentSize().height * 0.5 + 20));
    
    reduceLabelTextToFitWidth(senderName, maxWidth);
    
    messageLayout->addChild(senderName);
    
    Label* messageText = Label::createWithTTF(_("Welcome to your Me Page!"), Style::Font::Regular(), isPortrait ? 62 : is3x4Device ? 62 : 82);
    messageText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    messageText->setPosition(Vec2(messageLayout->getContentSize().width * textPos,messageLayout->getContentSize().height * 0.5 - 20));
    messageText->setOpacity(179);
    
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
                    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChatEntryPointScene));
                }
            }
        });
    
        chatButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        chatButton->setNormalizedPosition(Vec2((visibleSize.width - sideMargin) / visibleSize.width, 0.5));
        messageLayout->addChild(chatButton);
    }
    
    this->setContentSize(Size(visibleSize.width ,totalHeight));
}


void MeHQMessages::createMessageList()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    bool isPortrait = visibleSize.width < visibleSize.height;
    
    const float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    
    float totalHeight = 200;
    
    this->removeAllChildren();
	
	Sprite* icon = Sprite::create("res/meHQ/title_icon_last_messages.png");
	icon->setAnchorPoint(Vec2(1.5f,0.35f));
	icon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	
    ui::Text* heading = ui::Text::create(_("LAST MESSAGES"), Style::Font::Regular(), 75);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(icon->getContentSize().width* 0.75f,0,0,50)));
    heading->setContentSize(Size(this->getContentSize().width, 200));
    this->addChild(heading);
    heading->addChild(icon);
    
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
            
            Label* senderName = Label::createWithTTF(message->senderName(), Style::Font::Regular(), 77);
            senderName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            senderName->setNormalizedPosition(Vec2(textPos,0.66));
            
            reduceLabelTextToFitWidth(senderName, maxWidth);
            
            messageLayout->addChild(senderName);
            
            ui::Text* messageText = ui::Text::create("", Style::Font::RegularSystemName, 62);
            std::string text = message->messageText();
            if(message->messageType() == Chat::Message::MessageTypeArt)
            {
                text = _("Sent a Picture");
            }
            else if(message->messageType() == Chat::Message::MessageTypeSticker)
            {
                text = _("Sent a Sticker");
            }
            else if(message->messageType() == Chat::Message::MessageTypeContent)
            {
                text = _("Sent a Link");
            }
            
            messageText->setString(text);
            messageText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            messageText->setNormalizedPosition(Vec2(textPos,0.33));
            messageText->setOpacity(179);
            
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
                    
                    const std::string& childId = ChildManager::getInstance()->getParentOrChildId();
                    const std::string& childName = ChildManager::getInstance()->getParentOrChildName();
                    const std::string& childAvatar = ChildManager::getInstance()->getParentOrChildAvatarId();
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
	
	if(TutorialController::getInstance()->isTutorialActive())
	{
		onTutorialStateChanged(TutorialController::getInstance()->getCurrentState());
	}
    
    if(_refreshCallback)
    {
        _refreshCallback();
    }
}

void MeHQMessages::enableButtons(bool enable)
{
	for(auto child : this->getChildren())
	{
		ui::Layout* layout = dynamic_cast<ui::Layout*>(child);
		if(layout)
		{
			layout->setTouchEnabled(enable);
		}
	}
}

/// Get Timeline Summary response
void MeHQMessages::onChatAPIGetTimelineSummary(const Chat::MessageList& messageList)
{
    _messages = messageList;
    createMessageList();
}

void MeHQMessages::onChatAPIGetFriendList(const Chat::FriendList& friendList, int amountOfNewMessages)
{
	_friendList = friendList;
	Chat::ChatAPI::getInstance()->getTimelineSummary();
}

/// API error from Chat request
void MeHQMessages::onChatAPIErrorRecieved(const std::string& requestTag, long errorCode)
{
    
}

void MeHQMessages::onTutorialStateChanged(const std::string& stateId)
{
	
}

NS_AZOOMEE_END
