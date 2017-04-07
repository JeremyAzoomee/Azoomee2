#ifndef __GOOGLE_PAYMENT_SINGLETON_H__
#define __GOOGLE_PAYMENT_SINGLETON_H__

#include "cocos2d.h"

class GooglePaymentSingleton : public cocos2d::Ref
{
    
public:
    static GooglePaymentSingleton* getInstance(void);
    virtual ~GooglePaymentSingleton();
    bool init(void);
    
    void startIABPayment();
    void finishedIABPayment(std::string developerPayload, std::string orderId, std::string token);
    void purchaseFailed();
    void showDoublePurchase();
    void onGooglePaymentMadeAnswerReceived(std::string responseDataString);
    void backendRequestFailed();
    
private:
    bool paymentInProgress;
    void createModalLayer();
    void removeModalLayer();
    cocos2d::LayerColor* modalLayer;
    void addListenerToBackgroundLayer();
    
    std::string savedDeveloperPayload;
    std::string savedOrderId;
    std::string savedToken;
    
    bool iabAttemptInProgress;
    int requestAttempts;
};

#endif
