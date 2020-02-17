#include "API.h"
#include <cocos/cocos2d.h>
#include <functional>
#include "../Analytics/AnalyticsSingleton.h"
#include "../Utils/StringFunctions.h"
#include "../Data/AppConfig.h"

using namespace cocos2d;


NS_AZOOMEE_BEGIN

#pragma mark - Constants

const char* const API::TagIpCheck = "ipCheck";
const char* const API::TagLogin = "parentLogin";
const char* const API::TagGetAnonCredentials = "getAnonCredentials";
const char* const API::TagUpdateBillingData = "updateBilling";
const char* const API::TagParentPin = "updateParentPin";
const char* const API::TagGetAvailableChildren = "getChildren";
const char* const API::TagChildLogin = "childLogin";
const char* const API::TagGetSessionCookies = "getSessionCookies";
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
const char* const API::TagGetTimelineSummary = "chat.getTimelineSummary";
const char* const API::TagGetForceUpdateInformation = "forceUpdate";
const char* const API::TagCookieRefresh = "cookieRefresh";
const char* const API::TagChildCookieRefresh = "childCookieRefresh";
const char* const API::TagGetContentPoolRequest = "getContentPool";
const char* const API::TagGetHqStructureDataRequest = "getHQStructureData";
const char* const API::TagUpdateChildAvatar = "updateChildAvatar";
const char* const API::TagUpdateParentDetails = "updateParentDetails";
const char* const API::TagUpdateParentPassword = "updateParentPassword";
const char* const API::TagGetParentDetails = "getParentDetails";
const char* const API::TagUpdateChildNameRequest = "updateChildNameRequest";
const char* const API::TagAddVoucher = "addVoucher";
const char* const API::TagGetVodacomTransactionId = "vodacomTransactionId";
const char* const API::TagGetVideoProgress = "getVideoProgress";
const char* const API::TagUpdateProgressMeta = "updateProgressMeta";
const char* const API::TagRewardCallback = "rewardCallback";
const char* const API::TagRedeemReward = "redeemReward";
const char* const API::TagGetPendingRewards = "getPendingRewards";
const char* const API::TagGetInventory = "getInventory";
const char* const API::TagBuyReward = "buyReward";
const char* const API::TagGetRewardStrategy = "getRewardStrategy";
const char* const API::TagGetShopFeed = "getShopFeed";
const char* const API::TagGetOomeeMakerAssets = "getOomeeMakerAssets";
const char* const API::TagGetMarketingAssets = "getMarketingAssets";
const char* const API::TagSaveNewOomee = "saveNewOomee";
const char* const API::TagSaveLocalOomee = "saveLocalOomee";
const char* const API::TagGetChildOomees = "getChildOomees";
const char* const API::TagUpdateChildOomee = "updateChildOomee";
const char* const API::TagGetAllOomees = "getAllOomees";
const char* const API::TagDeleteChildOomee = "deleteChildOomee";

const std::string API::kAZCountryCodeKey = "X-AZ-COUNTRYCODE";

const std::string API::kRewardHeaderCheckEventName = "rewardHeaderCheckEvent";

void API::HandleAPIResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response, HttpRequestCreatorResponseDelegate* delegate, HttpRequestCreator* request)
{
    HandleAPIResponse(sender, response, request,
                          delegate != nullptr ? std::bind(&HttpRequestCreatorResponseDelegate::onHttpRequestSuccess, delegate, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) : APIResponseSuccessCallback(),
                          delegate != nullptr ? std::bind(&HttpRequestCreatorResponseDelegate::onHttpRequestFailed, delegate, std::placeholders::_1, std::placeholders::_2) : APIResponseFailureCallback());
}

void API::HandleAPIResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response, HttpRequestCreator* request, const APIResponseSuccessCallback& onSuccess, const APIResponseFailureCallback& onFailure)
{
	std::string responseHeaderString  = std::string(response->getResponseHeader()->begin(), response->getResponseHeader()->end());
	std::string responseDataString = std::string(response->getResponseData()->begin(), response->getResponseData()->end());
	std::string requestTag = response->getHttpRequest()->getTag();
	
	cocos2d::log("HandleAPIResponse: %s (%ld)", requestTag.c_str(), response->getResponseCode());
//	cocos2d::log("Response header: %s", responseHeaderString.c_str());
//	cocos2d::log("Response string: %s", responseDataString.c_str());
	
    const long responseCode = response->getResponseCode();
	if((responseCode == 200) || (responseCode == 201) || (responseCode == 204))
	{
        if(onSuccess != nullptr)
		{
			onSuccess(requestTag, responseHeaderString, responseDataString);
		}
	}
	else
	{
		HandleAPIError(response, request, onFailure);
	}
}

