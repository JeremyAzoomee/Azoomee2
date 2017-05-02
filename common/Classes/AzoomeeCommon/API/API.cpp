#include "API.h"
#include <cocos/cocos2d.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

HttpRequestCreator* API::LoginRequest(const std::string& username,
                                      const std::string& password,
                                      HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"password\": \"%s\",\"userName\": \"%s\",\"appType\": \"CHILD_APP\"}", password.c_str(), username.c_str());
    request->requestTag = "parentLogin";
    request->method = "POST";
    return request;
}

HttpRequestCreator* API::UpdateBillingDataRequest(HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = "updateBilling";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::UpdateParentPinRequest(HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = "updateParentPin";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::GetAvailableChildrenRequest(HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->urlParameters = "expand=true";
    request->requestTag = "getChildren";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::ChildLoginRequest(const std::string& profileName,
                                           HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"userName\": \"%s\", \"password\": \"\"}", profileName.c_str());
    request->requestTag = "childLogin";
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
    request->requestTag = "getGordon";
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
    request->requestTag = "registerParent";
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
    request->requestTag = "registerChild";
    request->method = "POST";
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
    request->requestTag = "iabGooglePaymentMade";
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
    request->requestTag = "iapAmazonPaymentMade";
    request->method = "POST";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::VerifyApplePaymentRequest(const std::string& receiptData,
                                                   HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"receipt-data\": \"%s\"}", receiptData.c_str());
    request->requestTag = "iapApplePaymentMade";
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

NS_AZOOMEE_END