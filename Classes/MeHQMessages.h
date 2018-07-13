//
//  MeHQMessages.h
//  azoomee2
//
//  Created by Macauley on 14/05/2018.
//

#ifndef MeHQMessages_h
#define MeHQMessages_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeChat/ChatAPI.h>

NS_AZOOMEE_BEGIN

class MeHQMessages : public cocos2d::ui::Layout, public Chat::ChatAPIObserver
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void()> RefreshLayoutCallback;
private:
    cocos2d::ui::Layout* _contentLayer = nullptr;
    Chat::FriendList _friendList;
    Chat::MessageList _messages;
    
    RefreshLayoutCallback _refreshCallback = nullptr;
    
    void createMessageList();
    
    void buildEmptyCarousel();
    
    
    
protected:
    virtual void onSizeChanged() override;
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(MeHQMessages);
    
    void setRefreshCallback(const RefreshLayoutCallback& callback);
    
    /// Get Timeline Summary response
    virtual void onChatAPIGetTimelineSummary(const Chat::MessageList& messageList) override;
    /// API error from Chat request
    virtual void onChatAPIErrorRecieved(const std::string& requestTag, long errorCode) override;
};

NS_AZOOMEE_END

#endif /* MeHQMessages_h */
