#include "CrashlyticsConfig.h"
#include <cocos/cocos2d.h>


namespace Azoomee
{
  
void createCrashlyticsException(const std::string& exceptionDomain, int exceptionCode, const std::string& exceptionMessage)
{
    std::string messageBody = cocos2d::StringUtils::format("Domain:%s; Code:%d; Message:%s", exceptionDomain.c_str(), exceptionCode, exceptionMessage.c_str());
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "CrashlyticsLogException", "(Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringMessage = methodInfo.env->NewStringUTF(messageBody.c_str());
    
    CCLOG("To be sent to jni for Crashlytics: %s", messageBody.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstringMessage);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void createCrashlyticsUserInfo(const std::string& adultIdentifier, const std::string& childIdentifier)
{
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "CrashlyticsLogUser", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jAdultIdentifier = methodInfo.env->NewStringUTF(adultIdentifier.c_str());
    jstring jChildIdentifier = methodInfo.env->NewStringUTF(childIdentifier.c_str());
    
    CCLOG("To be sent to jni for Crashlytics: AdultIdentifier:%s, ChildIdentifier:%s", adultIdentifier.c_str(), childIdentifier.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jAdultIdentifier, jChildIdentifier);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
    
void setCrashlyticsKEYwithSTRING(const std::string& key, const std::string& dataString)
{
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "CrashlyticsKEYwithSTRING", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jKey = methodInfo.env->NewStringUTF(key.c_str());
    jstring jDataString = methodInfo.env->NewStringUTF(dataString.c_str());
    
    CCLOG("To be sent to jni for Crashlytics: Key:%s, dataString:%s", key.c_str(), dataString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jKey, jDataString);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

}
