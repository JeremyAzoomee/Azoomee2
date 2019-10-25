//
//  ChatHQ.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/10/2019.
//

#ifndef ChatHQ_h
#define ChatHQ_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon//UI/DynamicText.h>
#include "HQPage.h"
#include "AzoomeeChat/ChatAPI.h"
#include "FriendsList.h"
#include "RecentMessages.h"

NS_AZOOMEE_BEGIN

class ChatHQ : public HQPage, Chat::ChatAPIObserver
{
    typedef HQPage Super;
private:
    cocos2d::LayerGradient* _topScrollGradient = nullptr;
    
    RecentMessages* _recentMessagesLayout = nullptr;
    DynamicText* _friendsListTitle = nullptr;
    FriendsList* _friendsListLayout = nullptr;
    
    Chat::FriendRef _currentUser = nullptr;
    
    void createRecentMessages();
    void createFriendsList();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(ChatHQ);
    
    // Chat API observer
    /// Friend List success response
    virtual void onChatAPIGetFriendList(const Chat::FriendList& friendList, int amountOfNewMessages) override;
    /// Get message list success response
    virtual void onChatAPIGetChatMessages(const Chat::MessageList& messageList) override;
    /// Send message success response
    virtual void onChatAPISendMessage(const Chat::MessageRef& sentMessage) override;
    /// Get Timeline Summary response
    virtual void onChatAPIGetTimelineSummary(const Chat::MessageList& messageList) override;
    /// Conversation moderation status changed
    virtual void onChatAPIModerationStatusChanged(const Chat::FriendRef& friendObj) override;
    /// API error from Chat request
    virtual void onChatAPIErrorRecieved(const std::string& requestTag, long errorCode) override;
    /// Refresh child session response
    virtual void onChatAPIRefreshChildSession() override;
};

NS_AZOOMEE_END

#endif /* ChatHQ_h */
