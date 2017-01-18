#include "Crashlytics_ios.h"
#import <Fabric/Fabric.h>
#import <Crashlytics/Crashlytics.h>

void createCrashlyticsExecption_ios(std::string execptionDomain, int execptionCode, std::string execptionMessage)
{
    NSError *error = [NSError errorWithDomain:[NSString stringWithUTF8String:execptionDomain.c_str()] code:execptionCode userInfo:@{[NSString stringWithUTF8String:execptionMessage.c_str()]: @""}];
    [CrashlyticsKit recordError:error];
}

void createCrashlyticsUserInfo_ios(std::string AdultIdentifier, std::string ChildIdentifier)
{
    [CrashlyticsKit setUserIdentifier:[NSString stringWithUTF8String:AdultIdentifier.c_str()]];
    [CrashlyticsKit setUserName:[NSString stringWithUTF8String:ChildIdentifier.c_str()]];
}
