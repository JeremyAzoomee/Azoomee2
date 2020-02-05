#ifndef __PAYMENT_SINGLETON_IOS_H__
#define __PAYMENT_SINGLETON_IOS_H__

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class ApplePaymentSingleton : public cocos2d::Ref
{
    
public:
    static ApplePaymentSingleton* getInstance(void);
    virtual ~ApplePaymentSingleton();
    bool init(void);
    
    void startIAPPayment();
    void refreshReceipt(bool usingButton);
    
    void transactionStatePurchased(const std::string& receiptData, const std::string& transactionID);
    void onAnswerReceived(const std::string& responseDataString);
    
private:
    //For making repeat requests
    std::string _savedReceipt;
    std::string _transactionID;
    int _requestAttempts;
};

NS_AZOOMEE_END

#endif
