#include "MixPanelNotifications_android.h"

void showNotification_android()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", JNIFunction.c_str(), "(Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jstringJSONProperties);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

void showNotificationWithID_android(int notificationID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", JNIFunction.c_str(), "(Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jstringJSONProperties);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}
