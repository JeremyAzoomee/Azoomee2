#include "Notifications.h"
#import <Mixpanel/Mixpanel.h>


namespace Azoomee
{

void showMixPanelNotification()
{
    [[Mixpanel sharedInstance] showNotification];
}

void showMixPanelNotificationWithID(int notificationID)
{
    [[Mixpanel sharedInstance] showNotificationWithID:notificationID];
}
  
}
