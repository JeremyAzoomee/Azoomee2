#ifndef __AMAZON_PAYMENT_SINGLETON_H__
#define __AMAZON_PAYMENT_SINGLETON_H__

#include <cocos/cocos2d.h>

class AmazonPaymentSingleton : public cocos2d::Ref
{
    
public:
    static AmazonPaymentSingleton* getInstance(void);
    virtual ~AmazonPaymentSingleton();
    bool init(void);
    
    void onAmazonPaymentMadeAnswerReceived(std::string responseDataString);
    void amazonPaymentMade(std::string requestId, std::string receiptId, std::string amazonUserid);
    void startIAPPayment();
    
    void purchaseFailed();
    void backendRequestFailed();
    
    bool showIAPContent();
    
    void removeModalLayer();
    
private:
    void fulfillAmazonPayment(std::string receiptId);
    
    cocos2d::LayerColor *modalLayer;
    void createModalLayer();
    void addListenerToBackgroundLayer();
    
    //For making repeat requests
    std::string savedRequestId;
    std::string savedReceiptId;
    std::string savedAmazonUserid;
    
    int requestAttempts;
    bool iapAttemptInprogress;
    
};

#endif
