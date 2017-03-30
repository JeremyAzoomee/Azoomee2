#include "PaymentSingleton_ios.h"
#include "MessageBox.h"
#include "payment_ios.h"
#include "ModalMessages.h"
#include "external/json/document.h"
#include "AnalyticsSingleton.h"
#include "BackEndCaller.h"

USING_NS_CC;

using namespace cocos2d;

static PaymentSingleton_ios *_sharedPaymentSingleton_ios = NULL;

PaymentSingleton_ios* PaymentSingleton_ios::getInstance()
{
    if (! _sharedPaymentSingleton_ios)
    {
        _sharedPaymentSingleton_ios = new PaymentSingleton_ios();
        _sharedPaymentSingleton_ios->init();
    }
    
    return _sharedPaymentSingleton_ios;
}

PaymentSingleton_ios::~PaymentSingleton_ios(void)
{
}

bool PaymentSingleton_ios::init(void)
{
    return true;
}

void PaymentSingleton_ios::makeMonthlyPayment()
{
    requestAttempts = 0;
    ModalMessages::getInstance()->startLoading();
    
    payment_ios* applePaymentObject = [[payment_ios alloc] init];
    [applePaymentObject retain];
    [applePaymentObject makeOneMonthPayment];
}

void PaymentSingleton_ios::restorePayment()
{
    payment_ios* applePaymentObject = [[payment_ios alloc] init];
    [applePaymentObject retain];
    [applePaymentObject restorePayment];
}

void PaymentSingleton_ios::transactionStatePurchased(std::string receiptData)
{
    CCLOG("IAP Request made");
    
    savedReceipt = receiptData;
    
   /* HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestBody = StringUtils::format("{\"requestId\": \"%s\", \"receiptId\": \"%s\", \"amazonUserId\": \"%s\"}", requestId.c_str(), receiptId.c_str(), amazonUserid.c_str());
    httpRequestCreator->requestTag = "iapAmazonPaymentMade";
    httpRequestCreator->createEncryptedPostHttpRequest();*/
}

void PaymentSingleton_ios::onAnswerReceived(std::string responseDataString)
{
    CCLOG("The response id is: %s", responseDataString.c_str());
    
    rapidjson::Document paymentData;
    paymentData.Parse(responseDataString.c_str());
    
    if(paymentData.HasParseError())
    {
        requestAttempts = requestAttempts + 1;
        transactionStatePurchased(savedReceipt);
        return;
    }
    
    bool paymentFailed = true;
    
    if(paymentData.HasMember("receiptStatus"))
    {
        if(paymentData["receiptStatus"].IsString())
        {
            // EXPIRED, INVALID, UNCERTAIN
            if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED")
            {
                AnalyticsSingleton::getInstance()->iapSubscriptionSuccessEvent();
                paymentFailed = false;
                
                //std::string receiptId = paymentData["receiptId"].GetString();
                //fulfillAmazonPayment(receiptId);
                
                ModalMessages::getInstance()->stopLoading();
                
                BackEndCaller::getInstance()->newTrialJustStarted = true;
                BackEndCaller::getInstance()->autoLogin();
            }
            else
                AnalyticsSingleton::getInstance()->iapSubscriptionErrorEvent(StringUtils::format("%s", paymentData["receiptStatus"].GetString()));
        }
    }
    
    if(paymentFailed)
    {
        if(requestAttempts < 4)
        {
            requestAttempts = requestAttempts + 1;
            transactionStatePurchased(savedReceipt);
        }
        else
        {
            ModalMessages::getInstance()->stopLoading();
            MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
            return;
        }
    }
}


void PaymentSingleton_ios::ErrorMessage()
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
}

void PaymentSingleton_ios::DoublePurchase()
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_PURCHASE_DOUBLE, nullptr);
}
