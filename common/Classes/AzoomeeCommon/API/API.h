#ifndef AzoomeeCommon_API_h
#define AzoomeeCommon_API_h

#include "../Azoomee.h"
#include <string>
#include "HttpRequestCreator.h"


NS_AZOOMEE_BEGIN

class API
{
public:
    static HttpRequestCreator* LoginRequest(const std::string& username,
                                            const std::string& password,
                                            HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* UpdateBillingDataRequest(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* UpdateParentPinRequest(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetAvailableChildrenRequest(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* ChildLoginRequest(const std::string& profileName,
                                                 HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetGordenRequest(const std::string& userId,
                                                const std::string& sessionId,
                                                HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* RegisterParentRequest(const std::string& emailAddress,
                                                     const std::string& password,
                                                     const std::string& pinNumber,
                                                     const std::string& source,
                                                     HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* RegisterChildRequest(const std::string& childProfileName,
                                                    const std::string& childGender,
                                                    const std::string& childDOB,
                                                    const std::string& avatar,
                                                    HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* VerifyGooglePaymentRequest(const std::string& orderId,
                                                          const std::string& iapSku,
                                                          const std::string& purchaseToken,
                                                          HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* VerifyAmazonPaymentRequest(const std::string& requestId,
                                                          const std::string& receiptId,
                                                          const std::string& amazonUserid,
                                                          HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* VerifyApplePaymentRequest(const std::string& receiptData,
                                                         HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetEncryptedContentRequest(const std::string& url,
                                                          const std::string& category,
                                                          HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetPublicContentRequest(const std::string& url,
                                                       const std::string& category,
                                                       HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetElectricDreamsContent(const std::string& requestId, const std::string& childId,
                                                        const std::string& contentID,
                                                        HttpRequestCreatorResponseDelegate* delegate);
    
    
    
};
  
NS_AZOOMEE_END

#endif
