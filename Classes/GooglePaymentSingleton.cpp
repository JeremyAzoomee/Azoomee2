#include "GooglePaymentSingleton.h"
#include "external/json/document.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "LoginLogicHandler.h"
#include "RoutePaymentSingleton.h"
#include "DynamicNodeHandler.h"
#include "FlowDataSingleton.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

#endif

using namespace cocos2d;
using namespace Azoomee;

NS_AZOOMEE_BEGIN

static GooglePaymentSingleton *_sharedGooglePaymentSingleton = NULL;

GooglePaymentSingleton* GooglePaymentSingleton::getInstance()
{
    if (! _sharedGooglePaymentSingleton)
    {
        _sharedGooglePaymentSingleton = new GooglePaymentSingleton();
        _sharedGooglePaymentSingleton->init();
    }
    
    return _sharedGooglePaymentSingleton;
}

GooglePaymentSingleton::~GooglePaymentSingleton(void)
{
}

bool GooglePaymentSingleton::init(void)
{
    return true;
}

void GooglePaymentSingleton::startBackEndPaymentVerification(std::string developerPayload, std::string orderId, std::string token)
{
    savedDeveloperPayload = developerPayload;
    savedOrderId = orderId;
    savedToken = token;
    if(!ParentDataProvider::getInstance()->isUserLoggedIn())
    {
        auto funcCallAction = CallFunc::create([=](){
            ModalMessages::getInstance()->stopLoading();
            FlowDataSingleton::getInstance()->setSuccessFailPath(IAP_SUCCESS);
            DynamicNodeHandler::getInstance()->handleSuccessFailEvent();
        });
        
        Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from google window, otherwise the app will crash
    }
    else
    {
        auto funcCallAction = CallFunc::create([=](){
            BackEndCaller::getInstance()->verifyGooglePayment(orderId, ConfigStorage::getInstance()->getIapSkuForProvider("google"), token);
        });
    
        Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from google window, otherwise the app will crash
    }
}

void GooglePaymentSingleton::onGooglePaymentVerificationAnswerReceived(std::string responseDataString)
{
    rapidjson::Document paymentData;
    paymentData.Parse(responseDataString.c_str());
    
    if(paymentData.HasParseError())
    {
        requestAttempts = requestAttempts + 1;
        startBackEndPaymentVerification(savedDeveloperPayload, savedOrderId, savedToken);
        return;
    }
    
    if(!paymentData.HasParseError() && paymentData.HasMember("receiptStatus"))
    {
        if(std::string(paymentData["receiptStatus"].GetString()) == "FULFILLED")
        {
            RoutePaymentSingleton::getInstance()->inAppPaymentSuccess();
            return;
        }
        else
        {
            AnalyticsSingleton::getInstance()->iapSubscriptionErrorEvent(StringUtils::format("%s", paymentData["receiptStatus"].GetString()));
        }
    }
    
    if(requestAttempts < 4)
    {
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage("AnswerRecieved-RequestAttempts<4");
        requestAttempts = requestAttempts + 1;
        startBackEndPaymentVerification(savedDeveloperPayload, savedOrderId, savedToken);
        return;
    }
    else
    {
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage("AnswerRecieved-RequestAttempts>4");
        return;
    }
}

void GooglePaymentSingleton::purchaseFailedBeforeFulfillment()
{
    auto funcCallAction = CallFunc::create([=](){
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage("PurchaseFailed-From Native");
    });
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from google window, otherwise the app will crash
}

void GooglePaymentSingleton::purchaseFailedAlreadyPurchased()
{
    auto funcCallAction = CallFunc::create([=](){
        RoutePaymentSingleton::getInstance()->doublePurchaseMessage();
    });
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from google window, otherwise the app will crash
}

void GooglePaymentSingleton::restoreFailedNoPurchase()
{
	auto funcCallAction = CallFunc::create([=](){
		RoutePaymentSingleton::getInstance()->failedRestoreMessage();
	});
	
	Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from google window, otherwise the app will crash
}

//--------------------PAYMENT FUNCTIONS------------------

void GooglePaymentSingleton::startIABPayment()
{
    requestAttempts = 0;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "startGooglePurchase");
    
#endif
}

void GooglePaymentSingleton::startRestorePurchase()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	
	JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "startGoogleRestorePurchase");
	
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseHappened(JNIEnv* env, jobject thiz, jstring developerPayload, jstring orderId, jstring token);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseHappened(JNIEnv* env, jobject thiz, jstring developerPayload, jstring orderId, jstring token)
{
    const char* cDeveloperPayload = env->GetStringUTFChars(developerPayload, NULL);
    const char* cOrderId = env->GetStringUTFChars(orderId, NULL);
    const char* cToken = env->GetStringUTFChars(token, NULL);
    
    RoutePaymentSingleton::getInstance()->writeAndroidReceiptDataToFile(std::string(cDeveloperPayload), std::string(cOrderId), std::string(cToken));
    AnalyticsSingleton::getInstance()->iapSubscriptionSuccessEvent();
    GooglePaymentSingleton::getInstance()->startBackEndPaymentVerification(std::string(cDeveloperPayload), std::string(cOrderId), std::string(cToken));
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseFailed(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseFailed(JNIEnv* env, jobject thiz)
{
    GooglePaymentSingleton::getInstance()->purchaseFailedBeforeFulfillment();
}

extern "C"

{
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googleNoPurchaseFound(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googleNoPurchaseFound(JNIEnv* env, jobject thiz)
{
	GooglePaymentSingleton::getInstance()->restoreFailedNoPurchase();
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseFailedAlreadyPurchased(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseFailedAlreadyPurchased(JNIEnv* env, jobject thiz)
{
    GooglePaymentSingleton::getInstance()->purchaseFailedAlreadyPurchased();
}

extern "C"

{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getGoogleSku(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getGoogleSku(JNIEnv* env, jobject thiz)
{
    return env->NewStringUTF(ConfigStorage::getInstance()->getIapSkuForProvider("google").c_str());
}

extern "C"

{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getLoggedInParentUserId(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getLoggedInParentUserId(JNIEnv* env, jobject thiz)
{
    return env->NewStringUTF(ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
}

extern "C"

{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getDeveloperKey(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getDeveloperKey(JNIEnv* env, jobject thiz)
{
    return env->NewStringUTF(ConfigStorage::getInstance()->getDeveloperPublicKey().c_str());
}

#endif

NS_AZOOMEE_END
