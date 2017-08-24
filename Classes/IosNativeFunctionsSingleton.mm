#include "IosNativeFunctionsSingleton.h"

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

NS_AZOOMEE_END
