//
//  Device_ios.mm
//  TinizineCommon
//
//  Created by Macauley.Scoffins on 17/02/2020.
//

#include "Device.h"
#include "Net/Utils.h"
#include "Analytics/AnalyticsSingleton.h"
#include "Utils/StringFunctions.h"
#import <AdSupport/ASIdentifierManager.h>
#import <LocalAuthentication/LocalAuthentication.h>
#import <UIKit/UIKit.h>
#import "StoreKit/StoreKit.h"
#import <Mixpanel/Mixpanel.h>
#import "Utils/BiometricAuthenticationHandler.h"
#import "Utils/LocaleManager.h"
#import "Platform/iOS/AzoomeeViewController.h"

NS_TZ_BEGIN

std::string Device::getDeviceInformation()
{
    std::string sourceDeviceString1 = "NA";
    std::string sourceDeviceString2 = "NA";
    
    sourceDeviceString1 = [[NSString stringWithFormat:@"%@%@", [[UIDevice currentDevice] systemName], [[UIDevice currentDevice] systemVersion]] cStringUsingEncoding:NSUTF8StringEncoding];
    sourceDeviceString2 = [[[UIDevice currentDevice] model] cStringUsingEncoding:NSUTF8StringEncoding];
    
    std::string sourceDevice = Net::urlEncode(sourceDeviceString1) + "|" + Net::urlEncode(sourceDeviceString2);
    
    return sourceDevice;
}

std::string Device::getDeviceAdvertisingId()
{
    std::string deviceId = "";
    
    deviceId = [[[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString] cStringUsingEncoding:NSUTF8StringEncoding];

    return deviceId;
}
    
std::string Device::getOSManufacturer()
{
    if(_osManufacturer != "")
    {
        return _osManufacturer;
    }
    
    AnalyticsSingleton::getInstance()->registerIAPOS("iOS");
    _osManufacturer = "Apple";
    
    return _osManufacturer;
}
    
std::string Device::getDeviceLanguage()
{
    std::string languageCode = "";

    languageCode = [[[NSLocale preferredLanguages] firstObject] cStringUsingEncoding:NSUTF8StringEncoding];
    
    return languageCode;
}

std::string Device::getAppVersion() const
{
    NSString * appVersionString = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
    NSString * appBuildString = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
    NSString * versionBuildString = [NSString stringWithFormat:@"%@ (%@)", appVersionString, appBuildString];
    return [versionBuildString cStringUsingEncoding:NSUTF8StringEncoding];
}

void Device::identifyMixpanel()
{
    NSString *idfa = [NSString stringWithCString:getDeviceAdvertisingId().c_str() encoding:NSUTF8StringEncoding];
    
    if(![idfa isEqualToString:@""])
    {
        [[Mixpanel sharedInstance] identify:idfa];
    }
}

void Device::deleteHttpCookies()
{
    NSHTTPCookieStorage *cookieStorage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
       for (NSHTTPCookie *each in cookieStorage.cookies)
       {
           [cookieStorage deleteCookie:each];
       }
}

bool Device::biometricAuthenticationAvailable()
{
    LAContext *myContext = [[LAContext alloc] init];
    NSError *authError = nil;
    if ([myContext canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:&authError]) {
        return true;
    }
    else
    {
        cocos2d::log("BIOMETRIC could not start");
        return false;
    }
}

void Device::startBiometricAuthentication()
{
    LAContext *myContext = [[LAContext alloc] init];
    NSError *authError = nil;
    NSString *myLocalizedReasonString = [NSString stringWithUTF8String: _("Please use Touch ID or Face ID to enter your PIN.").c_str()];
    if ([myContext canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:&authError]) {
        [myContext evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics
                  localizedReason:myLocalizedReasonString
                            reply:^(BOOL success, NSError *error) {
                                if (success) {
                                    BiometricAuthenticationHandler::getInstance()->biometricAuthenticationSuccess();
                                } else {
                                    if([error code] == -2)
                                    {
                                        BiometricAuthenticationHandler::getInstance()->biometricAuthenticationNotNeeded(); //cancel pressed on faceid
                                    }
                                    else
                                    {
                                        BiometricAuthenticationHandler::getInstance()->biometricAuthenticationFailure();
                                    }
                                }
                            }];
    }
    else
    {
        cocos2d::log("BIOMETRIC could not start");
    }
}

void Device::stopBiometricAuthentication()
{
    //Do Nothing, is handled by OS
}

void Device::setOrientation(Application::Orientation orientation)
{
    AzoomeeViewController* rootViewController = [AzoomeeViewController sharedInstance];
    switch(orientation)
    {
        case Application::Orientation::Portrait:
            [rootViewController setOrientationToPortrait];
            break;
        case Application::Orientation::Landscape:
            [rootViewController setOrientationToLandscape];
            break;
        case Application::Orientation::Any:
            [rootViewController setOrientationToAny];
            break;
    }
}

NS_TZ_END
