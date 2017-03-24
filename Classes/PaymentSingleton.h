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
    void startIAPPayment();
    
    void purchaseFailed();
    void backendRequestFailed();
    
    bool OS_is_IAP_Compatible();
    bool showIAPContent();
    
    void removeModalLayer();
    bool iapAttemptInprogress;
    
private:
    void fulfillAmazonPayment(std::string receiptId);
    
    bool isOS_IAP_Compatible;
    void setupisOS_IAP_Compatible();
    
    cocos2d::LayerColor *modalLayer;
    void createModalLayer();
    void addListenerToBackgroundLayer();
    
    //For making repeat requests
    std::string savedRequestId;
    std::string savedReceiptId;
    std::string savedAmazonUserid;
    
    int requestAttempts;
    
};

#endif
