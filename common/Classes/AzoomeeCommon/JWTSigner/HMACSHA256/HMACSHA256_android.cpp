#include "HMACSHA256.h"
#include <cocos/cocos2d.h>
#include <cocos/platform/android/jni/JniHelper.h>

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

using namespace cocos2d;


namespace Azoomee
{

/*std::string HMACSHA256::getHMACSHA256Hash(const std::string& message, const std::string& secret)
{
    std::string result = "done";
    
    std::string hmacResponse = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getHMACSHA256", message, secret);
    
    return hmacResponse;
}*/
  
    std::string HMACSHA256::getHMACSHA256Hash(const std::string& message, const std::string& secret)
    {
        std::string result = "done";
        
        cocos2d::JniMethodInfo methodInfo;
        
        if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getHMACSHA256", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"))
        {
            return false;
        }
        
        cocos2d::log("HMACSHA messge size: %i", message.size());
        
        jstring jniMessage = methodInfo.env->NewStringUTF(message.c_str());
        jstring jniSecret = methodInfo.env->NewStringUTF(secret.c_str());
        
        cocos2d::log("jni HMACSHA messge size: %i", JniHelper::jstring2string(jniMessage).size());
        
        jstring hmacResponse = (jstring) methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, jniMessage, jniSecret);
        
        std::string cppResponse = JniHelper::jstring2string(hmacResponse);
        
        methodInfo.env->DeleteLocalRef(jniMessage);
        methodInfo.env->DeleteLocalRef(jniSecret);
        
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
        return cppResponse;
    }
    
}
