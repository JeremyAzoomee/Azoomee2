#include "ApplePaymentSingleton.h"
#include "PaymentViewController_ios.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "external/json/document.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "LoginLogicHandler.h"
#include "RoutePaymentSingleton.h"

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
    [[PaymentViewController sharedPayment_ios] makeOneMonthPayment];
}

void ApplePaymentSingleton::refreshReceipt(bool usingButton)
{
    requestAttempts = 0;
    [[PaymentViewController sharedPayment_ios] restorePayment];
}

void ApplePaymentSingleton::transactionStatePurchased(std::string receiptData)
{
    savedReceipt = receiptData;
    
    BackEndCaller::getInstance()->verifyApplePayment(receiptData);
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

    if(paymentData.HasMember("receiptStatus"))
    {
        if(paymentData["receiptStatus"].IsString())
        {
            if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED" && RoutePaymentSingleton::getInstance()->pressedIAPStartButton)
            {
                RoutePaymentSingleton::getInstance()->inAppPaymentSuccess();
                return;
            }
            else if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED")
            {
                AnalyticsSingleton::getInstance()->iapAppleAutoRenewSubscriptionEvent();
                ModalMessages::getInstance()->stopLoading();
                BackEndCaller::getInstance()->updateBillingData();
                return;
            }
            else
                AnalyticsSingleton::getInstance()->iapSubscriptionErrorEvent(StringUtils::format("%s", paymentData["receiptStatus"].GetString()));
        }
    }
    
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
            RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage();
            return;
        }
    }
}

//---------Delegate Functions----------
void ApplePaymentSingleton::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle,int buttonNumber)
{
    LoginLogicHandler::getInstance()->doLoginLogic();
}
