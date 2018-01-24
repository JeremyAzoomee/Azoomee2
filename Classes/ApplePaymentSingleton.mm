#include "ApplePaymentSingleton.h"
#include "PaymentViewController_ios.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "external/json/document.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "LoginLogicHandler.h"
#include "RoutePaymentSingleton.h"
#include "DynamicNodeHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    
    if(!RoutePaymentSingleton::getInstance()->receiptDataFileExists())
    {
        RoutePaymentSingleton::getInstance()->writeReceiptDataToFile(receiptData);
    }
    
    if(ParentDataProvider::getInstance()->isLoggedInParentAnonymous() || ParentDataProvider::getInstance()->getLoggedInParentId() == "")
    {
        ModalMessages::getInstance()->stopLoading();
        DynamicNodeHandler::getInstance()->startSignupFlow();
    }
    else
    {
        BackEndCaller::getInstance()->verifyApplePayment(receiptData);
    }
}

void ApplePaymentSingleton::onAnswerReceived(std::string responseDataString)
{
    rapidjson::Document paymentData;
    paymentData.Parse(responseDataString.c_str());

    if(!paymentData.HasParseError() && paymentData.HasMember("receiptStatus"))
    {
        if(std::string(paymentData["receiptStatus"].GetString()) == "FULFILLED" && RoutePaymentSingleton::getInstance()->pressedIAPStartButton)
        {
            ModalMessages::getInstance()->stopLoading();
            RoutePaymentSingleton::getInstance()->inAppPaymentSuccess();
            return;
        }
        else if(std::string(paymentData["receiptStatus"].GetString()) == "FULFILLED")
        {
            AnalyticsSingleton::getInstance()->iapAppleAutoRenewSubscriptionEvent();
            ModalMessages::getInstance()->stopLoading();
            BackEndCaller::getInstance()->updateBillingData();
            return;
        }
        else
            AnalyticsSingleton::getInstance()->iapSubscriptionErrorEvent(StringUtils::format("%s", paymentData["receiptStatus"].GetString()));

    }
    
    if(requestAttempts < 4)
    {
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage("AnswerRecieved-RequestAttempts<4");
        requestAttempts = requestAttempts + 1;
        transactionStatePurchased(savedReceipt);
        return;
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
            RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage("AnswerRecieved-RequestAttempts>4");
            return;
        }
    }
}

//---------Delegate Functions----------
void ApplePaymentSingleton::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    LoginLogicHandler::getInstance()->doLoginLogic();
}

NS_AZOOMEE_END
