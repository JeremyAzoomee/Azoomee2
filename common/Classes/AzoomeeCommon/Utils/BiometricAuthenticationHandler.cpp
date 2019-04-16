#include "BiometricAuthenticationHandler.h"
#include "../Data/ConfigStorage.h"
#include "../Strings.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IosNativeFunctionsSingleton.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";
#endif

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const char* const BiometricAuthenticationHandler::kBiometricValidationSuccess = "biometricValidationSuccess";
const char* const BiometricAuthenticationHandler::kBiometricValidationFailure = "biometricValidationFailure";
const char* const BiometricAuthenticationHandler::kBiometricValidation = "biometricValidation";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const char* const kFingerPrintAuthenticationAvailableJavaMethodName = "fingerPrintAuthenticationAvailable";
const char* const kBiometricStartJavaMethodName = "startFingerprintAuthentication";
const char* const kBiometricStopJavaMethodName = "stopFingerprintAuthentication";

const char* const kBiometricDialogImage = "res/settings/fingerprint.png";
#endif

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
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IosNativeFunctionsSingleton::getInstance()->doBiometricValidation(true);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return JniHelper::callStaticBooleanMethod(kAzoomeeActivityJavaClassName, kFingerPrintAuthenticationAvailableJavaMethodName);
#endif
    return false;
}

bool BiometricAuthenticationHandler::biometricAuthenticationSet() const
{
    return UserDefault::getInstance()->getIntegerForKey(kBiometricValidation, 1) == 1;
}

void BiometricAuthenticationHandler::startBiometricAuthentication()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IosNativeFunctionsSingleton::getInstance()->doBiometricValidation(false);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _waitingForFingerPrint = MessageBox::createWith(_("Waiting for authentication"), kBiometricDialogImage, _("Please authenticate yourself to access parents area"), _("Cancel"), this);
    JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, kBiometricStartJavaMethodName);
#endif
}

void BiometricAuthenticationHandler::biometricAuthenticationSuccess()
{
    removeMessageBoxAndroid();

    UserDefault::getInstance()->setIntegerForKey(kBiometricValidation, 1);
    EventCustom event(kBiometricValidationSuccess);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void BiometricAuthenticationHandler::biometricAuthenticationFailure()
{
    removeMessageBoxAndroid();
    
    EventCustom event(kBiometricValidationFailure);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void BiometricAuthenticationHandler::biometricAuthenticationError()
{
	removeMessageBoxAndroid();
}

void BiometricAuthenticationHandler::biometricAuthenticationNotNeeded()
{
    UserDefault::getInstance()->setIntegerForKey(kBiometricValidation, -1);
}

void BiometricAuthenticationHandler::removeMessageBoxAndroid()
{
	if(_waitingForFingerPrint)
	{
		_waitingForFingerPrint->removeMessageBox();
		_waitingForFingerPrint = nullptr;
	}
}

//messagebox delegate

void BiometricAuthenticationHandler::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(buttonTitle == _("Cancel"))
    {
        JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, kBiometricStopJavaMethodName);
    }
#endif
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

NS_AZOOMEE_END
