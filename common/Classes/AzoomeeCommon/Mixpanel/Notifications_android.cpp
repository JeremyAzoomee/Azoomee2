#include "Notifications.h"


namespace Azoomee
{

void showMixPanelNotification()
{
    cocos2d::JniMethodInfo methodInfo;
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "showMixpanelNotification", "()V"))
    {
        CCLOG("showNotification_android failed");
        return;
    }
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void showMixPanelNotificationWithID(int notificationID)
{
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "showMixpanelNotificationWithID", "(I)V"))
    {
        CCLOG("showNotificationWithID_android failed");
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,notificationID);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

}
