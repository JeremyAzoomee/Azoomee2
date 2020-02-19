#ifndef __CHATNOTIFICATIONSSINGLETON_H__
#define __CHATNOTIFICATIONSSINGLETON_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Tinizine.h>
#include <AzoomeeCommon/API/API.h>
#include "AzoomeeChat/ChatAPI.h"
#include "AzoomeeChat/Data/Message.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ChatNotificationsSingleton : public cocos2d::Ref, public Chat::ChatAPIObserver
{
    
public:
    static ChatNotificationsSingleton* getInstance();
    virtual ~ChatNotificationsSingleton();
    bool init();
    bool userHasNotifications();
    
    void forceNotificationsUpdate();
    
    void stopNotificationsUpdate();
    
private:
    bool loggedInUserHasNotifications = false;
    
    // - Chat API event observer
    void onChatAPIGetFriendList(const Chat::FriendList& friendList, int amountOfNewMessages) override;
};

NS_AZ_END

#endif
