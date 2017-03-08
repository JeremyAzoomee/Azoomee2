#include "PaymentSingleton.h"
#include "HttpRequestCreator.h"
#include "external/json/document.h"
#include "ModalMessages.h"
#include "MessageBox.h"
#include "LoginScene.h"

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
                
                Director::getInstance()->replaceScene(LoginScene::createScene(3001));
            }
            else
            {
                //handle status is not fulfilled
                return;
            }
        }
        else
        {
            //handle receiptStatus value unexpected
            return;
        }
    }
    else
    {
        //Handle string not having member of receiptStatus
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
