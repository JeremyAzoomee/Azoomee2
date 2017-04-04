#ifndef __GOOGLE_PAYMENT_SINGLETON_H__
#define __GOOGLE_PAYMENT_SINGLETON_H__

#include "cocos2d.h"

class GooglePaymentSingleton : public cocos2d::Ref
{
    
public:
    static GooglePaymentSingleton* getInstance(void);
    virtual ~GooglePaymentSingleton();
    bool init(void);
    
    void startIABPayment();
    void finishedIABPayment();
    void purchaseFailed();
    void showDoublePurchase();
    
private:
    bool paymentInProgress;
    void createModalLayer();
    void removeModalLayer();
    cocos2d::LayerColor* modalLayer;
    void addListenerToBackgroundLayer();
};

#endif
