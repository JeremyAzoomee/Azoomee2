#include "MixPanelNotifications_ios.h"
#import <Mixpanel/Mixpanel.h>
#import <UIKit/UIKit.h>

void showNotification_ios()
{
    [[Mixpanel sharedInstance] showNotification];
}

void showNotificationWithID_ios(int notificationID)
{
    [[Mixpanel sharedInstance] showNotificationWithID:notificationID];
}
