#ifndef __ROUTE_PAYMENT_SINGLETON_H__
#define __ROUTE_PAYMENT_SINGLETON_H__

#include "cocos2d.h"
#include "MessageBox.h"

class RoutePaymentSingleton : public cocos2d::Ref, public MessageBoxDelegate
{
private:
    bool appleReceiptRefreshchecked;
    std::string OSManufacturer;
    
public:
    static RoutePaymentSingleton* getInstance(void);
    virtual ~RoutePaymentSingleton();
    bool init(void);
    
    void startInAppPayment();
    
    void setOSManufacturer();
    
    bool showIAPContent();
    
    bool osIsIos();
    bool osIsAndroid();
    bool osIsAmazon();
    
    void refreshAppleReceiptFromButton();
    bool checkIfAppleReceiptRefreshNeeded();
    
    //Delegate Functions
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
};

#endif
