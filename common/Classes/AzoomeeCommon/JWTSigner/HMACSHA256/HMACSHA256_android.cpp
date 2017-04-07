#include "HMACSHA256.h"
#include <cocos/cocos2d.h>

using namespace cocos2d;


namespace Azoomee
{

std::string HMACSHA256::getHMACSHA256Hash(const std::string& message, const std::string& secret)
{
    std::string result = "done";
  
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getHMACSHA256", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"))
    {
        return false;
    }
    
    
    jstring jniMessage = methodInfo.env->NewStringUTF(message.c_str());
    jstring jniSecret = methodInfo.env->NewStringUTF(secret.c_str());
    
    jstring hmacResponse = (jstring) methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, jniMessage, jniSecret);
    
    std::string cppResponse = StringUtils::format("%s", (char*)JniHelper::jstring2string(hmacResponse).c_str());
    
    methodInfo.env->DeleteLocalRef(jniMessage);
    methodInfo.env->DeleteLocalRef(jniSecret);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
    return cppResponse;
}
  
}