void API::HandleAPIError(cocos2d::network::HttpResponse *response, HttpRequestCreator* request, const APIResponseFailureCallback& onFailure)
{
	const std::string& responseHeaderString  = std::string(response->getResponseHeader()->begin(), response->getResponseHeader()->end());
	const std::string& responseDataString = std::string(response->getResponseData()->begin(), response->getResponseData()->end());
	const std::string& requestTag = response->getHttpRequest()->getTag();
	long errorCode = response->getResponseCode();
	
    cocos2d::log("HandleAPIError: %s (%ld) %s %s", requestTag.c_str(), response->getResponseCode(), request->_method.c_str(), request->getRequestURL().c_str());
	cocos2d::log("HandleAPIError response: %s", responseDataString.c_str());
    if(!request->_requestBody.empty())
    {
        cocos2d::log("HandleAPIError request body: %s", request->_requestBody.c_str());
    }
	
	//-----------------------Handle error code--------------------------
	
	if(request->getAmountOfFails() < 2)
	{
		request->resendRequest();
		return;
	}
	
	if(response->getResponseCode() != -1)
	{
		AnalyticsSingleton::getInstance()->httpRequestFailed(requestTag, errorCode, StringFunctions::getValueFromHttpResponseHeaderForKey("X-AZ-QID", responseHeaderString));
	}
	
	if((errorCode == 401) && (StringFunctions::findPositionOfNthString(responseDataString, "Invalid Request Time", 1) != responseDataString.length()))
	{
		errorCode = 2001;
	}

	if(onFailure != nullptr)
	{
		onFailure(requestTag, errorCode);
	}

}

#pragma mark - API Methods

