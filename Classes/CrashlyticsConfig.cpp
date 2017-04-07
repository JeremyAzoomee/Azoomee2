#include "CrashlyticsConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "Crashlytics_ios.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "Crashlytics_android.h"
#endif

void createCrashlyticsExecption(std::string execptionDomain, int execptionCode, std::string execptionMessage)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    createCrashlyticsExecption_ios(execptionDomain, execptionCode, execptionMessage);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    createCrashlyticsExecption_android(execptionDomain, execptionCode, execptionMessage);
    
#endif
}

void createCrashlyticsUserInfo(std::string AdultIdentifier, std::string ChildIdentifier)
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    createCrashlyticsUserInfo_ios(AdultIdentifier, ChildIdentifier);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    createCrashlyticsUserInfo_android(AdultIdentifier, ChildIdentifier);
    
#endif
}
