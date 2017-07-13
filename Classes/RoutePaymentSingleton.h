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
    std::string OSManufacturer;
    void setOSManufacturer();
    
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
    
    void refreshAppleReceiptFromButton();
    bool checkIfAppleReceiptRefreshNeeded();
    
    void backendRequestFailed(long errorCode);
    
    void purchaseFailureErrorMessage(std::string failureDetails);
    void doublePurchaseMessage();
    
    //Delegate Functions
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
};

NS_AZOOMEE_END

#endif
