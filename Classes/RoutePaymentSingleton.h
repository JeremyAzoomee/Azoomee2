#ifndef __ROUTE_PAYMENT_SINGLETON_H__
#define __ROUTE_PAYMENT_SINGLETON_H__

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/MessageBox.h>

NS_AZOOMEE_BEGIN

class RoutePaymentSingleton : public cocos2d::Ref, public MessageBoxDelegate
{
private:
    bool appleReceiptRefreshchecked = false;
    void createReceiptDataFolder();
    void removeReceiptDataFileAndLogin();
    
public:
    static RoutePaymentSingleton* getInstance(void);
    virtual ~RoutePaymentSingleton();
    bool init(void);
    
    bool pressedIAPStartButton = false;
    bool pressedRestorePurchaseButton = false;
    
    void startInAppPayment();
    void inAppPaymentSuccess();
    
    bool showIAPContent();
    
    bool osIsIos();
    bool osIsAndroid();
    bool osIsAmazon();
	
	void restorePayment();
    
    void backendRequestFailed(long errorCode);
    
    void purchaseFailureErrorMessage(const std::string& failureDetails);
    void doublePurchaseMessage();
	void failedRestoreMessage();
	
    void writeReceiptDataToFile(const std::string &receiptData);
    void writeAndroidReceiptDataToFile(const std::string& developerPayload, const std::string& orderId, const std::string& token);
    void writeAmazonReceiptDataToFile(const std::string& requestId, const std::string& receiptId, const std::string& amazonUserId);
    
    bool receiptDataFileExists();
    void removeReceiptDataFile();
    void retryReceiptValidation();
    
    //Delegate Functions
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    static const std::string& kReceiptCacheFolder;
    static const std::string& kReceiptDataFileName;
	
	static const std::string kPaymentSuccessfulEventName;
	static const std::string kPaymentFailedEventName;
};

NS_AZOOMEE_END

#endif
