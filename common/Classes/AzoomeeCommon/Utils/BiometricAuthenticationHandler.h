#ifndef __BIOMETRIC_AUTHENTICATION_HANDLER_H__
#define __BIOMETRIC_AUTHENTICATION_HANDLER_H__

#include "cocos2d.h"
#include "../Azoomee.h"
#include "../UI/MessageBox.h"

NS_AZOOMEE_BEGIN

class BiometricAuthenticationHandler : public MessageBoxDelegate

{
    
public:
    static const char* const kBiometricValidationSuccess;
    static const char* const kBiometricValidation;
    static const char* const kBiometricValidationFailure;
    
    static BiometricAuthenticationHandler* getInstance();
    virtual ~BiometricAuthenticationHandler();
    
    bool biometricAuthenticationAvailable() const;
    bool biometricAuthenticationSet() const;
    void startBiometricAuthentication();
    void biometricAuthenticationSuccess();
    void biometricAuthenticationFailure();
    void biometricAuthenticationError();
    void biometricAuthenticationNotNeeded();
	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void removeMessageBoxAndroid();
#endif
    //messagebox delegate
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
private:
    BiometricAuthenticationHandler();
    MessageBox* _waitingForFingerPrint = nullptr;
};

NS_AZOOMEE_END

#endif
