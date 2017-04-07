#include "Crashlytics_android.h"

void createCrashlyticsExecption_android(std::string execptionDomain, int execptionCode, std::string execptionMessage)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string messageBody = cocos2d::StringUtils::format("Domain:%s; Code:%d; Message:%s", execptionDomain.c_str(), execptionCode, execptionMessage.c_str());
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "CrashlyticsLogException", "(Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringMessage = methodInfo.env->NewStringUTF(messageBody.c_str());
    
    CCLOG("To be sent to jni for Crashlytics: %s", messageBody.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstringMessage);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void createCrashlyticsUserInfo_android(std::string AdultIdentifier, std::string ChildIdentifier)
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "CrashlyticsLogUser", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jAdultIdentifier = methodInfo.env->NewStringUTF(AdultIdentifier.c_str());
    jstring jChildIdentifier = methodInfo.env->NewStringUTF(ChildIdentifier.c_str());
    
    CCLOG("To be sent to jni for Crashlytics: AdultIdentifier:%s, ChildIdentifier:%s", AdultIdentifier.c_str(), ChildIdentifier.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jAdultIdentifier, jChildIdentifier);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}
