#include "CrashlyticsConfig.h"
#import <Fabric/Fabric.h>
#import <Crashlytics/Crashlytics.h>


namespace Azoomee
{
  
void createCrashlyticsException(const std::string& exceptionDomain, int exceptionCode, const std::string& exceptionMessage)
{
    NSError *error = [NSError errorWithDomain:[NSString stringWithUTF8String:exceptionDomain.c_str()] code:exceptionCode userInfo:@{[NSString stringWithUTF8String:exceptionMessage.c_str()]: @""}];
    [CrashlyticsKit recordError:error];
}

void createCrashlyticsUserInfo(const std::string& adultIdentifier, const std::string& childIdentifier)
{
    [CrashlyticsKit setUserIdentifier:[NSString stringWithUTF8String:adultIdentifier.c_str()]];
    [CrashlyticsKit setUserName:[NSString stringWithUTF8String:childIdentifier.c_str()]];
}
  
}
