#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "PushNotificationsHandler.h"
#import <AirshipKit/AirshipKit.h>

namespace Azoomee{
    
    static std::auto_ptr<PushNotificationsHandler> sPushNotificationsHandlerSharedInstance;
    
    PushNotificationsHandler* PushNotificationsHandler::getInstance()
    {
        if(!sPushNotificationsHandlerSharedInstance.get())
        {
            sPushNotificationsHandlerSharedInstance.reset(new PushNotificationsHandler());
        }
        return sPushNotificationsHandlerSharedInstance.get();
    }
    
    PushNotificationsHandler::~PushNotificationsHandler(void)
    {
        
    }
    
    void PushNotificationsHandler::setNamedUserIdentifierForPushChannel(const std::string &channelName)
    {
        [UAirship namedUser].identifier = [NSString stringWithFormat:@"%s", channelName.c_str()];
    }
    
    void PushNotificationsHandler::setTagForPushChannel(const std::string &tag)
    {
        
    }
    
    void PushNotificationsHandler::enablePushNotifications()
    {
        [UAirship push].userPushNotificationsEnabled = YES;
        [UAirship push].defaultPresentationOptions = (UNNotificationPresentationOptionAlert |
                                                      UNNotificationPresentationOptionBadge |
                                                      UNNotificationPresentationOptionSound);
    }
    
    void PushNotificationsHandler::disablePushNotifications()
    {
        [UAirship push].userPushNotificationsEnabled = NO;
    }
    
}

#endif
