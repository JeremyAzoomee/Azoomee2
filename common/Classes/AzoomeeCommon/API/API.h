#ifndef AzoomeeCommon_API_h
#define AzoomeeCommon_API_h

#include "../Azoomee.h"
#include <string>
#include "../Data/Json.h"
#include "HttpRequestCreator.h"


NS_AZOOMEE_BEGIN


/**
 * This class contains purely the logic for building API requests to Azoomee.
 * It is stateless and has no knowledge or state of the app.
 */
class API
{
public:

#pragma mark - Constants
    
    static const char* const TagLogin;
    static const char* const TagUpdateBillingData;
    static const char* const TagParentPin;
    static const char* const TagGetAvailableChildren;
    static const char* const TagChildLogin;
    static const char* const TagGetGorden;
    static const char* const TagRegisterParent;
    static const char* const TagRegisterChild;
    static const char* const TagUpdateChild;
    static const char* const TagVerifyGooglePayment;
    static const char* const TagVerifyAmazonPayment;
    static const char* const TagVerifyApplePayment;
    static const char* const TagGetEncryptedContent;
    static const char* const TagGetPublicContent;
    static const char* const TagGetChatList;
    static const char* const TagGetChatMessages;
    static const char* const TagSendChatMessage;
    static const char* const TagResetPasswordRequest;
    static const char* const TagOfflineCheck;
    
#pragma mark - API Methods
    
    static HttpRequestCreator* OfflineCheck(HttpRequestCreatorResponseDelegate* delegate);
    
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
    
    static HttpRequestCreator* UpdateChildRequest(const std::string& url,
                                                    const std::string& childId,
                                                    const std::string& childProfileName,
                                                    const std::string& childGender,
                                                    const std::string& childDOB,
                                                    const std::string& avatar,
                                                    const std::string& ownerId,
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
    
    static HttpRequestCreator* GetElectricDreamsContent(const std::string& requestId, 
                                                        const std::string& childId,
                                                        const std::string& contentID,
                                                        HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* ResetPaswordRequest(const std::string& forEmailAddress,
                                                   HttpRequestCreatorResponseDelegate* delegate);
    
#pragma mark - Sharing
    
    // Get the chat list for childId
    // childId must be the currently logged in child, or the request will fail
    static HttpRequestCreator* GetChatListRequest(const std::string& childId,
                                                  HttpRequestCreatorResponseDelegate* delegate);
    
    // Get the chat list between childId and friendId
    // childId must be the currently logged in child, or the request will fail
    static HttpRequestCreator* GetChatMessagesRequest(const std::string& childId,
                                                      const std::string& friendId,
                                                      HttpRequestCreatorResponseDelegate* delegate);
    
    // Send a chat message to friendId
    // childId must be the currently logged in child, or the request will fail
    // params must be a Json object of type Message
    static HttpRequestCreator* SendChatMessageRequest(const std::string& childId,
                                                      const std::string& friendId,
                                                      const JsonObjectRepresentation& jsonObject,
                                                      HttpRequestCreatorResponseDelegate* delegate);
    
    
    
};
  
NS_AZOOMEE_END

#endif
