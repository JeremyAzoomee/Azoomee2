#ifndef __PAYMENT_SINGLETON_H__
#define __PAYMENT_SINGLETON_H__

#include "cocos2d.h"

class PaymentSingleton : public cocos2d::Ref
{
    
public:
    static PaymentSingleton* getInstance(void);
    virtual ~PaymentSingleton();
    bool init(void);
    
    void onAmazonPaymentMadeAnswerReceived(std::string responseDataString);
    void amazonPaymentMade(std::string requestId, std::string receiptId, std::string amazonUserid);
    void startAmazonPayment();
    
    void purchaseFailed();
    
    bool enableIAP();
    
    void removeModalLayer();
    
private:
    void fulfillAmazonPayment(std::string receiptId);
    
    bool isEnabledIAP;
    void setupIsEnabledIAP();
    
    cocos2d::LayerColor *modalLayer;
    void createModalLayer();
    void addListenerToBackgroundLayer();
    
};

#endif
