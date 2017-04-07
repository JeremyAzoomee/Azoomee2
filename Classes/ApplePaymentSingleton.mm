#include "ApplePaymentSingleton.h"
#include "payment_ios.h"
#include "ModalMessages.h"
#include "external/json/document.h"
#include "AnalyticsSingleton.h"
#include "BackEndCaller.h"
#include "HttpRequestCreator.h"
#include "ParentDataProvider.h"

USING_NS_CC;

using namespace cocos2d;

static ApplePaymentSingleton *_sharedApplePaymentSingleton = NULL;

ApplePaymentSingleton* ApplePaymentSingleton::getInstance()
{
    if (! _sharedApplePaymentSingleton)
    {
        _sharedApplePaymentSingleton = new ApplePaymentSingleton();
        _sharedApplePaymentSingleton->init();
    }
    
    return _sharedApplePaymentSingleton;
}

ApplePaymentSingleton::~ApplePaymentSingleton(void)
{
}

bool ApplePaymentSingleton::init(void)
{
    return true;
}

void ApplePaymentSingleton::makeMonthlyPayment()
{
    requestAttempts = 0;
    ModalMessages::getInstance()->startLoading();
    
    makingMonthlyPayment = true;
    [[payment_ios sharedPayment_ios] makeOneMonthPayment];
}

void ApplePaymentSingleton::refreshReceipt(bool usingButton)
{
    makingMonthlyPayment = false;
    
    refreshFromButton = usingButton;
    requestAttempts = 0;
    ModalMessages::getInstance()->startLoading();
    [[payment_ios sharedPayment_ios] restorePayment];
}

void ApplePaymentSingleton::transactionStatePurchased(std::string receiptData)
{
    CCLOG("IAP Request made");
    
    savedReceipt = receiptData;
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestBody = StringUtils::format("{\"receipt-data\": \"%s\"}", receiptData.c_str());
    httpRequestCreator->requestTag = "iapApplePaymentMade";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

void ApplePaymentSingleton::onAnswerReceived(std::string responseDataString)
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
            if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED" && makingMonthlyPayment)
            {
                AnalyticsSingleton::getInstance()->iapSubscriptionSuccessEvent();
                paymentFailed = false;

                BackEndCaller::getInstance()->newTrialJustStarted = true;
                BackEndCaller::getInstance()->autoLogin();
            }
            else if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED")
            {
                AnalyticsSingleton::getInstance()->iapAppleAutoRenewSubscriptionEvent();
                ModalMessages::getInstance()->stopLoading();
                paymentFailed = false;
                BackEndCaller::getInstance()->autoLogin();
                return;
            }
            else
            {
                AnalyticsSingleton::getInstance()->iapSubscriptionErrorEvent(StringUtils::format("%s", paymentData["receiptStatus"].GetString()));
                
                ModalMessages::getInstance()->stopLoading();
                MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
            }
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
            if(ParentDataProvider::getInstance()->isPaidUser())
            {
                ModalMessages::getInstance()->stopLoading();
                //Error message shown, before logging in again to get new entitlements
                MessageBox::createWith(ERROR_CODE_APPLE_ACCOUNT_DOWNGRADED, this);
            }
            else
            {
                ModalMessages::getInstance()->stopLoading();
                MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
                return;
            }
        }
    }
}

void ApplePaymentSingleton::ErrorMessage()
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
}

void ApplePaymentSingleton::DoublePurchase()
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_PURCHASE_DOUBLE, nullptr);
}

void ApplePaymentSingleton::backendRequestFailed(long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    
    if(!makingMonthlyPayment)
    {
        if(refreshFromButton && errorCode == 400)
            MessageBox::createWith(ERROR_CODE_APPLE_NO_PREVIOUS_PURCHASE, nullptr);
        else if(errorCode == 400)
            BackEndCaller::getInstance()->autoLogin();
        else
            MessageBox::createWith(ERROR_CODE_APPLE_SUB_REFRESH_FAIL, this);
    }
    else
        MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
}

//---------Delegate Functions----------
void ApplePaymentSingleton::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    BackEndCaller::getInstance()->autoLogin();
}
