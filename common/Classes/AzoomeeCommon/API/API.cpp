#include "API.h"
#include <cocos/cocos2d.h>
#include "../Data/ConfigStorage.h"
#include "../Utils/SessionIdManager.h"

using namespace cocos2d;


NS_AZOOMEE_BEGIN

#pragma mark - Constants

const char* const API::TagLogin = "parentLogin";
const char* const API::TagAnonymousDeviceLogin = "anonymousDeviceLogin";
const char* const API::TagUpdateBillingData = "updateBilling";
const char* const API::TagParentPin = "updateParentPin";
const char* const API::TagGetAvailableChildren = "getChildren";
const char* const API::TagChildLogin = "childLogin";
const char* const API::TagGetGorden = "getGordon";
const char* const API::TagRegisterParent = "registerParent";
const char* const API::TagRegisterChild = "registerChild";
const char* const API::TagUpdateChild = "updateChild";
const char* const API::TagDeleteChild = "deleteChild";
const char* const API::TagVerifyGooglePayment = "iabGooglePaymentMade";
const char* const API::TagVerifyAmazonPayment = "iapAmazonPaymentMade";
const char* const API::TagVerifyApplePayment = "iapApplePaymentMade";
const char* const API::TagGetChatList = "chat.getChatList";
const char* const API::TagGetChatMessages = "chat.getChatMessages";
const char* const API::TagSendChatMessage = "chat.sendChatMessage";
const char* const API::TagMarkReadMessage = "chat.markReadMessage";
const char* const API::TagResetPasswordRequest = "resetPasswordRequest";
const char* const API::TagOfflineCheck = "offlineCheck";
const char* const API::TagFriendRequest = "friendRequest";
const char* const API::TagFriendRequestReaction = "friendRequestReaction";
const char* const API::TagGetPendingFriendRequests = "getPendingFriendRequests";
const char* const API::TagReportChat = "chat.report";
const char* const API::TagResetReportedChat = "chat.resetReported";
const char* const API::TagGetForceUpdateInformation = "forceUpdate";
const char* const API::TagCookieRefresh = "cookieRefresh";
const char* const API::TagUpdateChildAvatar = "updateChildAvatar";

#pragma mark - API Methods

HttpRequestCreator* API::OfflineCheck(HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagOfflineCheck;
    
    request->url = "https://versions.azoomee.com";
    
#ifdef USINGCI
    request->url = "http://versions.azoomee.ninja";
#endif
    
    request->encrypted = false;
    return request;
}

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

HttpRequestCreator* API::AnonymousDeviceLoginRequest(const std::string &deviceId,
                                                     HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"deviceId\": \"%s\"}", deviceId.c_str());
    request->requestTag = TagAnonymousDeviceLogin;
    request->urlParameters = "hqs=true";
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

