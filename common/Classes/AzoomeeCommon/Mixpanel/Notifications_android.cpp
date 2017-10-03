#include "Notifications.h"
#include <cocos/cocos2d.h>
#include <cocos/platform/android/jni/JniHelper.h>

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

namespace Azoomee
{

void showMixPanelNotification()
{
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "showMixpanelNotification");

}

void showMixPanelNotificationWithID(int notificationID)
{
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "showMixpanelNotificationWithID", notificationID);
}

}
