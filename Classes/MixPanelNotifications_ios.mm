#include "MixPanelNotifications_ios.h"
#import "Mixpanel.h"
#import <UIKit/UIKit.h>

void showNotification()
{
    [[Mixpanel sharedInstance] showNotification];
}

void showNotificationWithID(int notificationID)
{
    [[Mixpanel sharedInstance] showNotificationWithID:notificationID];
}
