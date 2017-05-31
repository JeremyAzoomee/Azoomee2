#include "API.h"
#include <cocos/cocos2d.h>

using namespace cocos2d;


NS_AZOOMEE_BEGIN

#pragma mark - Constants

const char* const API::TagLogin = "parentLogin";
const char* const API::TagUpdateBillingData = "updateBilling";
const char* const API::TagParentPin = "updateParentPin";
const char* const API::TagGetAvailableChildren = "getChildren";
const char* const API::TagChildLogin = "childLogin";
const char* const API::TagGetGorden = "getGordon";
const char* const API::TagRegisterParent = "registerParent";
const char* const API::TagRegisterChild = "registerChild";
const char* const API::TagUpdateChild = "updateChild";
const char* const API::TagVerifyGooglePayment = "iabGooglePaymentMade";
const char* const API::TagVerifyAmazonPayment = "iapAmazonPaymentMade";
const char* const API::TagVerifyApplePayment = "iapApplePaymentMade";
const char* const API::TagGetChatList = "chat.getChatList";
const char* const API::TagGetChatMessages = "chat.getChatMessages";
const char* const API::TagSendChatMessage = "chat.sendChatMessage";

#pragma mark - API Methods

HttpRequestCreator* API::LoginRequest(const std::string& username,
                                      const std::string& password,
                                      HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"password\": \"%s\",\"userName\": \"%s\",\"appType\": \"CHILD_APP\"}", password.c_str(), username.c_str());
    request->requestTag = TagLogin;
    request->method = "POST";
    return request;
}

HttpRequestCreator* API::UpdateBillingDataRequest(HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagUpdateBillingData;
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::UpdateParentPinRequest(HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagParentPin;
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::GetAvailableChildrenRequest(HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->urlParameters = "expand=true";
    request->requestTag = TagGetAvailableChildren;
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::ChildLoginRequest(const std::string& profileName,
                                           HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"userName\": \"%s\", \"password\": \"\"}", profileName.c_str());
    request->requestTag = TagChildLogin;
    request->method = "POST";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::GetGordenRequest(const std::string& userId,
                                          const std::string& sessionId,
                                          HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->urlParameters = StringUtils::format("userid=%s&sessionid=%s", userId.c_str(), sessionId.c_str());
    request->requestTag = TagGetGorden;
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::RegisterParentRequest(const std::string& emailAddress,
                                               const std::string& password,
                                               const std::string& pinNumber,
                                               const std::string& source,
                                               HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"emailAddress\":\"%s\",\"over18\":\"true\",\"termsAccepted\":\"true\",\"password\":\"%s\",\"source\":\"%s\",\"pinNumber\":\"%s\"}", emailAddress.c_str(), password.c_str(), source.c_str(), pinNumber.c_str());
    request->requestTag = TagRegisterParent;
    request->method = "POST";
    return request;
}

HttpRequestCreator* API::RegisterChildRequest(const std::string& childProfileName,
                                              const std::string& childGender,
                                              const std::string& childDOB,
                                              const std::string& avatar,
                                              HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"profileName\":\"%s\",\"dob\":\"%s\",\"sex\":\"%s\",\"avatar\":\"%s\",\"password\":\"\"}", childProfileName.c_str(), childDOB.c_str(), childGender.c_str(), avatar.c_str());
    request->requestTag = TagRegisterChild;
    request->method = "POST";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::UpdateChildRequest(const std::string& url,
                                              const std::string& childId,
                                              const std::string& childProfileName,
                                              const std::string& childGender,
                                              const std::string& childDOB,
                                              const std::string& avatar,
                                              const std::string& ownerId,
                                              HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"id\":\"%s\",\"profileName\":\"%s\",\"dob\":\"%s\",\"sex\":\"%s\",\"avatar\":\"%s\",\"ownerId\":\"%s\"}", childId.c_str(), childProfileName.c_str(), childDOB.c_str(), childGender.c_str(), avatar.c_str(), ownerId.c_str());
    request->requestTag = TagUpdateChild;
    request->url = 	url;
    request->method = "PATCH";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::VerifyGooglePaymentRequest(const std::string& orderId,
                                                    const std::string& iapSku,
                                                    const std::string& purchaseToken,
                                                    HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"orderId\": \"%s\", \"subscriptionId\": \"%s\", \"purchaseToken\": \"%s\"}", orderId.c_str(), iapSku.c_str(), purchaseToken.c_str());
    request->requestTag = TagVerifyGooglePayment;
    request->method = "POST";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::VerifyAmazonPaymentRequest(const std::string& requestId,
                                                    const std::string& receiptId,
                                                    const std::string& amazonUserid,
                                                    HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"requestId\": \"%s\", \"receiptId\": \"%s\", \"amazonUserId\": \"%s\"}", requestId.c_str(), receiptId.c_str(), amazonUserid.c_str());
    request->requestTag = TagVerifyAmazonPayment;
    request->method = "POST";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::VerifyApplePaymentRequest(const std::string& receiptData,
                                                   HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"receipt-data\": \"%s\"}", receiptData.c_str());
    request->requestTag = TagVerifyApplePayment;
    request->method = "POST";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::GetEncryptedContentRequest(const std::string& url,
                                                    const std::string& category,
                                                    HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->url = url;
    request->requestTag = category;
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::GetPublicContentRequest(const std::string& url,
                                                 const std::string& category,
                                                 HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->url = url;
    request->requestTag = category;
    return request;
}

HttpRequestCreator* API::GetElectricDreamsContent(const std::string& requestId, 
                                                  const std::string& childId,
                                                  const std::string& contentID,
                                                  HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = requestId;
    request->requestPath = StringUtils::format("/api/electricdreams/%s/content/%s", childId.c_str(), contentID.c_str());
    request->encrypted = true;
    return request;
}

#pragma mark - Sharing

HttpRequestCreator* API::GetChatListRequest(const std::string& childId,
                                            HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagGetChatList;
    request->requestPath = StringUtils::format("/api/share/%s/chat", childId.c_str());
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::GetChatMessagesRequest(const std::string& childId,
                                                const std::string& friendId,
                                                HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagGetChatMessages;
    request->requestPath = StringUtils::format("/api/share/%s/%s", childId.c_str(), friendId.c_str());
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::SendChatMessageRequest(const std::string& childId,
                                                const std::string& friendId,
                                                const JsonObjectRepresentation& jsonObject,
                                                HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagSendChatMessage;
    request->requestPath = StringUtils::format("/api/share/%s/%s", childId.c_str(), friendId.c_str());
    request->method = "POST";
    request->encrypted = true;
    
    // Create body from the json
    const auto& json = jsonObject.toJson();
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json.Accept(writer);
    request->requestBody = buffer.GetString();
    
    return request;
}

NS_AZOOMEE_END
