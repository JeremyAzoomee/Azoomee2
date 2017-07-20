#ifndef __VIDEOPLAYLISTMANAGER_H__
#define __VIDEOPLAYLISTMANAGER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeChat/ChatAPI.h>
#include <AzoomeeChat/Data/Message.h>

NS_AZOOMEE_BEGIN

class ChatNotificationsSingleton : public cocos2d::Ref, private HttpRequestCreatorResponseDelegate, public Chat::ChatAPIObserver
{
    
public:
    static ChatNotificationsSingleton* getInstance(void);
    virtual ~ChatNotificationsSingleton();
    bool init(void);
    
    void getNotificationsForLoggedInUser();
    bool userHasNotifications();
    
    void setNavigationLayer(cocos2d::Layer* navLayer);
    cocos2d::Layer* getNavigationLayer();
    
private:
    std::map<std::string, bool> notificationsForUsers;
    int lastUpdateTimeStamp = 0;
    void notifyNavigationLayer();
    void removeBadgeFromNavigationLayer();
    cocos2d::Layer* navigationLayer;
    bool loggedInUserHasNotifications = false;
    void scheduleUpdateOfPollingUnreadMessages();
    
    // - HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
    // - Chat API event observer
    void onChatAPIMessageRecieved(const Chat::MessageRef& message) override;
};

NS_AZOOMEE_END

#endif
