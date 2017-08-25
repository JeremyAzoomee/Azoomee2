#include "AmazonPaymentSingleton.h"
#include "external/json/document.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include "BackEndCaller.h"
#include "LoginLogicHandler.h"
#include "RoutePaymentSingleton.h"

#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

#include <jni.h>

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

#endif

using namespace cocos2d;
using namespace Azoomee;

NS_AZOOMEE_BEGIN

static AmazonPaymentSingleton *_sharedAmazonPaymentSingleton = NULL;

AmazonPaymentSingleton* AmazonPaymentSingleton::getInstance()
{
    if (! _sharedAmazonPaymentSingleton)
    {
        _sharedAmazonPaymentSingleton = new AmazonPaymentSingleton();
        _sharedAmazonPaymentSingleton->init();
    }
    
    return _sharedAmazonPaymentSingleton;
}

AmazonPaymentSingleton::~AmazonPaymentSingleton(void)
{
}

bool AmazonPaymentSingleton::init(void)
{
    return true;
}

//--------------------PAYMENT FUNCTIONS------------------

void AmazonPaymentSingleton::startIAPPayment()
{
    requestAttempts = 0;
        
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
        JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "startAmazonPurchase");
        
    #endif
}

void AmazonPaymentSingleton::amazonPaymentMade(std::string requestId, std::string receiptId, std::string amazonUserid)
{
    CCLOG("IAP Request made with requestid:%s receiptID:%s userID:%s", requestId.c_str(),receiptId.c_str(),amazonUserid.c_str());
    
    savedRequestId = requestId;
    savedReceiptId = receiptId;
    savedAmazonUserid = amazonUserid;
    
    BackEndCaller::getInstance()->verifyAmazonPayment(requestId, receiptId, amazonUserid);
}

void AmazonPaymentSingleton::onAmazonPaymentMadeAnswerReceived(std::string responseDataString)
{
    CCLOG("The response id is: %s", responseDataString.c_str());
    
    rapidjson::Document paymentData;
    paymentData.Parse(responseDataString.c_str());
    
    if(!paymentData.HasParseError() && paymentData.HasMember("receiptStatus"))
    {
        if(std::string(paymentData["receiptStatus"].GetString()) == "FULFILLED")
        {
            std::string receiptId = paymentData["receiptId"].GetString();
            fulfillAmazonPayment(receiptId);
            
            RoutePaymentSingleton::getInstance()->inAppPaymentSuccess();
            return;
        }
        else
            AnalyticsSingleton::getInstance()->iapSubscriptionErrorEvent(StringUtils::format("%s", paymentData["receiptStatus"].GetString()));
    }

    if(requestAttempts < 4)
    {
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage("AnswerRecieved-RequestAttempts<4");
        requestAttempts = requestAttempts + 1;
        amazonPaymentMade(savedRequestId, savedReceiptId, savedAmazonUserid);
        return;
    }
    else
    {
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage("AnswerRecieved-RequestAttempts>4");
        return;
    }
}

void AmazonPaymentSingleton::fulfillAmazonPayment(std::string receiptId)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "fulfillAmazonPurchase", receiptId);
    
#endif

}

//-------------------- JAVA RETURN FUNCTIONS-----------------

void showDoublePurchase()
{
    auto funcCallAction = CallFunc::create([=](){
        RoutePaymentSingleton::getInstance()->doublePurchaseMessage();
    });
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from amazon window, otherwise the app will crash
}

void purchaseFailureErrorMessageWithDelay()
{
    auto funcCallAction = CallFunc::create([=](){
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage("PurchaseFailed-From Native");
    });
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from amazon window, otherwise the app will crash
}

NS_AZOOMEE_END


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseHappened(JNIEnv* env, jobject thiz, jstring requestId, jstring receiptId, jstring amazonUserid);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseHappened(JNIEnv* env, jobject thiz, jstring requestId, jstring receiptId, jstring amazonUserid)
{
    const char* cRequestId = env->GetStringUTFChars(requestId, NULL);
    const char* cReceiptId = env->GetStringUTFChars(receiptId, NULL);
    const char* cAmazonUserid = env->GetStringUTFChars(amazonUserid, NULL);
    
    CCLOG("COCOS2DX: I have the data: requestid: %s, receiptid: %s, amazonuserid: %s", cRequestId, cReceiptId, cAmazonUserid);
    
    AmazonPaymentSingleton::getInstance()->amazonPaymentMade(cRequestId, cReceiptId, cAmazonUserid);
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_alreadyPurchased(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_alreadyPurchased(JNIEnv* env, jobject thiz)
{
    CCLOG("COCOS2DX: alreadyPurchased CALLED!!!!!");
    showDoublePurchase();
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseFailed(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseFailed(JNIEnv* env, jobject thiz)
{
    CCLOG("COCOS2DX: PURCHASE FAILED");
    purchaseFailureErrorMessageWithDelay();
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_userDataFailed(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_userDataFailed(JNIEnv* env, jobject thiz)
{
    CCLOG("COCOS2DX: USER DATA FAILED");
    AnalyticsSingleton::getInstance()->iapUserDataFailedEvent();
}

extern "C"

{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getAmazonSku(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getAmazonSku(JNIEnv* env, jobject thiz)
{
    return env->NewStringUTF(ConfigStorage::getInstance()->getIapSkuForProvider("amazon").c_str());
}

#endif
