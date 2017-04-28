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
#endif

USING_NS_CC;
using namespace cocos2d;
using namespace Azoomee;


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
        
        cocos2d::JniMethodInfo methodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "startAmazonPurchase", "()V"))
        {
            return;
        }
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
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
    
    if(paymentData.HasParseError())
    {
        requestAttempts = requestAttempts + 1;
        amazonPaymentMade(savedRequestId, savedReceiptId, savedAmazonUserid);
        return;
    }
    
    if(paymentData.HasMember("receiptStatus"))
    {
        if(paymentData["receiptStatus"].IsString())
        {
            if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED")
            {
                std::string receiptId = paymentData["receiptId"].GetString();
                fulfillAmazonPayment(receiptId);
                
                RoutePaymentSingleton::getInstance()->inAppPaymentSuccess();
                return;
            }
            else
                AnalyticsSingleton::getInstance()->iapSubscriptionErrorEvent(StringUtils::format("%s", paymentData["receiptStatus"].GetString()));
        }
    }

    if(requestAttempts < 4)
    {
        requestAttempts = requestAttempts + 1;
        amazonPaymentMade(savedRequestId, savedReceiptId, savedAmazonUserid);
    }
    else
    {
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage();
        return;
    }
}

void AmazonPaymentSingleton::fulfillAmazonPayment(std::string receiptId)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "fulfillAmazonPurchase", "(Ljava/lang/String;)V"))
    {
        return;
    }
    jstring jReceiptId = methodInfo.env->NewStringUTF(receiptId.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jReceiptId);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
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
    RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage();
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
