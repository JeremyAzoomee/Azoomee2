#ifndef __AMAZON_PAYMENT_SINGLETON_H__
#define __AMAZON_PAYMENT_SINGLETON_H__

#include <cocos/cocos2d.h>
#include <TinizineCommon/Tinizine.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class AmazonPaymentSingleton : public cocos2d::Ref
{
    
public:
    static AmazonPaymentSingleton* getInstance(void);
    virtual ~AmazonPaymentSingleton();
    bool init(void);
    
    void onAmazonPaymentMadeAnswerReceived(std::string responseDataString);
    void amazonPaymentMade(std::string requestId, std::string receiptId, std::string amazonUserid);
    void startIAPPayment();
    
private:
    void fulfillAmazonPayment(std::string receiptId);
    
    //For making repeat requests
    std::string savedRequestId;
    std::string savedReceiptId;
    std::string savedAmazonUserid;
    
    int requestAttempts;
    
};

NS_AZ_END

#endif
