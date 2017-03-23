#include "MixPanelNotifications_android.h"

void showNotification_android()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "showMixpanelNotification", "()V"))
    {
        CCLOG("showNotification_android failed");
        return;
    }
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

void showNotificationWithID_android(int notificationID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "showMixpanelNotificationWithID", "(I)V"))
    {
        CCLOG("showNotificationWithID_android failed");
        return;
    }
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,notificationID);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}
