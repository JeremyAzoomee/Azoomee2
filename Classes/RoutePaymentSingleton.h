#ifndef __ROUTE_PAYMENT_SINGLETON_H__
#define __ROUTE_PAYMENT_SINGLETON_H__

#include "cocos2d.h"
#include <AzoomeeCommon/Data/Json.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class RoutePaymentSingleton : public cocos2d::Ref
{
private:
    bool appleReceiptRefreshchecked = false;
    void createReceiptDataFolder();
    void removeReceiptDataFileAndLogin();
    void writeReceiptDataToFile(const std::string &receiptData);
    
    rapidjson::Document _iapConfiguration;
    
public:
    static RoutePaymentSingleton* getInstance(void);
    virtual ~RoutePaymentSingleton();
    bool init(void);
    
    bool pressedIAPStartButton = false;
    bool pressedRestorePurchaseButton = false;
    
    void startInAppPayment();
    void inAppPaymentSuccess();
    
#if defined(AZOOMEE_ENVIRONMENT_CI)
    void startIOSRecPayment();
#endif
    
    bool showIAPContent();
    
    bool osIsIos();
    bool osIsAndroid();
    bool osIsAmazon();
	
	void restorePayment();
    
    void backendRequestFailed(long errorCode);
    
    void purchaseFailureErrorMessage(const std::string& failureDetails);
    void doublePurchaseMessage();
	void failedRestoreMessage();
    void canceledAction();
	
    void writeAppleReceiptDataToFile(const std::string& receiptData, const std::string& transactionID);
    void writeAndroidReceiptDataToFile(const std::string& developerPayload, const std::string& orderId, const std::string& token);
    void writeAmazonReceiptDataToFile(const std::string& requestId, const std::string& receiptId, const std::string& amazonUserId);
    
    bool receiptDataFileExists();
    void removeReceiptDataFile();
    void retryReceiptValidation();
    
    std::string getIapSkuForProvider(const std::string& provider);
    
    static const std::string& kReceiptCacheFolder;
    static const std::string& kReceiptDataFileName;
	
	static const std::string kPaymentSuccessfulEventName;
	static const std::string kPaymentFailedEventName;
};

NS_AZOOMEE_END

#endif
