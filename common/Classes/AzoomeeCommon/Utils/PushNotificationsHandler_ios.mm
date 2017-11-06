#include "PushNotificationsHandler.h"
#import <AirshipKit/AirshipKit.h>

NS_AZOOMEE_BEGIN
    
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
    
    PushNotificationsHandler::PushNotificationsHandler()
    {
        
    }
    
    void PushNotificationsHandler::setNamedUserIdentifierForPushChannel(const std::string &channelName)
    {
        [UAirship namedUser].identifier = [NSString stringWithFormat:@"%s", channelName.c_str()];
        [[UAirship push] setChannelTagRegistrationEnabled:FALSE];
    }
    
    void PushNotificationsHandler::setTagForPushChannel(const std::string &tagGroup, const std::string &tag)
    {
        [[UAirship namedUser] setTags:@[[NSString stringWithFormat:@"%s", tag.c_str()]] group:[NSString stringWithFormat:@"%s", tagGroup.c_str()]];
        [[UAirship namedUser] updateTags];
    }
    
    void PushNotificationsHandler::enablePushNotifications()
    {
        [UAirship push].userPushNotificationsEnabled = YES;
        [UAirship push].defaultPresentationOptions = (NULL); //this setting is for FOREGROUND presentation only
    }
    
    void PushNotificationsHandler::disablePushNotifications()
    {
        [UAirship push].userPushNotificationsEnabled = NO;
    }
    
    void PushNotificationsHandler::resetExistingNotifications()
    {
        [[UAirship push] setBadgeNumber:0];
        [[UAirship push] resetBadge];
    }

NS_AZOOMEE_END
