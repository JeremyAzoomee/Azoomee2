#include "Crashlytics_ios.h"
#import <Fabric/Fabric.h>
#import <Crashlytics/Crashlytics.h>

void createCrashlyticsExecption_ios(std::string execptionDomain, int execptionCode, std::string execptionMessage)
{
    NSError *error = [NSError errorWithDomain:[NSString stringWithUTF8String:execptionDomain.c_str()] code:execptionCode userInfo:@{[NSString stringWithUTF8String:execptionMessage.c_str()]: @""}];
    [CrashlyticsKit recordError:error];
}

void createCrashlyticsUserInfo_ios(std::string identifier, std::string email, std::string userName)
{
    [CrashlyticsKit setUserIdentifier:[NSString stringWithUTF8String:identifier.c_str()]];
    [CrashlyticsKit setUserEmail:[NSString stringWithUTF8String:email.c_str()]];
    [CrashlyticsKit setUserName:[NSString stringWithUTF8String:userName.c_str()]];
}
