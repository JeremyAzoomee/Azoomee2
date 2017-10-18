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
    
    void deleteHttpCookies();
    
    void addNamedUserIdentifierToPushChannel(const std::string &userid);
    
private:
    IosNativeFunctionsSingleton();
};

NS_AZOOMEE_END

#endif
