#ifndef __ROUTE_PAYMENT_SINGLETON_H__
#define __ROUTE_PAYMENT_SINGLETON_H__

#include "cocos2d.h"

class RoutePaymentSingleton : public cocos2d::Ref
{
    
public:
    static RoutePaymentSingleton* getInstance(void);
    virtual ~RoutePaymentSingleton();
    bool init(void);
    
    void startInAppPayment();
    
    std::string getOSManufacturer();
    
    bool showIAPContent();
    
    bool osIsIos();
    bool osIsAndroid();
    bool osIsAmazon();
    
    void refreshAppleReceiptFromButton();
    void checkIfAppleReceiptRefreshNeeded();
};

#endif
