#include "ApplePaymentSingleton.h"
#include "PaymentViewController_ios.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "external/json/document.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include "LoginLogicHandler.h"
#include "RoutePaymentSingleton.h"
#include "FlowDataSingleton.h"
#include <AzoomeeCommon/Data/User/UserAccountManager.h>

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
    _requestAttempts = 0;
    [[PaymentViewController sharedPayment_ios] makeOneMonthPayment];
}

void ApplePaymentSingleton::refreshReceipt(bool usingButton)
{
    _requestAttempts = 0;
    [[PaymentViewController sharedPayment_ios] restorePayment];
}

void ApplePaymentSingleton::transactionStatePurchased(const std::string& receiptData, const std::string& transactionID)
{
    _savedReceipt = receiptData;
    _transactionID = transactionID;
    
    if(!RoutePaymentSingleton::getInstance()->receiptDataFileExists())
    {
        RoutePaymentSingleton::getInstance()->writeAppleReceiptDataToFile(receiptData, transactionID);
        AnalyticsSingleton::getInstance()->iapSubscriptionSuccessEvent();
    }
    
    if(!ParentManager::getInstance()->isUserLoggedIn())
    {
        ModalMessages::getInstance()->stopLoading();
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(RoutePaymentSingleton::kPaymentSuccessfulEventName);
    }
    else
    {
        BackEndCaller::getInstance()->verifyApplePayment(receiptData, transactionID);
    }
}

void ApplePaymentSingleton::onAnswerReceived(const std::string& responseDataString)
{
    rapidjson::Document paymentData;
    paymentData.Parse(responseDataString.c_str());

    if(!paymentData.HasParseError() && paymentData.HasMember("receiptStatus"))
    {
        const std::string& receiptStatus = paymentData["receiptStatus"].GetString();
        if(receiptStatus == "FULFILLED" && RoutePaymentSingleton::getInstance()->pressedIAPStartButton)
        {
            ModalMessages::getInstance()->stopLoading();
            RoutePaymentSingleton::getInstance()->inAppPaymentSuccess();
            return;
        }
        else if(receiptStatus == "FULFILLED")
        {
            AnalyticsSingleton::getInstance()->iapAppleAutoRenewSubscriptionEvent();
            ModalMessages::getInstance()->stopLoading();
            UserAccountManager::getInstance()->getBillingDataForLoggedInParent(nullptr);
            //BackEndCaller::getInstance()->updateBillingData();
            return;
        }
        else
            AnalyticsSingleton::getInstance()->iapSubscriptionErrorEvent(StringUtils::format("%s", paymentData["receiptStatus"].GetString()));

    }
    
    if(_requestAttempts < 4)
    {
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage("AnswerRecieved-RequestAttempts<4");
        _requestAttempts = _requestAttempts + 1;
        transactionStatePurchased(_savedReceipt, _transactionID);
        return;
    }
    else
    {
        if(ParentManager::getInstance()->isPaidUser())
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
