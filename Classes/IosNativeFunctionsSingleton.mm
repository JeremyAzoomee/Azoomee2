#include "IosNativeFunctionsSingleton.h"
#import <AdSupport/ASIdentifierManager.h>

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

const char* IosNativeFunctionsSingleton::getIosDeviceData()
{
    NSString *returnString = [NSString stringWithFormat:@"%@%@|%@", [[UIDevice currentDevice] systemName], [[UIDevice currentDevice] systemVersion], [[UIDevice currentDevice] model]];
    return [returnString cStringUsingEncoding:NSUTF8StringEncoding];
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

NS_AZOOMEE_END
