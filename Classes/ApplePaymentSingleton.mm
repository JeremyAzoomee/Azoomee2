#include "ApplePaymentSingleton.h"
#include "PaymentViewController_ios.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "external/json/document.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "BackEndCaller.h"
#include "HttpRequestCreator.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "OnboardingSuccessScene.h"

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

void ApplePaymentSingleton::startIAPPayment()
{
    requestAttempts = 0;
    makingMonthlyPayment = true;
    [[PaymentViewController sharedPayment_ios] makeOneMonthPayment];
}

void ApplePaymentSingleton::refreshReceipt(bool usingButton)
{
    requestAttempts = 0;
    makingMonthlyPayment = false;
    refreshFromButton = usingButton;
    [[PaymentViewController sharedPayment_ios] restorePayment];
}

void ApplePaymentSingleton::transactionStatePurchased(std::string receiptData)
{
    savedReceipt = receiptData;
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestBody = StringUtils::format("{\"receipt-data\": \"%s\"}", receiptData.c_str());
    httpRequestCreator->requestTag = "iapApplePaymentMade";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

void ApplePaymentSingleton::onAnswerReceived(std::string responseDataString)
{
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
            if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED" && makingMonthlyPayment)
            {
                AnalyticsSingleton::getInstance()->iapSubscriptionSuccessEvent();
                paymentFailed = false;

                BackEndCaller::getInstance()->updateBillingData();
                auto onboardingSuccessScene = OnboardingSuccessScene::createScene(true);
                Director::getInstance()->replaceScene(onboardingSuccessScene);
            }
            else if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED")
            {
                AnalyticsSingleton::getInstance()->iapAppleAutoRenewSubscriptionEvent();
                ModalMessages::getInstance()->stopLoading();
                
                paymentFailed = false;
                BackEndCaller::getInstance()->updateBillingData();
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

void ApplePaymentSingleton::ErrorMessage(bool loginAfterOK)
{
    ModalMessages::getInstance()->stopLoading();
    if(loginAfterOK)
        MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, this);
    else
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
        if(errorCode == 409)
            BackEndCaller::getInstance()->autoLogin();
        else if(refreshFromButton && errorCode == 400)
            MessageBox::createWith(ERROR_CODE_APPLE_NO_PREVIOUS_PURCHASE, nullptr);
        else if(errorCode == 400)
            BackEndCaller::getInstance()->autoLogin();
        else
            MessageBox::createWith(ERROR_CODE_APPLE_SUB_REFRESH_FAIL, this);
    }
    else
        MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, this);
}

//---------Delegate Functions----------
void ApplePaymentSingleton::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    BackEndCaller::getInstance()->autoLogin();
}