HttpRequestCreator* API::GetForceUpdateInformationRequest(Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagGetForceUpdateInformation;
    request->url = "https://versions.azoomee.com";
    
#ifdef USINGCI
    request->url = "http://versions.azoomee.ninja";
#endif
    
    request->encrypted = false;
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
    request->urlParameters = "hqs=true";
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

HttpRequestCreator* API::RefreshParentCookiesRequest(Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagCookieRefresh;
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::RegisterParentRequest(const std::string& emailAddress,
                                               const std::string& password,
                                               const std::string& pinNumber,
                                               const std::string& source,
                                               const std::string& sourceDevice,
                                               HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"emailAddress\":\"%s\",\"over18\":\"true\",\"termsAccepted\":\"true\",\"password\":\"%s\",\"source\":\"%s\",\"pinNumber\":\"%s\", \"sourceDevice\":\"%s\"}", emailAddress.c_str(), password.c_str(), source.c_str(), pinNumber.c_str(), sourceDevice.c_str());
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

HttpRequestCreator* API::DeleteChild(const std::string& childId,
                                     const std::string& childProfileName,
                                     const std::string& childGender,
                                     HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"profileName\":\"%s%s\",\"sex\":\"%s\",\"status\":\"DELETED\"}", childProfileName.c_str(),SessionIdManager::getInstance()->getCurrentSessionId().c_str(), childGender.c_str());
    request->requestTag = TagDeleteChild;
    request->url = ConfigStorage::getInstance()->getServerUrl() + ConfigStorage::getInstance()->getPathForTag(TagDeleteChild) + childId;
    request->method = "PATCH";
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::UpdateChildAvatar(const std::string &childId,
                                           const std::string &imageData,
                                           Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagUpdateChildAvatar;
    request->requestPath = StringUtils::format("/api/user/child/%s/avatar", childId.c_str());
    request->method = "PATCH";
    request->encrypted = true;
    
    request->requestBody = "{\"userId\":\"" + childId + "\", \"data\":\"" + imageData + "\"}";
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

HttpRequestCreator* API::ResetPaswordRequest(const std::string& forEmailAddress,
                                                  HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagResetPasswordRequest;
    request->requestPath = StringUtils::format("/api/auth/requestPasswordReset?emailAddress=%s", forEmailAddress.c_str());
    request->encrypted = false;
    return request;
}

#pragma mark - Sharing

HttpRequestCreator* API::GetChatListRequest(const std::string& userId,
                                            HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagGetChatList;
    request->requestPath = StringUtils::format("/api/share/%s/chat", userId.c_str());
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::GetChatMessagesRequest(const std::string& userId,
                                                const std::string& friendId,
                                                int pageNumber,
                                                HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagGetChatMessages;
    request->requestPath = StringUtils::format("/api/share/v2/%s/%s", userId.c_str(), friendId.c_str());
    request->urlParameters = StringUtils::format("page=%d", pageNumber);
    request->encrypted = true;
    return request;
}

HttpRequestCreator* API::SendChatMessageRequest(const std::string& userId,
                                                const std::string& friendId,
                                                const JsonObjectRepresentation& jsonObject,
                                                HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagSendChatMessage;
    request->requestPath = StringUtils::format("/api/share/v2/%s/%s", userId.c_str(), friendId.c_str());
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

HttpRequestCreator* API::MarkReadMessageRequest(const std::string& userId,
                                                const std::string& friendId,
                                                const uint64_t& readAt,
                                                HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagMarkReadMessage;
    request->requestPath = StringUtils::format("/api/share/v2/%s/%s", userId.c_str(), friendId.c_str());
    request->method = "PATCH";
    request->encrypted = true;
    request->requestBody = StringUtils::format("{\"readAt\": \"%lld\"}", readAt);
    return request;
}

HttpRequestCreator* API::SendChatReportRequest(const std::string &userId, const std::string &friendId, Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagReportChat;
    request->requestPath = StringUtils::format("/api/share/v2/%s/%s/conversationstatus", userId.c_str(), friendId.c_str());
    request->requestBody = "{\"status\": \"IN_MODERATION\"}";
    request->method = "PUT";
    request->encrypted = true;
    
    return request;
}

HttpRequestCreator* API::ResetReportedChatRequest(const std::string &userId, const std::string &friendId, Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestTag = TagResetReportedChat;
    request->requestPath = StringUtils::format("/api/share/v2/%s/%s/conversationstatus", userId.c_str(), friendId.c_str());
    request->requestBody = "{\"status\": \"ACTIVE\"}";
    request->method = "PATCH";
    request->encrypted = true;
    
    return request;
}

#pragma mark - Friend Requests

HttpRequestCreator* API::GetPendingFriendRequests(HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->urlParameters = "status=CREATED";
    request->requestTag = TagGetPendingFriendRequests;
    request->encrypted = true;
    
    return request;
}

HttpRequestCreator* API::FriendRequest(const std::string& senderChildId, const std::string& senderChildName, const std::string& inviteCode, HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestPath = StringUtils::format("/api/user/child/%s/invite/code", senderChildId.c_str());
    request->requestBody = StringUtils::format("{\"inviteeCode\": \"%s\", \"friendName\": \"\", \"senderName\": \"%s\"}", inviteCode.c_str(), senderChildName.c_str());
    request->method = "POST";
    request->requestTag = TagFriendRequest;
    request->encrypted = true;
    
    return request;
}

HttpRequestCreator* API::FriendRequestReaction(bool confirmed, const std::string& respondentChildId, const std::string& invitationId, const std::string& senderName, HttpRequestCreatorResponseDelegate* delegate)
{
    std::string status = "APPROVED";
    if(!confirmed) status = "REJECTED";
    
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestPath = StringUtils::format("/api/user/child/%s/invite/code/%s", respondentChildId.c_str(), invitationId.c_str());
    request->requestBody = StringUtils::format("{\"status\": \"%s\", \"friendName\": \"%s\"}", status.c_str(), senderName.c_str());
    request->method = "POST";
    request->requestTag = TagFriendRequestReaction;
    request->encrypted = true;
    return request;
}


NS_AZOOMEE_END
