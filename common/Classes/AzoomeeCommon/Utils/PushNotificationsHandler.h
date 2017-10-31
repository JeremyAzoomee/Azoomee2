#ifndef PushNotificationsHandler_h
#define PushNotificationsHandler_h

#include "cocos2d.h"
#include "../Azoomee.h"

NS_AZOOMEE_BEGIN
    
    class PushNotificationsHandler : public cocos2d::Ref
    {
    private:
        PushNotificationsHandler(); //call getInstance only
        
    public:
        static PushNotificationsHandler* getInstance(void);
        virtual ~PushNotificationsHandler();
        
        void setNamedUserIdentifierForPushChannel(const std::string &channelName);
        void setTagForPushChannel(const std::string &tagGroup, const std::string &tag);
        void enablePushNotifications();
        void disablePushNotifications();
        void resetExistingNotifications();
        
    };

NS_AZOOMEE_END

#endif
