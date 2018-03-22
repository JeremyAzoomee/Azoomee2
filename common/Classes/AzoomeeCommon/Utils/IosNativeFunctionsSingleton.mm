#include "IosNativeFunctionsSingleton.h"
#include "../Data/ConfigStorage.h"
#include "BiometricAuthenticationHandler.h"
#import <AdSupport/ASIdentifierManager.h>
#import <LocalAuthentication/LocalAuthentication.h>
#import <UIKit/UIKit.h>
#import "StoreKit/StoreKit.h"
#import <Mixpanel/Mixpanel.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<IosNativeFunctionsSingleton> _sharedIosNativeFunctionsSingleton;

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

void IosNativeFunctionsSingleton::identifyMixpanel()
{
    NSString *idfa = [NSString stringWithCString:getIosDeviceIDFA() encoding:NSUTF8StringEncoding];
    
    if(![idfa isEqualToString:@""])
    {
        [[Mixpanel sharedInstance] identify:idfa];
    }
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
    LAContext *myContext = [[LAContext alloc] init];
    NSError *authError = nil;
    NSString *myLocalizedReasonString = @"Please use Touch ID or Face ID to enter your PIN.";
    if ([myContext canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:&authError]) {
        
        if(precheck)
        {
            return true;
        }
        
        [myContext evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics
                  localizedReason:myLocalizedReasonString
                            reply:^(BOOL success, NSError *error) {
                                if (success) {
                                    BiometricAuthenticationHandler::getInstance()->biometricAuthenticationSuccess();
                                } else {
                                    if([error code] == -6)
                                    {
                                        BiometricAuthenticationHandler::getInstance()->biometricAuthenticationNotNeeded(); //cancel pressed on faceid
                                    }
                                    else if([error code] != -2)
                                    {
                                        BiometricAuthenticationHandler::getInstance()->biometricAuthenticationFailure();
                                    }
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

void IosNativeFunctionsSingleton::startIosReviewProcess()
{
    [SKStoreReviewController requestReview];
}

NS_AZOOMEE_END
