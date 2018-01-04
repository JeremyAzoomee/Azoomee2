#include "RoutePaymentSingleton.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "AmazonPaymentSingleton.h"
#include "GooglePaymentSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "LoginLogicHandler.h"
#include "OnboardingSuccessScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include "FlowDataSingleton.h"
#include "SceneManagerScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "platform/android/jni/JniHelper.h"

    static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   #include "ApplePaymentSingleton.h"
#endif


using namespace cocos2d;

NS_AZOOMEE_BEGIN

static RoutePaymentSingleton *_sharedRoutePaymentSingleton = NULL;

RoutePaymentSingleton* RoutePaymentSingleton::getInstance()
{
    if (! _sharedRoutePaymentSingleton)
    {
        _sharedRoutePaymentSingleton = new RoutePaymentSingleton();
        _sharedRoutePaymentSingleton->init();
    }
    
    return _sharedRoutePaymentSingleton;
}

RoutePaymentSingleton::~RoutePaymentSingleton(void)
{
}

bool RoutePaymentSingleton::init(void)
{
    setOSManufacturer();
    return true;
}
void RoutePaymentSingleton::startInAppPayment()
{
    pressedIAPStartButton = true;
    pressedRestorePurchaseButton = false;
    ModalMessages::getInstance()->startLoading();
    if(osIsIos())
    {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            ApplePaymentSingleton::getInstance()->startIAPPayment();
        #endif
        return;
    }
    
    if(osIsAndroid())
    {
        GooglePaymentSingleton::getInstance()->startIABPayment();
        return;
    }
    
    if(osIsAmazon())
    {
        AmazonPaymentSingleton::getInstance()->startIAPPayment();
        return;
    }
}

bool RoutePaymentSingleton::showIAPContent()
{
    return !ParentDataProvider::getInstance()->isPaidUser();
}

void RoutePaymentSingleton::setOSManufacturer()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    std::string resultStr = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getOSBuildManufacturer");
    
    if (resultStr == "Amazon")
    {
        AnalyticsSingleton::getInstance()->registerIAPOS("Amazon");
        OSManufacturer = "Amazon";
    }
    else
    {
        AnalyticsSingleton::getInstance()->registerIAPOS("Google");
        OSManufacturer = "Google";
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AnalyticsSingleton::getInstance()->registerIAPOS("iOS");
    OSManufacturer = "iOS";
#else
#error Unsupported platform in RoutePaymentSingleton
#endif

}

bool RoutePaymentSingleton::osIsIos()
{
    return (OSManufacturer == "iOS");
}

bool RoutePaymentSingleton::osIsAndroid()
{
    return (OSManufacturer == "Google");
}

bool RoutePaymentSingleton::osIsAmazon()
{
    return (OSManufacturer == "Amazon");
}

void RoutePaymentSingleton::refreshAppleReceiptFromButton()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        pressedIAPStartButton = false;
        pressedRestorePurchaseButton = true;
        ModalMessages::getInstance()->startLoading();
        ApplePaymentSingleton::getInstance()->refreshReceipt(true);
    #endif
}

bool RoutePaymentSingleton::checkIfAppleReceiptRefreshNeeded()
{
    if(appleReceiptRefreshchecked)
        return true;
    else
    {
        appleReceiptRefreshchecked = true;
        
        cocos2d::log("checkIfAppleReceiptRefreshNeeded Started");
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(ParentDataProvider::getInstance()->getBillingProvider() == "APPLE" && isDateStringOlderThanToday(ParentDataProvider::getInstance()->getBillingDate()))
        {
            pressedIAPStartButton = false;
            pressedRestorePurchaseButton = false;
            ApplePaymentSingleton::getInstance()->refreshReceipt(false);
            return false;
        }
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        cocos2d::log("checkIfAppleReceiptRefreshNeeded");
        if(ParentDataProvider::getInstance()->getBillingProvider() == "APPLE" && !ParentDataProvider::getInstance()->isPaidUser())
        {
            MessageBox::createWith(ERROR_CODE_APPLE_SUBSCRIPTION_ON_NON_APPLE, this);
            return false;
        }
    #endif
        return true;
    }
}

void RoutePaymentSingleton::backendRequestFailed(long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    
    if(pressedIAPStartButton)
        MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, this);
    else
    {
        if(pressedRestorePurchaseButton && errorCode == 400)
            MessageBox::createWith(ERROR_CODE_APPLE_NO_PREVIOUS_PURCHASE, nullptr);
        else if(pressedRestorePurchaseButton && errorCode == 409)
            doublePurchaseMessage();
        else if(errorCode == 409 || errorCode == 400)
            LoginLogicHandler::getInstance()->doLoginLogic();
        else
            MessageBox::createWith(ERROR_CODE_APPLE_SUB_REFRESH_FAIL, this);
    }
}

void RoutePaymentSingleton::purchaseFailureErrorMessage(const std::string& failureDetails)
{
    AnalyticsSingleton::getInstance()->iapSubscriptionFailedEvent(failureDetails);
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
}

void RoutePaymentSingleton::doublePurchaseMessage()
{
    AnalyticsSingleton::getInstance()->iapSubscriptionDoublePurchaseEvent();
    Azoomee::ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_PURCHASE_DOUBLE, nullptr);
}

void RoutePaymentSingleton::inAppPaymentSuccess()
{
    AnalyticsSingleton::getInstance()->iapSubscriptionSuccessEvent();
    
    BackEndCaller::getInstance()->updateBillingData();
    FlowDataSingleton::getInstance()->addIAPSuccess(true);
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(OnboardingSuccessScene));
}

//Delegate Functions
void RoutePaymentSingleton::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    LoginLogicHandler::getInstance()->doLoginLogic();
}

NS_AZOOMEE_END
