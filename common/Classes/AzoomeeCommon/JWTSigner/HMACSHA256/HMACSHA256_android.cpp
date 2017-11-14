#include "HMACSHA256.h"
#include <cocos/cocos2d.h>
#include <cocos/platform/android/jni/JniHelper.h>

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

using namespace cocos2d;


namespace Azoomee
{

std::string HMACSHA256::getHMACSHA256Hash(const std::string& message, const std::string& secret)
{
    std::string result = "done";
    
    std::string hmacResponse = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getHMACSHA256", message, secret);
    
    return hmacResponse;
}
    
}
