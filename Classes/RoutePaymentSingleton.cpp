#include "RoutePaymentSingleton.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "AmazonPaymentSingleton.h"
#include "GooglePaymentSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "LoginLogicHandler.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include "FlowDataSingleton.h"
#include "SceneManagerScene.h"
#include "AzoomeeCommon/Data/Child/ChildDataParser.h"
#include "DynamicNodeHandler.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "platform/android/jni/JniHelper.h"

    static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   #include "ApplePaymentSingleton.h"
#endif


using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string& RoutePaymentSingleton::kReceiptCacheFolder = "receiptCache/";
const std::string& RoutePaymentSingleton::kReceiptDataFileName = "receiptData.dat";

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
    if(receiptDataFileExists())
    {
        if(ParentDataProvider::getInstance()->isLoggedInParentAnonymous() || ParentDataProvider::getInstance()->getLoggedInParentId() == "")
        {
            DynamicNodeHandler::getInstance()->startSignupFlow();
        }
        else
        {
            retryReceiptValidation();
        }
    }
    
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

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(ParentDataProvider::getInstance()->getBillingProvider() == "APPLE" && isDateStringOlderThanToday(ParentDataProvider::getInstance()->getBillingDate()))
        {
            pressedIAPStartButton = false;
            pressedRestorePurchaseButton = false;
            ApplePaymentSingleton::getInstance()->refreshReceipt(false);
            return false;
        }
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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
    if(errorCode == 409) //409 means the user was already upgraded, so we can remove the local receipt file.
    {
        RoutePaymentSingleton::getInstance()->removeReceiptDataFile();
    }
    
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
    FlowDataSingleton::getInstance()->setSuccessFailPath(IAP_FAIL);
    DynamicNodeHandler::getInstance()->handleSuccessFailEvent();
}

void RoutePaymentSingleton::doublePurchaseMessage()
{
    AnalyticsSingleton::getInstance()->iapSubscriptionDoublePurchaseEvent();
    Azoomee::ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_PURCHASE_DOUBLE, nullptr);
}

void RoutePaymentSingleton::inAppPaymentSuccess()
{
    removeReceiptDataFile();
    
    AnalyticsSingleton::getInstance()->iapSubscriptionSuccessEvent();
    
    BackEndCaller::getInstance()->updateBillingData();
    FlowDataSingleton::getInstance()->addIAPSuccess(true);
    if(FlowDataSingleton::getInstance()->isSignupFlow())
    {
        FlowDataSingleton::getInstance()->setSuccessFailPath(PREMIUM_NEW_ACCOUNT);
    }
    else
    {
        FlowDataSingleton::getInstance()->setSuccessFailPath(PREMIUM_EXISTING_ACCOUNT);
    }
    
    ChildDataParser::getInstance()->setChildLoggedIn(false);
    BackEndCaller::getInstance()->getAvailableChildren();
}

void RoutePaymentSingleton::writeAndroidReceiptDataToFile(const std::string& developerPayload, const std::string& orderId, const std::string& token)
{
    const std::string& stringToWrite = developerPayload + "|" + orderId + "|" + token;
    writeReceiptDataToFile(stringToWrite);
}

void RoutePaymentSingleton::writeAmazonReceiptDataToFile(const std::string &requestId, const std::string &receiptId, const std::string &amazonUserId)
{
    const std::string& stringToWrite = requestId + "|" + receiptId + "|" + amazonUserId;
    writeReceiptDataToFile(stringToWrite);
}

void RoutePaymentSingleton::writeReceiptDataToFile(const std::string &receiptData)
{
    int attemptNumber = 0;
    
    if(receiptDataFileExists())
    {
        attemptNumber = atoi(splitStringToVector(FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getDocumentsPath() + kReceiptCacheFolder + kReceiptDataFileName), "||").at(0).c_str());
        attemptNumber++;
    }
    
    createReceiptDataFolder();
    FileUtils::getInstance()->writeStringToFile(StringUtils::format("%d", attemptNumber) + "||" + receiptData, FileUtils::getInstance()->getDocumentsPath() + kReceiptCacheFolder + kReceiptDataFileName);
}

bool RoutePaymentSingleton::receiptDataFileExists()
{
    return FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getDocumentsPath() + kReceiptCacheFolder + kReceiptDataFileName);
}

void RoutePaymentSingleton::removeReceiptDataFile()
{
    if(receiptDataFileExists())
    {
        FileUtils::getInstance()->removeFile(FileUtils::getInstance()->getDocumentsPath() + kReceiptCacheFolder + kReceiptDataFileName);
    }
}

void RoutePaymentSingleton::retryReceiptValidation()
{
    if(!receiptDataFileExists())
    {
        return;
    }
    
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getDocumentsPath() + kReceiptCacheFolder + kReceiptDataFileName);
    const std::vector<std::string>& fileContentSplit = splitStringToVector(fileContent, "||");
    
    if(fileContentSplit.size() != 2)
    {
        return;
    }
    
    int attemptNumber = atoi(fileContentSplit.at(0).c_str());
    const std::string& dataString = fileContentSplit.at(1);
    
    if(attemptNumber > 2) //we want to avoid putting users to infinite loop
    {
        //TODO: send receipt file data to back-end
        return;
    }
    
    if(dataString != "")
    {
        writeReceiptDataToFile(dataString); //we do this to increase the attempt number by 1. If dataString is empty because of any issues, it is better not to rewrite the file.
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ApplePaymentSingleton::getInstance()->transactionStatePurchased(dataString);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    const std::vector<std::string>& paymentElements = splitStringToVector(dataString, "|");
    if(paymentElements.size() == 3)
    {
        if(osIsAmazon())
        {
            AmazonPaymentSingleton::getInstance()->amazonPaymentMade(paymentElements.at(0), paymentElements.at(1), paymentElements.at(2));
        }
        
        if(osIsAndroid())
        {
            GooglePaymentSingleton::getInstance()->startBackEndPaymentVerification(paymentElements.at(0), paymentElements.at(1), paymentElements.at(2));
        }
    }
#endif
    
}

void RoutePaymentSingleton::createReceiptDataFolder()
{
    if(!FileUtils::getInstance()->isDirectoryExist(FileUtils::getInstance()->getDocumentsPath() + kReceiptCacheFolder))
    {
        FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getDocumentsPath() + kReceiptCacheFolder);
    }
}

//Delegate Functions
void RoutePaymentSingleton::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    LoginLogicHandler::getInstance()->doLoginLogic();
}

NS_AZOOMEE_END
