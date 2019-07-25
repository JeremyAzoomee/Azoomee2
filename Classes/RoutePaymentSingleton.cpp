#include "RoutePaymentSingleton.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "AmazonPaymentSingleton.h"
#include "GooglePaymentSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Utils/DirUtil.h>
#include "LoginLogicHandler.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include "FlowDataSingleton.h"
#include "SceneManagerScene.h"
#include "AzoomeeCommon/Data/Child/ChildManager.h"
#include "DynamicNodeHandler.h"
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
        if(!ParentManager::getInstance()->isUserLoggedIn())
        {
            FlowDataSingleton::getInstance()->setSuccessFailPath(IAP_SUCCESS);
            DynamicNodeHandler::getInstance()->handleSuccessFailEvent();
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
    return !ParentManager::getInstance()->isPaidUser();
}

bool RoutePaymentSingleton::osIsIos()
{
    return (ConfigStorage::getInstance()->getOSManufacturer() == "Apple");
}

bool RoutePaymentSingleton::osIsAndroid()
{
    return (ConfigStorage::getInstance()->getOSManufacturer() == "Google");
}

bool RoutePaymentSingleton::osIsAmazon()
{
    return (ConfigStorage::getInstance()->getOSManufacturer() == "Amazon");
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
    if(errorCode == 409 || errorCode == 422) //409 means the user was already upgraded, so we can remove the local receipt file.
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
    //FlowDataSingleton::getInstance()->setSuccessFailPath(IAP_FAIL);
    //DynamicNodeHandler::getInstance()->handleSuccessFailEvent();
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

void RoutePaymentSingleton::inAppPaymentSuccess()
{
    removeReceiptDataFile();
    
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
    
    ChildManager::getInstance()->setChildLoggedIn(false);
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
    LoginLogicHandler::getInstance()->doLoginLogic();
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
    LoginLogicHandler::getInstance()->doLoginLogic();
}

NS_AZOOMEE_END
