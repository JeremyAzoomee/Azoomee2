#include "RoutePaymentSingleton.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "AmazonPaymentSingleton.h"
#include "GooglePaymentSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Utils/DirUtil.h>
#include "LoginController.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include "FlowDataSingleton.h"
#include "SceneManagerScene.h"
#include "AzoomeeCommon/Data/Child/ChildManager.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>

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

const std::string RoutePaymentSingleton::kPaymentSuccessfulEventName = "azoomee.paymentSuccessEvent";
const std::string RoutePaymentSingleton::kPaymentFailedEventName = "azoomee.paymentFailedEvent";

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
    return true;
}
void RoutePaymentSingleton::startInAppPayment()
{
    if(receiptDataFileExists())
    {
        if(!UserAccountManager::getInstance()->isUserLoggedIn())
        {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kPaymentSuccessfulEventName);
        }
        else
        {
            retryReceiptValidation();
        }
        return;
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
    return !UserAccountManager::getInstance()->isPaidUser();
}

bool RoutePaymentSingleton::osIsIos()
{
    return (ConfigStorage::getInstance()->getOSManufacturer() == ConfigStorage::kOSManufacturerApple);
}

bool RoutePaymentSingleton::osIsAndroid()
{
    return (ConfigStorage::getInstance()->getOSManufacturer() == ConfigStorage::kOSManufacturerGoogle);
}

bool RoutePaymentSingleton::osIsAmazon()
{
    return (ConfigStorage::getInstance()->getOSManufacturer() == ConfigStorage::kOSManufacturerAmazon);
}

void RoutePaymentSingleton::restorePayment()
{
	pressedIAPStartButton = false;
	pressedRestorePurchaseButton = true;
	if(osIsIos())
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		ApplePaymentSingleton::getInstance()->refreshReceipt(true);
#endif
	}
	else if(osIsAndroid())
	{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		GooglePaymentSingleton::getInstance()->startRestorePurchase();
	#endif
	}
	else
	{
		return;
	}
	ModalMessages::getInstance()->startLoading();
}

void RoutePaymentSingleton::backendRequestFailed(long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    
    if(errorCode == 409) // this if for any error involving duplicate/already redeemed purchases, from attempted payment restore with already redeemed purchase or trying to subscribe again
    {
        doublePurchaseMessage();
        RoutePaymentSingleton::getInstance()->removeReceiptDataFile();
    }
    else // generic error message for 400 and 403 errors where either the request was invalid or signatures didnt match up do access was denied
    {
        MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, this);
    }
}

void RoutePaymentSingleton::purchaseFailureErrorMessage(const std::string& failureDetails)
{
    AnalyticsSingleton::getInstance()->iapSubscriptionFailedEvent(failureDetails);
    ModalMessages::getInstance()->stopLoading();
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kPaymentFailedEventName);
}

void RoutePaymentSingleton::doublePurchaseMessage()
{
    AnalyticsSingleton::getInstance()->iapSubscriptionDoublePurchaseEvent();
    Azoomee::ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_PURCHASE_DOUBLE, nullptr);
}

void RoutePaymentSingleton::failedRestoreMessage()
{
	AnalyticsSingleton::getInstance()->iapSubscriptionErrorEvent("failed restore - no purchase");
	Azoomee::ModalMessages::getInstance()->stopLoading();
	MessageBox::createWith(ERROR_CODE_APPLE_NO_PREVIOUS_PURCHASE, nullptr);
}

void RoutePaymentSingleton::canceledAction()
{
    Azoomee::ModalMessages::getInstance()->stopLoading();
}

void RoutePaymentSingleton::inAppPaymentSuccess()
{
    removeReceiptDataFile();
    LoginController::getInstance()->handleLoginSuccess();
}

void RoutePaymentSingleton::writeAppleReceiptDataToFile(const std::string& receiptData, const std::string& transactionID)
{
    const std::string& stringToWrite = receiptData + "|" + transactionID;
    writeReceiptDataToFile(stringToWrite);
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
        attemptNumber = atoi(splitStringToVector(FileUtils::getInstance()->getStringFromFile(DirUtil::getDocumentsPath() + kReceiptCacheFolder + kReceiptDataFileName), "||").at(0).c_str());
        attemptNumber++;
    }
    
    createReceiptDataFolder();
    FileUtils::getInstance()->writeStringToFile(StringUtils::format("%d", attemptNumber) + "||" + receiptData, DirUtil::getDocumentsPath() + kReceiptCacheFolder + kReceiptDataFileName);
}

bool RoutePaymentSingleton::receiptDataFileExists()
{
    return FileUtils::getInstance()->isFileExist(DirUtil::getDocumentsPath() + kReceiptCacheFolder + kReceiptDataFileName);
}

void RoutePaymentSingleton::removeReceiptDataFile()
{
    if(receiptDataFileExists())
    {
        FileUtils::getInstance()->removeFile(DirUtil::getDocumentsPath() + kReceiptCacheFolder + kReceiptDataFileName);
    }
}

void RoutePaymentSingleton::removeReceiptDataFileAndLogin()
{
    removeReceiptDataFile();
    LoginController::getInstance()->doLoginLogic();
}

void RoutePaymentSingleton::retryReceiptValidation()
{
    if(!receiptDataFileExists())
    {
        removeReceiptDataFileAndLogin();
        return;
    }
    
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(DirUtil::getDocumentsPath() + kReceiptCacheFolder + kReceiptDataFileName);
    const std::vector<std::string>& fileContentSplit = splitStringToVector(fileContent, "||");
    
    if(fileContentSplit.size() != 2)
    {
        //TODO: send receipt file data to back-end
        removeReceiptDataFileAndLogin();
        return;
    }
    
    int attemptNumber = atoi(fileContentSplit.at(0).c_str());
    const std::string& dataString = fileContentSplit.at(1);
    
    if(attemptNumber > 6) //we want to avoid putting users to infinite loop
    {
        //TODO: send receipt file data to back-end
        removeReceiptDataFileAndLogin();
        return;
    }
    
    if(dataString != "")
    {
        writeReceiptDataToFile(dataString); //we do this to increase the attempt number by 1. If dataString is empty because of any issues, it is better not to rewrite the file.
    }
    
    pressedIAPStartButton = true;
    pressedRestorePurchaseButton = false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    const std::vector<std::string>& paymentElements = splitStringToVector(dataString, "|");
    const std::string& receiptData = paymentElements[0];
    // Get transactionID if we stored it. It may not exist if the receipt was saved in an older version
    const std::string& transactionID = (paymentElements.size() > 1) ? paymentElements[1] : "";
    ApplePaymentSingleton::getInstance()->transactionStatePurchased(receiptData, transactionID);
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
    else
    {
        removeReceiptDataFileAndLogin();
        return;
    }
#endif
    
}

void RoutePaymentSingleton::createReceiptDataFolder()
{
    const std::string& cacheFolder = DirUtil::getDocumentsPath() + kReceiptCacheFolder;
    if(!FileUtils::getInstance()->isDirectoryExist(cacheFolder))
    {
        FileUtils::getInstance()->createDirectory(cacheFolder);
    }
}

//Delegate Functions
void RoutePaymentSingleton::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    LoginController::getInstance()->doLoginLogic();
}

NS_AZOOMEE_END
