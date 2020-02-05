#ifndef __BIOMETRIC_AUTHENTICATION_HANDLER_H__
#define __BIOMETRIC_AUTHENTICATION_HANDLER_H__

#include "cocos2d.h"
#include "../Azoomee.h"

NS_AZOOMEE_BEGIN

class BiometricAuthenticationHandler
{
public:
    static const char* const kBiometricValidationSuccess;
    static const char* const kBiometricValidation;
    static const char* const kBiometricValidationFailure;
    static const char* const kBiometricValidationError;
    
    static BiometricAuthenticationHandler* getInstance();
    virtual ~BiometricAuthenticationHandler();
    
    bool biometricAuthenticationAvailable() const;
    bool biometricAuthenticationSet() const;
    void startBiometricAuthentication();
    void biometricAuthenticationSuccess();
    void biometricAuthenticationFailure();
    void biometricAuthenticationError();
    void biometricAuthenticationNotNeeded();
    void stopBiometricAuthentication();
	
    
private:
    BiometricAuthenticationHandler();
};

NS_AZOOMEE_END

#endif
