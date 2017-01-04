#include "HMACSHA256_javaCaller.h"


using namespace cocos2d;

static HMACSHA256_javaCaller *_sharedHMACSHA256_javaCaller = NULL;

HMACSHA256_javaCaller* HMACSHA256_javaCaller::getInstance()
{
    if (! _sharedHMACSHA256_javaCaller)
    {
        _sharedHMACSHA256_javaCaller = new HMACSHA256_javaCaller();
        _sharedHMACSHA256_javaCaller->init();
    }
    
    return _sharedHMACSHA256_javaCaller;
}

HMACSHA256_javaCaller::~HMACSHA256_javaCaller(void)
{
}

bool HMACSHA256_javaCaller::init(void)
{
    return true;
}

std::string HMACSHA256_javaCaller::getHMACSHA256Hash(std::string message, std::string secret)
{
    std::string result = "done";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
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
    
#endif
    
    return "invalid call, not allowed from any other than android";

}
