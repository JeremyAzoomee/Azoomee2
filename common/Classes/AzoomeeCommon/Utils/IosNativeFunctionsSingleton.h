#ifndef __IOSNATIVEFUNCTIONS_SINGLETON_IOS_H__
#define __IOSNATIVEFUNCTIONS_SINGLETON_IOS_H__

#include "cocos2d.h"
#include "../Azoomee.h"

NS_AZOOMEE_BEGIN

class IosNativeFunctionsSingleton

{
    
public:
    static IosNativeFunctionsSingleton* getInstance();
    virtual ~IosNativeFunctionsSingleton();
    
    const char* getIosSystemVersion();
    const char* getIosDeviceType();
    const char* getIosDeviceIDFA();
	const char* getIosDeviceLanguage();
    
    void identifyMixpanel();
    
    void deleteHttpCookies();
    
    bool doBiometricValidation(bool precheck);
    
    void startIosReviewProcess();
    
private:
    IosNativeFunctionsSingleton();
};

NS_AZOOMEE_END

#endif
