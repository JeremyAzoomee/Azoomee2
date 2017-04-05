#ifndef __PAYMENT_SINGLETON_IOS_H__
#define __PAYMENT_SINGLETON_IOS_H__

#include "cocos2d.h"

class ApplePaymentSingleton : public cocos2d::Ref
{
    
public:
    static ApplePaymentSingleton* getInstance(void);
    virtual ~ApplePaymentSingleton();
    bool init(void);
    
    void makeMonthlyPayment();
    void restorePayment();
    
    void transactionStatePurchased(std::string receiptData);
    void onAnswerReceived(std::string responseDataString);
    
    void transactionStateFailed();
    
    void transactionStateRestored();
    
    void ErrorMessage();
    void DoublePurchase();
    
    void backendRequestFailed(long errorCode);
    
private:
    //For making repeat requests
    std::string savedReceipt;
    
    int requestAttempts;
};

#endif
