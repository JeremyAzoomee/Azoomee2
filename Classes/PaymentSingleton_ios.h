#ifndef __PAYMENT_SINGLETON_IOS_H__
#define __PAYMENT_SINGLETON_IOS_H__

#include "cocos2d.h"

class PaymentSingleton_ios : public cocos2d::Ref
{
    
public:
    static PaymentSingleton_ios* getInstance(void);
    virtual ~PaymentSingleton_ios();
    bool init(void);
    
    void makeMonthlyPayment();
    void restorePayment();
    
    void transactionStatePurchased(std::string receiptData);
    void onAnswerReceived(std::string responseDataString);
    
    void transactionStateFailed();
    
    void transactionStateRestored();
    
    void ErrorMessage();
    void DoublePurchase();
    
private:
    //For making repeat requests
    std::string savedReceipt;
    
    int requestAttempts;
};

#endif
