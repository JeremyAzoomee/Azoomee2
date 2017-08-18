#ifndef __IOSNATIVEFUNCTIONS_SINGLETON_IOS_H__
#define __IOSNATIVEFUNCTIONS_SINGLETON_IOS_H__

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class IosNativeFunctionsSingleton

{
    
public:
    static IosNativeFunctionsSingleton* getInstance();
    virtual ~IosNativeFunctionsSingleton();
    
    const char* getIosDeviceData();
    const char* getIosDeviceIDFA();
    
private:
    IosNativeFunctionsSingleton();
};

NS_AZOOMEE_END

#endif
