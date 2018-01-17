#include "IosNativeFunctionsSingleton.h"
#import <AdSupport/ASIdentifierManager.h>
#import <LocalAuthentication/LocalAuthentication.h>
#import <UIKit/UIKit.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<IosNativeFunctionsSingleton> _sharedIosNativeFunctionsSingleton;
const char* const IosNativeFunctionsSingleton::kBiometricValidationSuccess = "biometricValidationSuccess";
const char* const IosNativeFunctionsSingleton::kBiometricValidationFailure = "biometricValidationFailure";
const char* const IosNativeFunctionsSingleton::kBiometricValidation = "biometricValidation";

IosNativeFunctionsSingleton* IosNativeFunctionsSingleton::getInstance()
{
    if (! _sharedIosNativeFunctionsSingleton.get())
    {
        _sharedIosNativeFunctionsSingleton.reset(new IosNativeFunctionsSingleton);
    }
    
    return _sharedIosNativeFunctionsSingleton.get();
}

IosNativeFunctionsSingleton::~IosNativeFunctionsSingleton()
{
}

IosNativeFunctionsSingleton::IosNativeFunctionsSingleton()
{
}

const char* IosNativeFunctionsSingleton::getIosSystemVersion()
{
    return [[NSString stringWithFormat:@"%@%@", [[UIDevice currentDevice] systemName], [[UIDevice currentDevice] systemVersion]] cStringUsingEncoding:NSUTF8StringEncoding];
}

const char* IosNativeFunctionsSingleton::getIosDeviceType()
{
    return [[[UIDevice currentDevice] model] cStringUsingEncoding:NSUTF8StringEncoding];
}

const char* IosNativeFunctionsSingleton::getIosDeviceIDFA()
{
    return [[[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString] cStringUsingEncoding:NSUTF8StringEncoding];
}

void IosNativeFunctionsSingleton::deleteHttpCookies()
{
    NSHTTPCookieStorage *cookieStorage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    for (NSHTTPCookie *each in cookieStorage.cookies)
    {
        [cookieStorage deleteCookie:each];
    }
}

bool IosNativeFunctionsSingleton::doBiometricValidation(bool precheck)
{
    auto userDefault = UserDefault::getInstance();
    
    LAContext *myContext = [[LAContext alloc] init];
    NSError *authError = nil;
    NSString *myLocalizedReasonString = @"Please use Face ID to enter your pin.";
    if ([myContext canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:&authError]) {
        
        if(precheck)
        {
            return true;
        }
        
        [myContext evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics
                  localizedReason:myLocalizedReasonString
                            reply:^(BOOL success, NSError *error) {
                                if (success) {
                                    cocos2d::log("BIOMETRIC success");
                                    userDefault->setIntegerForKey(kBiometricValidation, 1);
                                    EventCustom event(kBiometricValidationSuccess);
                                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                                } else {
                                    long errorCode = [error code];
                                    
                                    if([error code] == -6)
                                    {
                                        userDefault->setIntegerForKey(kBiometricValidation, -1);
                                    }
                                    else if([error code] != -2)
                                    {
                                        EventCustom event(kBiometricValidationFailure);
                                        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
                                    }
                                    cocos2d::log("BIOMETRIC error, code: %ld", errorCode);
                                }
                            }];
    } else {
        cocos2d::log("BIOMETRIC could not start");
        if(precheck)
        {
            return false;
        }
    }
    
    return true;
}

NS_AZOOMEE_END
