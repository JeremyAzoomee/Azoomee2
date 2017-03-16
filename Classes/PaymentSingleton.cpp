#include "PaymentSingleton.h"
#include "HttpRequestCreator.h"
#include "external/json/document.h"
#include "ModalMessages.h"
#include "MessageBox.h"
#include "LoginScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

using namespace cocos2d;

static PaymentSingleton *_sharedPaymentSingleton = NULL;

PaymentSingleton* PaymentSingleton::getInstance()
{
    if (! _sharedPaymentSingleton)
    {
        _sharedPaymentSingleton = new PaymentSingleton();
        _sharedPaymentSingleton->init();
    }
    
    return _sharedPaymentSingleton;
}

PaymentSingleton::~PaymentSingleton(void)
{
}

bool PaymentSingleton::init(void)
{
    return true;
}

void PaymentSingleton::startAmazonPayment()
{
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

void PaymentSingleton::amazonPaymentMade(std::string requestId, std::string receiptId, std::string amazonUserid)
{
    ModalMessages::getInstance()->startLoading();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestBody = StringUtils::format("{\"requestId\": \"%s\", \"receiptId\": \"%s\", \"amazonUserId\": \"%s\"}", requestId.c_str(), receiptId.c_str(), amazonUserid.c_str());
    httpRequestCreator->requestTag = "iapAmazonPaymentMade";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

void PaymentSingleton::onAmazonPaymentMadeAnswerReceived(std::string responseDataString)
{
    ModalMessages::getInstance()->stopLoading();
    
    CCLOG("The response id is: %s", responseDataString.c_str());
    
    rapidjson::Document paymentData;
    paymentData.Parse(responseDataString.c_str());
    
    if(paymentData.HasParseError())
    {
        //Handle parse error
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
                
                Director::getInstance()->replaceScene(LoginScene::createScene(ERROR_CODE_AMAZON_PURCHASE_SUCCESSFUL));
            }
            else
            {
                //handle status is not fulfilled
                MessageBox::createWith(ERROR_CODE_AMAZON_PURCHASE_FAILURE, nullptr);
                return;
            }
        }
        else
        {
            //handle receiptStatus value unexpected
            MessageBox::createWith(ERROR_CODE_AMAZON_PURCHASE_FAILURE, nullptr);
            return;
        }
    }
    else
    {
        //Handle string not having member of receiptStatus
        MessageBox::createWith(ERROR_CODE_AMAZON_PURCHASE_FAILURE, nullptr);
        return;
    }
    
}

void PaymentSingleton::fulfillAmazonPayment(std::string receiptId)
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

void showDoublePurchase()
{
    auto funcCallAction = CallFunc::create([=](){
        MessageBox::createWith(ERROR_CODE_AMAZON_PURCHASE_DOUBLE, nullptr);
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
    
    PaymentSingleton::getInstance()->amazonPaymentMade(cRequestId, cReceiptId, cAmazonUserid);
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

#endif
