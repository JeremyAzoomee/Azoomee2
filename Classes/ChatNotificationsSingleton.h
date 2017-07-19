#ifndef __VIDEOPLAYLISTMANAGER_H__
#define __VIDEOPLAYLISTMANAGER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class ChatNotificationsSingleton : public cocos2d::Ref
{
    
public:
    static ChatNotificationsSingleton* getInstance(void);
    virtual ~ChatNotificationsSingleton();
    bool init(void);
    
    void getNotificationsForUser(std::string userid);
    void getNotificationsForAllUsers();
    bool userHasNotifications(std::string userid);
    
private:
    std::map<std::string, bool> notificationsForUsers;
    int lastUpdateTimeStamp = 0;
};

NS_AZOOMEE_END

#endif
