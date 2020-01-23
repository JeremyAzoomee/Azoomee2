//
//  ChatHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/10/2019.
//

#include "ChatHQ.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include "AzoomeeChat/UI/MessageScene.h"
#include "ChatDelegate.h"
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const Size ChatHQ::kFriendTileSize = Size(300,450);

bool ChatHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    createRecentMessages();
    createFriendsList();
    
    const Color3B& gradColour = Style::Color::darkIndigo;
    _topScrollGradient = LayerGradient::create(Color4B(gradColour), Color4B(gradColour.r, gradColour.g, gradColour.b, 0));
    _topScrollGradient->setIgnoreAnchorPointForPosition(false);
    _topScrollGradient->setContentSize(Size(_contentListView->getContentSize().width, 0));
    _topScrollGradient->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _topScrollGradient->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    addChild(_topScrollGradient, 1);
    
    _contentListView->addEventListener([this](Ref* pSender, ui::ScrollView::EventType eType){
        if(eType == ui::ScrollView::EventType::CONTAINER_MOVED)
        {
            const float minY = _contentListView->getContentSize().height - _contentListView->getInnerContainerSize().height;
            float scrollDist = MAX(_contentListView->getInnerContainerPosition().y - minY, 0);
            
            _topScrollGradient->setContentSize(Size(_contentListView->getContentSize().width, MIN(scrollDist,160)));
            
        }
    });
    
    return true;
}

void ChatHQ::onEnter()
{
    // Create a friend object which represents the current user
    const std::string& childId = ChildManager::getInstance()->getParentOrChildId();
    const std::string& childName = ChildManager::getInstance()->getParentOrChildName();
    const std::string& childAvatar = ChildManager::getInstance()->getParentOrChildAvatarId();
    _currentUser = Chat::Friend::create(childId, childName, childAvatar);
    Azoomee::Chat::delegate = ChatDelegate::getInstance();
    Chat::ChatAPI::getInstance()->registerObserver(this);
    Chat::ChatAPI::getInstance()->requestFriendList();
    Chat::ChatAPI::getInstance()->scheduleFriendListPoll();
    Super::onEnter();
}

void ChatHQ::onExit()
{
    Chat::ChatAPI::getInstance()->unscheduleFriendListPoll();
    Chat::ChatAPI::getInstance()->removeObserver(this);
    Super::onExit();
}

void ChatHQ::onSizeChanged()
{
    Super::onSizeChanged();
    
    if(_isPortrait)
    {
        _contentListView->setSizePercent(Vec2(1.0f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
        
        _recentMessagesLayout->setSizeType(SizeType::ABSOLUTE);
        _recentMessagesLayout->setPositionType(PositionType::ABSOLUTE);
        _recentMessagesLayout->setContentSize(Size(_contentListView->getContentSize().width, HQConsts::ChatRecentMessagesHeight));
        if(_recentMessagesLayout->getParent() == _staticContentLayout)
        {
            _recentMessagesLayout->retain();
            _recentMessagesLayout->removeFromParent();
            _contentListView->insertCustomItem(_recentMessagesLayout, 0);
            _recentMessagesLayout->release();
        }
    }
    else
    {
        _contentListView->setSizePercent(Vec2(0.5f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.5f, 1.0f));
        
        _recentMessagesLayout->setSizeType(SizeType::PERCENT);
        _recentMessagesLayout->setSizePercent(Vec2(0.975f,0.95f));
        _recentMessagesLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _recentMessagesLayout->setPositionType(PositionType::PERCENT);
        _recentMessagesLayout->setPositionPercent(Vec2::ANCHOR_TOP_LEFT);
        if(_contentListView->getItem(0) == _recentMessagesLayout)
        {
            _recentMessagesLayout->retain();
            _contentListView->removeItem(0);
            _staticContentLayout->addChild(_recentMessagesLayout);
            _recentMessagesLayout->release();
        }
    }
    
    const float contentListViewWidth = _contentListView->getSizePercent().x * getContentSize().width;
    
    _recentMessagesLayout->setMessageBarHeight(_isPortrait ? RecentMessages::kMessageBarHeightPortrait : RecentMessages::kMessageBarHeightLandscape);
    
    _friendsListLayout->setContentSize(Size(contentListViewWidth, 0));
    _friendsListTitle->setFontSize(_isPortrait ? 89 : 75);
    _friendsListTitle->setTextAreaSize(Size(contentListViewWidth - kListViewSidePadding, _friendsListTitle->getContentSize().height));
    
    _topScrollGradient->setContentSize(Size(contentListViewWidth, _topScrollGradient->getContentSize().height));
    
    _contentListView->forceDoLayout();
}

void ChatHQ::createRecentMessages()
{
    _recentMessagesLayout = RecentMessages::create();
    _recentMessagesLayout->setSizeType(SizeType::PERCENT);
    _recentMessagesLayout->setSizePercent(Vec2(0.975f,0.95f));
    _recentMessagesLayout->setMessageBarHeight(_isPortrait ? RecentMessages::kMessageBarHeightPortrait : RecentMessages::kMessageBarHeightLandscape);
    _recentMessagesLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _recentMessagesLayout->setPositionType(PositionType::PERCENT);
    _recentMessagesLayout->setPositionPercent(Vec2::ANCHOR_TOP_LEFT);
    _staticContentLayout->addChild(_recentMessagesLayout);
}

void ChatHQ::createFriendsList()
{
    _friendsListTitle = DynamicText::create(_("Friends"), Style::Font::PoppinsBold(), 75);
    _friendsListTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _friendsListTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _friendsListTitle->setOverflow(Label::Overflow::SHRINK);
    _friendsListTitle->setTextColor(Color4B::WHITE);
    _friendsListTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentListView->pushBackCustomItem(_friendsListTitle);
    
    _friendsListLayout = FriendsList::create();
    _friendsListLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _friendsListLayout->setTileSize(kFriendTileSize);
    _friendsListLayout->setMinColumns(3);
    _friendsListLayout->setFriendSelectedCallback([this](Chat::FriendRef friendData) {
        openChatMessageSceneWithFriend(friendData);
    });
    _friendsListLayout->setAddFriendSelectedCallback([](){
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::SettingsFromHQ));
    });
    _contentListView->addChild(_friendsListLayout);
}

