#include "CrashlyticsConfig.h"
#include <cocos/cocos2d.h>
#include <cocos/platform/android/jni/JniHelper.h>

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

namespace Azoomee
{
  
void createCrashlyticsException(const std::string& exceptionDomain, int exceptionCode, const std::string& exceptionMessage)
{
#ifdef CRASHLYTICS_ENABLED
    const std::string& messageBody = cocos2d::StringUtils::format("Domain:%s; Code:%d; Message:%s", exceptionDomain.c_str(), exceptionCode, exceptionMessage.c_str());
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "CrashlyticsLogException", messageBody);
#endif
}

void createCrashlyticsUserInfo(const std::string& adultIdentifier, const std::string& childIdentifier)
{
#ifdef CRASHLYTICS_ENABLED
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "CrashlyticsLogUser", adultIdentifier, childIdentifier);
#endif
}
    
void setCrashlyticsKeyWithString(const std::string& key, const std::string& dataString)
{
#ifdef CRASHLYTICS_ENABLED
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "CrashlyticsKeyWithString", key, dataString);
#endif
}

}
