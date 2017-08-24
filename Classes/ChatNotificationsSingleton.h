#ifndef __CHATNOTIFICATIONSSINGLETON_H__
#define __CHATNOTIFICATIONSSINGLETON_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeChat/ChatAPI.h>
#include <AzoomeeChat/Data/Message.h>

NS_AZOOMEE_BEGIN

class ChatNotificationsSingleton : public cocos2d::Ref, public Chat::ChatAPIObserver
{
    
public:
    static ChatNotificationsSingleton* getInstance(void);
    virtual ~ChatNotificationsSingleton();
    bool init(void);
    bool userHasNotifications();
    
    void setNavigationLayer(cocos2d::Layer* navLayer);
    void forceNotificationsUpdate();
    cocos2d::Layer* getNavigationLayer();
    
    void stopNotificationsUpdate();
    
private:
    void notifyNavigationLayer();
    void removeBadgeFromNavigationLayer();
    cocos2d::Layer* navigationLayer;
    bool loggedInUserHasNotifications = false;
    
    // - Chat API event observer
    void onChatAPIMessageRecieved(const Chat::MessageRef& message) override;
    void onChatAPINewMessageNotificationReceived(int sumOfUnreadMessages) override;
};

NS_AZOOMEE_END

#endif
