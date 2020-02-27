#include "BiometricAuthenticationHandler.h"
#include "../Utils/LocaleManager.h"
#include "../Device.h"

using namespace cocos2d;

NS_TZ_BEGIN

const char* const BiometricAuthenticationHandler::kBiometricValidationSuccess = "biometricValidationSuccess";
const char* const BiometricAuthenticationHandler::kBiometricValidationFailure = "biometricValidationFailure";
const char* const BiometricAuthenticationHandler::kBiometricValidation = "biometricValidation";
const char* const BiometricAuthenticationHandler::kBiometricValidationError = "biometricValidationError";

static std::auto_ptr<BiometricAuthenticationHandler> _sharedBiometricAuthenticationHandler;

BiometricAuthenticationHandler* BiometricAuthenticationHandler::getInstance()
{
    if (! _sharedBiometricAuthenticationHandler.get())
    {
        _sharedBiometricAuthenticationHandler.reset(new BiometricAuthenticationHandler);
    }
    
    return _sharedBiometricAuthenticationHandler.get();
}

BiometricAuthenticationHandler::~BiometricAuthenticationHandler()
{
}

BiometricAuthenticationHandler::BiometricAuthenticationHandler()
{
}

bool BiometricAuthenticationHandler::biometricAuthenticationAvailable() const
{
    return Device::getInstance()->biometricAuthenticationAvailable();
}

bool BiometricAuthenticationHandler::biometricAuthenticationSet() const
{
    return UserDefault::getInstance()->getIntegerForKey(kBiometricValidation, 1) == 1;
}

void BiometricAuthenticationHandler::startBiometricAuthentication()
{
    Device::getInstance()->startBiometricAuthentication();
}

void BiometricAuthenticationHandler::biometricAuthenticationSuccess()
{
    UserDefault::getInstance()->setIntegerForKey(kBiometricValidation, 1);
    EventCustom event(kBiometricValidationSuccess);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void BiometricAuthenticationHandler::biometricAuthenticationFailure()
{
    EventCustom event(kBiometricValidationFailure);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void BiometricAuthenticationHandler::biometricAuthenticationError()
{
    EventCustom event(kBiometricValidationError);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void BiometricAuthenticationHandler::biometricAuthenticationNotNeeded()
{
    UserDefault::getInstance()->setIntegerForKey(kBiometricValidation, -1);
}

void BiometricAuthenticationHandler::stopBiometricAuthentication()
{
    Device::getInstance()->stopBiometricAuthentication();
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Biometric_biometricAuthenticationSuccess(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Biometric_biometricAuthenticationSuccess(JNIEnv* env, jobject thiz)
{
    BiometricAuthenticationHandler::getInstance()->biometricAuthenticationSuccess();
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Biometric_biometricAuthenticationFailure(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Biometric_biometricAuthenticationFailure(JNIEnv* env, jobject thiz)
{
    BiometricAuthenticationHandler::getInstance()->biometricAuthenticationFailure();
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Biometric_biometricAuthenticationError(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Biometric_biometricAuthenticationError(JNIEnv* env, jobject thiz)
{
    BiometricAuthenticationHandler::getInstance()->biometricAuthenticationError();
}
#endif

NS_TZ_END
