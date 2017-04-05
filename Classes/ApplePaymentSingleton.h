#ifndef __PAYMENT_SINGLETON_IOS_H__
#define __PAYMENT_SINGLETON_IOS_H__

#include "cocos2d.h"
#include "MessageBox.h"

class ApplePaymentSingleton : public cocos2d::Ref, public MessageBoxDelegate
{
    
public:
    static ApplePaymentSingleton* getInstance(void);
    virtual ~ApplePaymentSingleton();
    bool init(void);
    
    void makeMonthlyPayment();
    void refreshReceipt(bool usingButton);
    
    void transactionStatePurchased(std::string receiptData);
    void onAnswerReceived(std::string responseDataString);
    
    void ErrorMessage();
    void DoublePurchase();
    
    void backendRequestFailed(long errorCode);
    
    //Delegate Functions
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
private:
    //For making repeat requests
    std::string savedReceipt;
    
    int requestAttempts;
    
    bool refreshFromButton;
    
};

#endif
