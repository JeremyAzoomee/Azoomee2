#include "CrashlyticsConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "Crashlytics_ios.h"
#endif

void createCrashlyticsExecption(std::string execptionMessage)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "CrashlyticsLogException", "(Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringMessage = methodInfo.env->NewStringUTF(execptionMessage.c_str());
    
    CCLOG("To be sent to jni for Crashlytics: %s", execptionMessage.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstringMessage);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

void createCrashlyticsUserInfo(std::string identifier, std::string email, std::string userName)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "CrashlyticsLogUser", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jidentifier = methodInfo.env->NewStringUTF(identifier.c_str());
    jstring jemail = methodInfo.env->NewStringUTF(email.c_str());
    jstring juserName = methodInfo.env->NewStringUTF(userName.c_str());

    CCLOG("To be sent to jni for Crashlytics: indentifier:%s email:%s userName:%s", identifier.c_str(), email.c_str(), userName.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jidentifier, jemail, juserName);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}