HttpRequestCreator* API::IpCheck(HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagIpCheck;
    request->_url = "https://icanhazip.azoomee.com";
#ifdef AZOOMEE_ENVIRONMENT_CI
    request->_url = "https://icanhazip.azoomee.ninja";
#endif
    request->_encrypted = false;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::OfflineCheck(HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagOfflineCheck;
    
    request->_url = "https://versions.azoomee.com";
    
#ifdef AZOOMEE_ENVIRONMENT_CI
    request->_url = "http://versions.azoomee.ninja";
#endif
    
    request->_encrypted = false;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::LoginRequest(const std::string& username,
                                      const std::string& password,
                                      HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = "/api/auth/login";
    request->_requestBody = StringUtils::format("{\"password\": \"%s\",\"userName\": \"%s\",\"appType\": \"CHILD_APP\"}", password.c_str(), username.c_str());
    request->_requestTag = TagLogin;
    request->_method = "POST";
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::LoginRequest(const std::string& username,
                                      const std::string& password,
                                      const APIResponseSuccessCallback& onSuccess,
                                      const APIResponseFailureCallback& onFailure)
{
    HttpRequestCreator* request = new HttpRequestCreator(nullptr);
    request->_requestPath = "/api/auth/login";
    request->_requestBody = StringUtils::format("{\"password\": \"%s\",\"userName\": \"%s\",\"appType\": \"CHILD_APP\"}", password.c_str(), username.c_str());
    request->_requestTag = TagLogin;
    request->_method = "POST";
    request->setRequestCallback([onSuccess, onFailure, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, request, onSuccess, onFailure);
    });
    return request;
}

HttpRequestCreator* API::GetAnonCredentials(HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestTag = TagGetAnonCredentials;
	request->_requestPath = "/api/user/anonymous/signup";
	request->_requestBody = "{}";
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

HttpRequestCreator* API::GetAnonCredentials(const APIResponseSuccessCallback& onSuccess,
                                            const APIResponseFailureCallback& onFailure)
{
    HttpRequestCreator* request = new HttpRequestCreator(nullptr);
    request->_requestTag = TagGetAnonCredentials;
    request->_requestPath = "/api/user/anonymous/signup";
    request->_requestBody = "{}";
    request->setRequestCallback([onSuccess, onFailure, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, request, onSuccess, onFailure);
    });
    return request;
}

HttpRequestCreator* API::UpdateBillingDataRequest(const std::string& parentId,
												  HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagUpdateBillingData;
	request->_requestPath = StringUtils::format("/api/billing/user/%s/billingStatus", parentId.c_str());
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::UpdateBillingDataRequest(const std::string& parentId,
                                                  const APIResponseSuccessCallback& onSuccess,
                                                  const APIResponseFailureCallback& onFailure)
{
    HttpRequestCreator* request = new HttpRequestCreator(nullptr);
    request->_requestTag = TagUpdateBillingData;
    request->_requestPath = StringUtils::format("/api/billing/user/%s/billingStatus", parentId.c_str());
    request->_encrypted = true;
    request->_signAsParent = true;
    request->setRequestCallback([onSuccess, onFailure, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, request, onSuccess, onFailure);
    });
    return request;
}

HttpRequestCreator* API::GetForceUpdateInformationRequest(Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagGetForceUpdateInformation;
    request->_url = "https://versions.azoomee.com";
    
#ifdef AZOOMEE_ENVIRONMENT_CI
    request->_url = "http://versions.azoomee.ninja";
#endif
    
    request->_encrypted = false;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::UpdateParentPinRequest(const std::string& parentId,
                                                HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = StringUtils::format("/api/user/adult/%s", parentId.c_str());
    request->_requestTag = TagParentPin;
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::GetAvailableChildrenRequest(const std::string& parentId,
                                                     HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = StringUtils::format("/api/user/adult/%s/owns", parentId.c_str());
    request->_urlParameters = "expand=true";
    request->_requestTag = TagGetAvailableChildren;
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::GetAvailableChildrenRequest(const std::string& parentId,
                                                     const APIResponseSuccessCallback& onSuccess,
                                                     const APIResponseFailureCallback& onFailure)
{
    HttpRequestCreator* request = new HttpRequestCreator(nullptr);
    request->_requestPath = StringUtils::format("/api/user/adult/%s/owns", parentId.c_str());
    request->_urlParameters = "expand=true";
    request->_requestTag = TagGetAvailableChildren;
    request->_encrypted = true;
    request->_signAsParent = true;
    request->setRequestCallback([onSuccess, onFailure, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, request, onSuccess, onFailure);
    });
    return request;
}

HttpRequestCreator* API::ChildLoginRequest(const std::string& profileName,
                                           HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = "/api/auth/switchProfile";
    request->_requestBody = StringUtils::format("{\"userName\": \"%s\", \"password\": \"\"}", profileName.c_str());
    request->_requestTag = TagChildLogin;
    request->_method = "POST";
    request->_urlParameters = "hqs=true";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::ChildLoginRequest(const std::string& profileName,
                                           const APIResponseSuccessCallback& onSuccess,
                                           const APIResponseFailureCallback& onFailure)
{
    HttpRequestCreator* request = new HttpRequestCreator(nullptr);
    request->_requestPath = "/api/auth/switchProfile";
    request->_requestBody = StringUtils::format("{\"userName\": \"%s\", \"password\": \"\"}", profileName.c_str());
    request->_requestTag = TagChildLogin;
    request->_method = "POST";
    request->_urlParameters = "hqs=true";
    request->_encrypted = true;
    request->_signAsParent = true;
    request->setRequestCallback([onSuccess, onFailure, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, request, onSuccess, onFailure);
    });
    return request;
}

HttpRequestCreator* API::GetSessionCookiesRequest(const std::string& userId,
                                                  const std::string& sessionId,
                                                  bool parentSigned,
                                                  HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = "/api/porthole/pixel/gordon.png";
    request->_urlParameters = StringUtils::format("userid=%s&sessionid=%s", userId.c_str(), sessionId.c_str());
    request->_requestTag = TagGetSessionCookies;
    request->_encrypted = true;
    request->_signAsParent = parentSigned;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::GetSessionCookiesRequest(const std::string& userId,
                                                  const std::string& sessionId,
                                                  bool parentSigned,
                                                  const APIResponseSuccessCallback& onSuccess,
                                                  const APIResponseFailureCallback& onFailure)
{
    HttpRequestCreator* request = new HttpRequestCreator(nullptr);
    request->_requestPath = "/api/porthole/pixel/gordon.png";
    request->_urlParameters = StringUtils::format("userid=%s&sessionid=%s", userId.c_str(), sessionId.c_str());
    request->_requestTag = TagGetSessionCookies;
    request->_encrypted = true;
    request->_signAsParent = parentSigned;
    request->setRequestCallback([onSuccess, onFailure, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, request, onSuccess, onFailure);
    });
    return request;
}

HttpRequestCreator* API::RefreshParentCookiesRequest(Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = "/api/cookie/refresh/adult";
    request->_requestTag = TagCookieRefresh;
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::RefreshChildCookiesRequest(Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestTag = TagChildCookieRefresh;
	request->_requestPath = "/api/cookie/refresh/child";
	request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

HttpRequestCreator* API::RegisterParentRequest(const std::string& parentId,
											   const std::string& emailAddress,
                                               const std::string& password,
                                               const std::string& pinNumber,
                                               const std::string& source,
                                               const std::string& sourceDevice,
                                               const std::string& marketingAccepted,
                                               HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestBody = StringUtils::format("{\"emailAddress\":\"%s\",\"over18\":\"true\",\"termsAccepted\":\"true\",\"marketingAccepted\":\"%s\",\"password\":\"%s\",\"source\":\"%s\",\"pinNumber\":\"%s\", \"sourceDevice\":\"%s\"}", emailAddress.c_str(), marketingAccepted.c_str(), password.c_str(), source.c_str(), pinNumber.c_str(), sourceDevice.c_str());
	request->_requestPath = StringUtils::format("/api/user/anonymous/adult/%s",parentId.c_str());
    request->_requestTag = TagRegisterParent;
    request->_method = "PATCH";
	request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::RegisterChildRequest(const std::string& childProfileName,
                                              const std::string& childGender,
                                              const std::string& childDOB,
                                              const std::string& avatar,
                                              HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = "/api/user/child";
    request->_requestBody = StringUtils::format("{\"profileName\":\"%s\",\"dob\":\"%s\",\"sex\":\"%s\",\"avatar\":\"%s\",\"password\":\"\"}", childProfileName.c_str(), childDOB.c_str(), childGender.c_str(), avatar.c_str());
    request->_requestTag = TagRegisterChild;
    request->_method = "POST";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::RegisterChildRequestWithAvatarData(const std::string& childProfileName,
													   		const std::string& childGender,
													   		const std::string& childDOB,
													   		const std::string& imgData,
													   		HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = "/api/user/child";
	request->_requestBody = "{\"profileName\":\"" + childProfileName + "\",\"dob\":\"" + childDOB + "\",\"sex\":\"" + childGender + "\",\"avatarImageData\":\"" + imgData + "\",\"password\":\"\"}";
	request->_requestTag = TagRegisterChild;
	request->_method = "POST";
	request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

HttpRequestCreator* API::UpdateChildRequest(const std::string& childId,
                                              const std::string& childProfileName,
                                              const std::string& childGender,
                                              const std::string& childDOB,
                                              const std::string& avatar,
                                              const std::string& ownerId,
                                              HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestBody = StringUtils::format("{\"id\":\"%s\",\"profileName\":\"%s\",\"dob\":\"%s\",\"sex\":\"%s\",\"avatar\":\"%s\",\"ownerId\":\"%s\"}", childId.c_str(), childProfileName.c_str(), childDOB.c_str(), childGender.c_str(), avatar.c_str(), ownerId.c_str());
    request->_requestTag = TagUpdateChild;
	request->_requestPath = "/api/user/child/" + childId;
    request->_method = "PATCH";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::DeleteChild(const std::string& childId,
                                     const std::string& childProfileName,
                                     const std::string& childGender,
                                     HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = StringUtils::format("/api/user/child/%s", childId.c_str());
    request->_requestBody = StringUtils::format("{\"profileName\":\"%s%s\",\"sex\":\"%s\",\"status\":\"DELETED\"}", childProfileName.c_str(),childId.c_str(), childGender.c_str());
    request->_requestTag = TagDeleteChild;
    request->_method = "PATCH";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::UpdateChildAvatar(const std::string &childId,
                                           const std::string &imageData,
                                           Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagUpdateChildAvatar;
    request->_requestPath = StringUtils::format("/api/user/child/%s/avatar", childId.c_str());
    request->_method = "PATCH";
    request->_encrypted = true;
    
    request->_requestBody = "{\"userId\":\"" + childId + "\", \"data\":\"" + imageData + "\"}";
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::VerifyGooglePaymentRequest(const std::string& parentId,
                                                    const std::string& orderId,
                                                    const std::string& iapSku,
                                                    const std::string& purchaseToken,
                                                    HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = StringUtils::format("/api/billing/google/user/%s/receipt", parentId.c_str());
    request->_requestBody = StringUtils::format("{\"orderId\": \"%s\", \"subscriptionId\": \"%s\", \"purchaseToken\": \"%s\"}", orderId.c_str(), iapSku.c_str(), purchaseToken.c_str());
    request->_requestTag = TagVerifyGooglePayment;
    request->_method = "POST";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::VerifyAmazonPaymentRequest(const std::string& parentId,
                                                    const std::string& requestId,
                                                    const std::string& receiptId,
                                                    const std::string& amazonUserid,
                                                    HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = StringUtils::format("/api/billing/amazon/user/%s/receipt", parentId.c_str());
    request->_requestBody = StringUtils::format("{\"requestId\": \"%s\", \"receiptId\": \"%s\", \"amazonUserId\": \"%s\"}", requestId.c_str(), receiptId.c_str(), amazonUserid.c_str());
    request->_requestTag = TagVerifyAmazonPayment;
    request->_method = "POST";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::VerifyApplePaymentRequest(const std::string& parentId,
                                                   const std::string& receiptData,
                                                   const std::string& transactionID,
                                                   HttpRequestCreatorResponseDelegate* delegate)
{
    const std::string& transactionIDList = (transactionID.empty()) ? "[]" : StringUtils::format("[\"%s\"]", transactionID.c_str());
    
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = StringUtils::format("/api/billing/apple/user/%s/receipt", parentId.c_str());
    request->_requestBody = StringUtils::format("{\"receipt-data\": \"%s\", \"newTransactionIdList\": %s}", receiptData.c_str(), transactionIDList.c_str());
    request->_requestTag = TagVerifyApplePayment;
    request->_method = "POST";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::GetEncryptedContentRequest(const std::string& url,
                                                    const std::string& category,
                                                    HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_url = url;
    request->_requestTag = category;
    request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::GetPublicContentRequest(const std::string& url,
                                                 const std::string& category,
                                                 HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_url = url;
    request->_requestTag = category;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::GetContent(const std::string& requestId, 
                                                  const std::string& childId,
                                                  const std::string& contentID,
                                                  HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = requestId;
    request->_requestPath = StringUtils::format("/api/electricdreams/%s/content/%s", childId.c_str(), contentID.c_str());
    request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::ResetPaswordRequest(const std::string& forEmailAddress,
                                                  HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagResetPasswordRequest;
    request->_requestPath = StringUtils::format("/api/auth/requestPasswordReset?emailAddress=%s", forEmailAddress.c_str());
    request->_encrypted = false;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::GetContentPoolRequest(const std::string& childId, Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagGetContentPoolRequest;
    request->_requestPath = StringUtils::format("/api/electricdreams/v3/%s/items",childId.c_str());
    request->_method = "GET";
    request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::GetContentPoolRequest(const std::string& childId,
                                               const APIResponseSuccessCallback& onSuccess,
                                               const APIResponseFailureCallback& onFailure)
{
    HttpRequestCreator* request = new HttpRequestCreator(nullptr);
    request->_requestTag = TagGetContentPoolRequest;
    request->_requestPath = StringUtils::format("/api/electricdreams/v3/%s/items",childId.c_str());
    request->_method = "GET";
    request->_encrypted = true;
    request->setRequestCallback([onSuccess, onFailure, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, request, onSuccess, onFailure);
    });
    return request;
}

HttpRequestCreator* API::GetHQStructureDataRequest(const std::string& childId, Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagGetHqStructureDataRequest;
    request->_requestPath = StringUtils::format("/api/electricdreams/v3/%s/feeds",childId.c_str());
    request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::GetHQStructureDataRequest(const std::string& childId,
                                                   const APIResponseSuccessCallback& onSuccess,
                                                   const APIResponseFailureCallback& onFailure)
{
    HttpRequestCreator* request = new HttpRequestCreator(nullptr);
    request->_requestTag = TagGetHqStructureDataRequest;
    request->_requestPath = StringUtils::format("/api/electricdreams/v3/%s/feeds",childId.c_str());
    request->_encrypted = true;
    request->setRequestCallback([onSuccess, onFailure, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, request, onSuccess, onFailure);
    });
    return request;
}

HttpRequestCreator* API::UpdateParentDetailsRequest(const std::string &parentId,
                                                    const std::string &displayName,
                                                    const std::string &pinNumber,
                                                    Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestBody = StringUtils::format("{\"displayName\":\"%s\",\"pinNumber\":\"%s\"}", displayName.c_str(), pinNumber.c_str());
    request->_requestTag = TagUpdateParentDetails;
    request->_requestPath = StringUtils::format("/api/user/adult/%s",parentId.c_str());
    request->_method = "PATCH";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
};

HttpRequestCreator* API::UpdateParentPasswordRequest(const std::string &parentId,
                                                     const std::string &oldPassword,
                                                     const std::string &newPassword,
                                                     Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestBody = StringUtils::format("{\"oldPassword\":\"%s\",\"newPassword\":\"%s\"}", oldPassword.c_str(), newPassword.c_str());
    request->_requestTag = TagUpdateParentPassword;
    request->_requestPath = StringUtils::format("/api/user/v2/adult/%s/password",parentId.c_str());
    request->_method = "PATCH";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
};

HttpRequestCreator* API::getParentDetailsRequest(const std::string &parentId, Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagGetParentDetails;
    request->_requestPath = StringUtils::format("/api/user/adult/%s",parentId.c_str());
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::UpdateChildNameRequest(const std::string& childId,
                                                const std::string& newName,
                                                HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestBody = StringUtils::format("{\"profileName\":\"%s\"}", newName.c_str());
    request->_requestPath = StringUtils::format("/api/user/child/%s",childId.c_str());
    request->_requestTag = TagUpdateChildNameRequest;
    request->_method = "PATCH";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::AddVoucher(const std::string &parentId,
									const std::string &voucherCode,
									HttpRequestCreatorResponseDelegate *delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestBody = StringUtils::format("{\"voucherCode\":\"%s\"}", voucherCode.c_str());
	request->_requestPath = StringUtils::format("/api/billing/user/%s/voucher",parentId.c_str());
	request->_requestTag = TagAddVoucher;
	request->_method = "PATCH";
	request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

HttpRequestCreator* API::GetVideoProgress(const std::string &childId,
										  const std::string &videoId,
										  HttpRequestCreatorResponseDelegate *delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestPath = StringUtils::format("/api/useractivity/progress/%s/content/%s",childId.c_str(), videoId.c_str());
	request->_requestTag = TagGetVideoProgress;
	request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

HttpRequestCreator* API::UpdateContentProgressMeta(const std::string& childId,
											  const std::string& metaBody,
											  HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestPath = StringUtils::format("/api/useractivity/progress/%s",childId.c_str());
	request->_requestBody = metaBody;
	request->_requestTag = TagUpdateProgressMeta;
	request->_method = "POST";
	request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

HttpRequestCreator* API::GetMarketingAssets(const std::string& countryCode,
                                            HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = "/api/marketing/image/";
    request->_urlParameters = StringUtils::format("countryCode=%s&fallbackCountry=global", countryCode.c_str());
    request->_requestTag = TagGetMarketingAssets;
    request->_encrypted = true;
    request->_signAsParent = true;
    request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, delegate, request);
    });
    return request;
}

#pragma  mark - Oomee Maker

HttpRequestCreator* API::GetOomeeMakerAssets(const std::string& childId,
										HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestPath = StringUtils::format("/api/oomeemaker/zip/%s",childId.c_str());
	request->_requestTag = TagGetOomeeMakerAssets;
	request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}


HttpRequestCreator* API::SaveNewOomee(const std::string& childId,
											const std::string& adultId,
											const std::string& oomeeBodyId,
											const std::vector<std::string>& accessoryIds,
											bool selected,
											HttpRequestCreatorResponseDelegate* delegate)
{
	const std::string& accs = StringFunctions::joinStrings(accessoryIds, "\",\"");
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestPath = StringUtils::format("/api/oomeemaker/child/%s/oomees",childId.c_str());
	request->_requestTag = TagSaveNewOomee;
	request->_requestBody = "{\"childId\":\"" + childId + "\",\"adultId\":\"" + adultId + "\",\"oomeeId\":\"" + oomeeBodyId + "\",\"selected\":" + (selected ? "true" : "false") + ",\"oomeeItems\":[\"" + accs + "\"]}";
	request->_encrypted = true;
	request->_method = "POST";
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}


HttpRequestCreator* API::GetChildOomees(const std::string& childId,
                                        bool getOnlySelected,
											  HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestPath = StringUtils::format("/api/oomeemaker/child/%s/oomees",childId.c_str());
    if(getOnlySelected)
    {
        request->_urlParameters = "selected=true";
    }
	request->_requestTag = TagGetChildOomees;
	request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}


HttpRequestCreator* API::UpdateChildOomee(const std::string& childId,
												const std::string& oomeeId,
												const std::string& adultId,
												const std::string& oomeeBodyId,
												const std::vector<std::string>& accessoryIds,
												bool selected,
												HttpRequestCreatorResponseDelegate* delegate)
{
	const std::string& accs = StringFunctions::joinStrings(accessoryIds, "\",\"");
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestPath = StringUtils::format("/api/oomeemaker/child/%s/oomees/%s",childId.c_str(), oomeeId.c_str());
	request->_requestTag = TagUpdateChildOomee;
	request->_requestBody = "{\"childId\":\"" + childId + "\",\"adultId\":\"" + adultId + "\",\"oomeeId\":\"" + oomeeBodyId + "\",\"selected\":" + (selected ? "true" : "false") + ",\"oomeeItems\":[\"" + accs + "\"]}";
	request->_encrypted = true;
	request->_method = "PATCH";
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}


HttpRequestCreator* API::GetAllOomees(const std::string& adultId,
                                      bool getOnlySelected,
											HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestPath = StringUtils::format("/api/oomeemaker/adult/%s/oomees",adultId.c_str());
    if(getOnlySelected)
    {
        request->_urlParameters = "selected=true";
    }
	request->_requestTag = TagGetAllOomees;
	request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

HttpRequestCreator* API::DeleteChildOomee(const std::string& childId,
										  const std::string& oomeeId,
										  HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestPath = StringUtils::format("/api/oomeemaker/child/%s/%s",childId.c_str(), oomeeId.c_str());
	request->_requestTag = TagDeleteChildOomee;
	request->_encrypted = true;
	request->_method = "DELETE";
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

#pragma mark - Sharing

HttpRequestCreator* API::GetChatListRequest(const std::string& userId,
                                            bool isParent,
                                            HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagGetChatList;
    request->_requestPath = StringUtils::format("/api/share/%s/chat", userId.c_str());
    request->_encrypted = true;
    request->_signAsParent = isParent;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::GetChatMessagesRequest(const std::string& userId,
                                                const std::string& friendId,
                                                int pageNumber,
                                                bool isParent,
                                                HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagGetChatMessages;
    request->_requestPath = StringUtils::format("/api/share/v2/%s/%s", userId.c_str(), friendId.c_str());
    request->_urlParameters = StringUtils::format("page=%d", pageNumber);
    request->_encrypted = true;
    request->_signAsParent = isParent;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::SendChatMessageRequest(const std::string& userId,
                                                const std::string& friendId,
                                                const JsonObjectRepresentation& jsonObject,
                                                bool isParent,
                                                HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagSendChatMessage;
    request->_requestPath = StringUtils::format("/api/share/v2/%s/%s", userId.c_str(), friendId.c_str());
    request->_method = "POST";
    request->_encrypted = true;
    request->_signAsParent = isParent;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    
    // Create body from the json
    const auto& json = jsonObject.toJson();
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json.Accept(writer);
    request->_requestBody = buffer.GetString();
    return request;
}

HttpRequestCreator* API::MarkReadMessageRequest(const std::string& userId,
                                                const std::string& friendId,
                                                const uint64_t& readAt,
                                                bool isParent,
                                                HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagMarkReadMessage;
    request->_requestPath = StringUtils::format("/api/share/v2/%s/%s", userId.c_str(), friendId.c_str());
    request->_method = "PATCH";
    request->_encrypted = true;
    request->_signAsParent = isParent;
    request->_requestBody = StringUtils::format("{\"readAt\": \"%lld\"}", readAt);
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

HttpRequestCreator* API::SendChatReportRequest(const std::string &userId, const std::string &friendId, Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagReportChat;
    request->_requestPath = StringUtils::format("/api/share/v2/%s/%s/conversationstatus", userId.c_str(), friendId.c_str());
    request->_requestBody = "{\"status\": \"IN_MODERATION\"}";
    request->_method = "PUT";
    request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    
    return request;
}

HttpRequestCreator* API::ResetReportedChatRequest(const std::string &userId, const std::string &friendId, Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagResetReportedChat;
    request->_requestPath = StringUtils::format("/api/share/v2/%s/%s/conversationstatus", userId.c_str(), friendId.c_str());
    request->_requestBody = "{\"status\": \"ACTIVE\"}";
    request->_method = "PATCH";
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    
    return request;
}

HttpRequestCreator* API::GetTimelineSummary(const std::string &userId, Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestTag = TagGetTimelineSummary;
    request->_requestPath = StringUtils::format("/api/share/v2/%s/summary", userId.c_str());
    request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

#pragma mark - Friend Requests

HttpRequestCreator* API::GetPendingFriendRequests(const std::string &userId, HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    StringUtils::format("/api/user/adult/%s/invite/code/received", userId.c_str());
    request->_urlParameters = "status=CREATED";
    request->_requestTag = TagGetPendingFriendRequests;
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    
    return request;
}

HttpRequestCreator* API::FriendRequest(const std::string& senderChildId, const std::string& senderChildName, const std::string& inviteCode, HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = StringUtils::format("/api/user/child/%s/invite/code", senderChildId.c_str());
    request->_requestBody = StringUtils::format("{\"inviteeCode\": \"%s\", \"friendName\": \"\", \"senderName\": \"%s\"}", inviteCode.c_str(), senderChildName.c_str());
    request->_method = "POST";
    request->_requestTag = TagFriendRequest;
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    
    return request;
}

HttpRequestCreator* API::FriendRequestReaction(bool confirmed, const std::string& respondentChildId, const std::string& invitationId, const std::string& senderName, HttpRequestCreatorResponseDelegate* delegate)
{
    std::string status = "APPROVED";
    if(!confirmed) status = "REJECTED";
    
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = StringUtils::format("/api/user/child/%s/invite/code/%s", respondentChildId.c_str(), invitationId.c_str());
    request->_requestBody = StringUtils::format("{\"status\": \"%s\", \"friendName\": \"%s\"}", status.c_str(), senderName.c_str());
    request->_method = "POST";
    request->_requestTag = TagFriendRequestReaction;
    request->_encrypted = true;
    request->_signAsParent = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
    return request;
}

#pragma mark - Rewards

HttpRequestCreator* API::RedeemReward(const std::string& rewardId,
									  const APIResponseSuccessCallback& onSuccess,
                                      const APIResponseFailureCallback& onFailure)
{
	HttpRequestCreator* request = new HttpRequestCreator(nullptr);
	request->_requestPath = StringUtils::format("/api/rewards/%s",rewardId.c_str());
	request->_requestBody = "{\"status\": \"REDEEMED\"}";
	request->_method = "PATCH";
	request->_requestTag = TagRedeemReward;
	request->_encrypted = true;
	request->setRequestCallback([onSuccess, onFailure, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, request, onSuccess, onFailure);
	});
	return request;
}

HttpRequestCreator* API::GetPendingRewards(const std::string& userId,
											 HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestPath = "/api/rewards/";
	request->_urlParameters = StringUtils::format("status=PENDING&userId=%s",userId.c_str());
	request->_requestTag = TagGetPendingRewards;
	request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

HttpRequestCreator* API::BuyReward(const std::string& purchaseUrl,
									 HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_url = AppConfig::getInstance()->getServerUrl() + purchaseUrl;
	request->_requestTag = TagBuyReward;
	request->_requestBody = "{}";
	request->_method = "POST";
	request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

HttpRequestCreator* API::GetInventory(const std::string& userId,
                                      const APIResponseSuccessCallback& onSuccess,
                                      const APIResponseFailureCallback& onFailure)
{
	HttpRequestCreator* request = new HttpRequestCreator(nullptr);
	request->_requestPath = StringUtils::format("/api/inventory/%s",userId.c_str());
	request->_requestTag = TagGetInventory;
	request->_encrypted = true;
	request->setRequestCallback([onSuccess, onFailure, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, request, onSuccess, onFailure);
    });
	return request;
}

HttpRequestCreator* API::RewardCallback(const std::string& url,
									  HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_url = url;
	request->_requestTag = TagRewardCallback;
	request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

HttpRequestCreator* API::GetRewardStrategy(const std::string& method, HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->_requestPath = "/api/rewards/strategy/feed";
    request->_requestTag = StringUtils::format("%s-%s", TagGetRewardStrategy, method.c_str());
    request->_encrypted = true;
    request->_method = method;
    request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
        HandleAPIResponse(sender, response, delegate, request);
    });
    return request;
}

HttpRequestCreator* API::GetShopFeed(HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestPath = "/api/shopfront/feed";
	request->_requestTag = TagGetShopFeed;
	request->_encrypted = true;
	request->setRequestCallback([delegate, request](cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
		HandleAPIResponse(sender, response, delegate, request);
	});
	return request;
}

#pragma mark - Vodacom

HttpRequestCreator* API::GetVodacomTransactionId(const std::string& userId,
											HttpRequestCreatorResponseDelegate* delegate)
{
	HttpRequestCreator* request = new HttpRequestCreator(delegate);
	request->_requestTag = TagGetVodacomTransactionId;
	request->_requestPath = StringUtils::format("/api/vodasabilling/%s/receipt", userId.c_str());
	request->_requestBody = "{}";
	request->_method = "POST";
	request->_encrypted = true;
    request->_signAsParent = true;
	return request;
}


NS_AZOOMEE_END
