#ifndef DirectorySearcher_h
#define DirectorySearcher_h

#include "cocos2d.h"

namespace Azoomee
{
    
    class PushNotificationsHandler : public cocos2d::Ref
    {
    private:
        
    public:
        static PushNotificationsHandler* getInstance(void);
        virtual ~PushNotificationsHandler();
        
        void setNamedUserIdentifierForPushChannel(const std::string &channelName);
        void setTagForPushChannel(const std::string &tag);
        void enablePushNotifications();
        void disablePushNotifications();
        
    };
}

#endif
