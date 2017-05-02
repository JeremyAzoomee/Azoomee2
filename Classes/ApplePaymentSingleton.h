#ifndef __PAYMENT_SINGLETON_IOS_H__
#define __PAYMENT_SINGLETON_IOS_H__

#include "cocos2d.h"
#include <AzoomeeCommon/UI/MessageBox.h>

using namespace Azoomee;


class ApplePaymentSingleton : public cocos2d::Ref, public MessageBoxDelegate
{
    
public:
    static ApplePaymentSingleton* getInstance(void);
    virtual ~ApplePaymentSingleton();
    bool init(void);
    
    void startIAPPayment();
    void refreshReceipt(bool usingButton);
    
    void transactionStatePurchased(std::string receiptData);
    void onAnswerReceived(std::string responseDataString);
    
    //Delegate Functions
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
private:
    //For making repeat requests
    std::string savedReceipt;
    int requestAttempts;
};

#endif