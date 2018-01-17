#ifndef __IOSNATIVEFUNCTIONS_SINGLETON_IOS_H__
#define __IOSNATIVEFUNCTIONS_SINGLETON_IOS_H__

#include "cocos2d.h"
#include "../Azoomee.h"

NS_AZOOMEE_BEGIN

class IosNativeFunctionsSingleton

{
    
public:
    static const char* const kBiometricValidationSuccess;
    static const char* const kBiometricValidation;
    static const char* const kBiometricValidationFailure;
    
    static IosNativeFunctionsSingleton* getInstance();
    virtual ~IosNativeFunctionsSingleton();
    
    const char* getIosSystemVersion();
    const char* getIosDeviceType();
    const char* getIosDeviceIDFA();
    
    void deleteHttpCookies();
    
    bool doBiometricValidation(bool precheck);
    
private:
    IosNativeFunctionsSingleton();
};

NS_AZOOMEE_END

#endif
