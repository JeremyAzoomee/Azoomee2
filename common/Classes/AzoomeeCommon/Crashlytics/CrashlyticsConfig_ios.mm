#include "CrashlyticsConfig.h"

#ifdef CRASHLYTICS_ENABLED
#import <Fabric/Fabric.h>
#import <Crashlytics/Crashlytics.h>
#endif


namespace Azoomee
{
  
void createCrashlyticsException(const std::string& exceptionDomain, int exceptionCode, const std::string& exceptionMessage)
{
#ifdef CRASHLYTICS_ENABLED
    NSError *error = [NSError errorWithDomain:[NSString stringWithUTF8String:exceptionDomain.c_str()] code:exceptionCode userInfo:@{[NSString stringWithUTF8String:exceptionMessage.c_str()]: @""}];
    [CrashlyticsKit recordError:error];
#endif
}

void createCrashlyticsUserInfo(const std::string& adultIdentifier, const std::string& childIdentifier)
{
#ifdef CRASHLYTICS_ENABLED
    [CrashlyticsKit setUserIdentifier:[NSString stringWithUTF8String:adultIdentifier.c_str()]];
    [CrashlyticsKit setUserName:[NSString stringWithUTF8String:childIdentifier.c_str()]];
#endif
}
    
void setCrashlyticsKeyWithString(const std::string& key, const std::string& dataString)
{
#ifdef CRASHLYTICS_ENABLED
    [CrashlyticsKit setObjectValue:[NSString stringWithUTF8String:dataString.c_str()] forKey:[NSString stringWithUTF8String:key.c_str()]];
#endif
}
  
}