void ChatHQ::openChatMessageSceneWithFriend(const Chat::FriendRef& friendData)
{
    const bool isParent = friendData->friendId() == UserAccountManager::getInstance()->getLoggedInParentId();
    AnalyticsSingleton::getInstance()->setChatFriendIsParent(isParent);
    AnalyticsSingleton::getInstance()->genericButtonPressEvent(isParent ? "ChatScene - SelectedParent" : "ChatScene - SelectedFriend");
    AnalyticsSingleton::getInstance()->contentItemSelectedEvent("CHAT");
    
    Chat::FriendList participants = { _currentUser, friendData };
    auto messageScene = Chat::MessageScene::create(participants);
    Director::getInstance()->replaceScene(TransitionSlideInB::create(0.25f, messageScene));
}

// Chat API observer
/// Friend List success response
void ChatHQ::onChatAPIGetFriendList(const Chat::FriendList& friendList, int amountOfNewMessages)
{
    AnalyticsSingleton::getInstance()->setNumberOfChatFriends((int)friendList.size());
    
    float scrollPercent = _contentListView->getScrolledPercentVertical();
    if(isnan(scrollPercent))
    {
        scrollPercent = 0.1f;
    }
    _friendsListLayout->setFriendsList(friendList);
    _friendsListLayout->forceDoLayout();
    _contentListView->forceDoLayout();
    _contentListView->scrollToPercentVertical(scrollPercent, 0, false);
    
    Chat::ChatAPI::getInstance()->getTimelineSummary();
}

/// Get Timeline Summary response
void ChatHQ::onChatAPIGetTimelineSummary(const Chat::MessageList& messageList)
{
    std::vector<RecentMessage> messages;
    const Chat::FriendList& friends = Chat::ChatAPI::getInstance()->getFriendList();
    for(auto message : messageList)
    {
        auto friendIt = std::find_if(friends.begin(), friends.end(), [&](const Chat::FriendRef& friendObj){
            return friendObj->friendId() == message->senderId();
        });
        
        if(friendIt == friends.end())
        {
            continue;
        }
        
        messages.push_back({*friendIt, message});
    }
    
    if(messages.size() == 0)
    {
        Chat::MessageRef message = Chat::Message::createTextMessage(_("Hi, welcome to chat!"));
        Chat::FriendRef user = Chat::Friend::create(ChildManager::getInstance()->getParentOrChildId(), ChildManager::getInstance()->getParentOrChildName(), ChildManager::getInstance()->getParentOrChildAvatarId());
        messages.push_back({user, message});
        _recentMessagesLayout->setMessageSelectedCallback(nullptr);
    }
    else
    {
        _recentMessagesLayout->setMessageSelectedCallback([this](Chat::FriendRef friendData){
            openChatMessageSceneWithFriend(friendData);
        });
    }
    
    _recentMessagesLayout->setMessageData(messages);
}


NS_AZOOMEE_END
