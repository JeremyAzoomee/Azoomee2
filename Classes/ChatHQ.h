//
//  ChatHQ.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/10/2019.
//

#ifndef ChatHQ_h
#define ChatHQ_h

#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon//UI/DynamicText.h>
#include "HQPage.h"
#include "AzoomeeChat/ChatAPI.h"
#include "FriendsList.h"
#include "RecentMessages.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ChatHQ : public HQPage, Chat::ChatAPIObserver
{
    typedef HQPage Super;
private:
    static const cocos2d::Size kFriendTileSize;
    
    cocos2d::LayerGradient* _topScrollGradient = nullptr;
    
    RecentMessages* _recentMessagesLayout = nullptr;
    DynamicText* _friendsListTitle = nullptr;
    FriendsList* _friendsListLayout = nullptr;
    
    Chat::FriendRef _currentUser = nullptr;
    
    void createRecentMessages();
    void createFriendsList();
    void openChatMessageSceneWithFriend(const Chat::FriendRef& friendData);
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(ChatHQ);
    
    // Chat API observer
    /// Friend List success response
    virtual void onChatAPIGetFriendList(const Chat::FriendList& friendList, int amountOfNewMessages) override;
    /// Get Timeline Summary response
    virtual void onChatAPIGetTimelineSummary(const Chat::MessageList& messageList) override;
};

NS_AZ_END

#endif /* ChatHQ_h */
