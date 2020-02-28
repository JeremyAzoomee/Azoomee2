#include "ApplePaymentSingleton.h"
#include "PaymentViewController_ios.h"
#include "ModalMessages.h"
#include "external/json/document.h"
#include <TinizineCommon/Analytics/AnalyticsSingleton.h>
#include "BackEndCaller.h"
#include <TinizineCommon/Data/Parent/UserAccountManager.h>
#include "LoginController.h"
#include "RoutePaymentSingleton.h"
#include "FlowDataSingleton.h"
#include "PopupMessageBox.h"
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/Colour.h>
#include "ErrorCodes.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

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
    
    if(!UserAccountManager::getInstance()->isUserLoggedIn())
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
        if(UserAccountManager::getInstance()->isPaidUser())
        {
            ModalMessages::getInstance()->stopLoading();
            const auto& errorMessageText = LocaleManager::getInstance()->getErrorMessageWithCode(ERROR_CODE_APPLE_ACCOUNT_DOWNGRADED);
                   
            PopupMessageBox* messageBox = PopupMessageBox::create();
            messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
            messageBox->setBody(errorMessageText.at(ERROR_BODY));
            messageBox->setButtonText(_("Back"));
            messageBox->setButtonColour(Colours::Color_3B::darkIndigo);
            messageBox->setPatternColour(Colours::Color_3B::azure);
            messageBox->setButtonPressedCallback([this](MessagePopupBase* pSender){
                pSender->removeFromParent();
                LoginController::getInstance()->doLoginLogic();
            });
            Director::getInstance()->getRunningScene()->addChild(messageBox, 1);
        }
        else
        {
            RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage("AnswerRecieved-RequestAttempts>4");
            return;
        }
    }
}

NS_AZ_END
