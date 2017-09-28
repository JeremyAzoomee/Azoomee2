#include "CrashlyticsConfig.h"
#include <cocos/cocos2d.h>
#include <cocos/platform/android/jni/JniHelper.h>

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

namespace Azoomee
{
  
void createCrashlyticsException(const std::string& exceptionDomain, int exceptionCode, const std::string& exceptionMessage)
{
    std::string messageBody = cocos2d::StringUtils::format("Domain:%s; Code:%d; Message:%s", exceptionDomain.c_str(), exceptionCode, exceptionMessage.c_str());
    
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "CrashlyticsLogException", messageBody);
}

void createCrashlyticsUserInfo(const std::string& adultIdentifier, const std::string& childIdentifier)
{
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "CrashlyticsLogUser", adultIdentifier, childIdentifier);
}
    
void setCrashlyticsKeyWithString(const std::string& key, const std::string& dataString)
{
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "CrashlyticsKeyWithString", key, dataString);
}

}
