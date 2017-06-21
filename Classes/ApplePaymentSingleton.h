#ifndef __PAYMENT_SINGLETON_IOS_H__
#define __PAYMENT_SINGLETON_IOS_H__

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/MessageBox.h>

NS_AZOOMEE_BEGIN

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

NS_AZOOMEE_END

#endif
