#ifndef __GOOGLE_PAYMENT_SINGLETON_H__
#define __GOOGLE_PAYMENT_SINGLETON_H__

#include "cocos2d.h"
#include <AzoomeeCommon/Tinizine.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class GooglePaymentSingleton : public cocos2d::Ref
{
    
public:
    static GooglePaymentSingleton* getInstance(void);
    virtual ~GooglePaymentSingleton();
    bool init(void);
    
    void startIABPayment();
	void startRestorePurchase();
    void startBackEndPaymentVerification(std::string developerPayload, std::string orderId, std::string token);
    void purchaseFailedBeforeFulfillment();
    void purchaseFailedAlreadyPurchased();
	void restoreFailedNoPurchase();
    void onGooglePaymentVerificationAnswerReceived(std::string responseDataString);
    
    std::string getDeveloperPublicKey();
    
private:
    
    std::string savedDeveloperPayload;
    std::string savedOrderId;
    std::string savedToken;
    
    bool iabAttemptInProgress = false;
    int requestAttempts;
};

NS_AZ_END

#endif
