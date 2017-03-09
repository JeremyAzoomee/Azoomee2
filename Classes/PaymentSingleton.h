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
    
private:
    void fulfillAmazonPayment(std::string receiptId);
    
};

#endif
