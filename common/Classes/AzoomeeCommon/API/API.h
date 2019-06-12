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
    
    static const char* const TagIpCheck;
    static const char* const TagLogin;
	static const char* const TagGetAnonCredentials;
    static const char* const TagUpdateBillingData;
    static const char* const TagParentPin;
    static const char* const TagGetAvailableChildren;
    static const char* const TagChildLogin;
    static const char* const TagGetGorden;
    static const char* const TagRegisterParent;
    static const char* const TagRegisterChild;
    static const char* const TagUpdateChild;
    static const char* const TagDeleteChild;
    static const char* const TagVerifyGooglePayment;
    static const char* const TagVerifyAmazonPayment;
    static const char* const TagVerifyApplePayment;
    static const char* const TagGetEncryptedContent;
    static const char* const TagGetPublicContent;
    static const char* const TagGetChatList;
    static const char* const TagGetChatMessages;
    static const char* const TagSendChatMessage;
    static const char* const TagMarkReadMessage;
    static const char* const TagResetPasswordRequest;
    static const char* const TagOfflineCheck;
    static const char* const TagFriendRequest;
    static const char* const TagFriendRequestReaction;
    static const char* const TagGetPendingFriendRequests;
    static const char* const TagReportChat;
    static const char* const TagResetReportedChat;
    static const char* const TagGetTimelineSummary;
    static const char* const TagGetForceUpdateInformation;
    static const char* const TagCookieRefresh;
	static const char* const TagChildCookieRefresh;
    static const char* const TagGetContentPoolRequest;
    static const char* const TagGetHqStructureDataRequest;
    static const char* const TagUpdateChildAvatar;
    static const char* const TagUpdateParentDetails;
    static const char* const TagUpdateParentPassword;
    static const char* const TagGetParentDetails;
    static const char* const TagUpdateChildNameRequest;
	static const char* const TagAddVoucher;
	static const char* const TagGetVodacomTransactionId;
	static const char* const TagGetVideoProgress;
	static const char* const TagUpdateProgressMeta;
	static const char* const TagRewardCallback;
	static const char* const TagRedeemReward;
	static const char* const TagGetPendingRewards;
	static const char* const TagGetInventory;
	static const char* const TagBuyReward;
	static const char* const TagGetShopFeed;
	static const char* const TagGetOomeeMakerAssets;
	
	static const std::string kAZCountryCodeKey;
	
#pragma mark - API Methods
	
	static void HandleAPIResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response, HttpRequestCreatorResponseDelegate* delegate, HttpRequestCreator* request);
	static void HandleAPIError(cocos2d::network::HttpResponse *response, HttpRequestCreatorResponseDelegate* delegate, HttpRequestCreator* request);
    
    static HttpRequestCreator* IpCheck(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* OfflineCheck(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* LoginRequest(const std::string& username,
                                            const std::string& password,
                                            HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* GetAnonCredentials(HttpRequestCreatorResponseDelegate* delegate);
    
	static HttpRequestCreator* UpdateBillingDataRequest(const std::string& parentId,
														HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetForceUpdateInformationRequest(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* UpdateParentPinRequest(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetAvailableChildrenRequest(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* ChildLoginRequest(const std::string& profileName,
                                                 HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetGordenRequest(const std::string& userId,
                                                const std::string& sessionId,
                                                HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* RefreshParentCookiesRequest(HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* RefreshChildCookiesRequest(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* RegisterParentRequest(const std::string& parentId,
													 const std::string& emailAddress,
                                                     const std::string& password,
                                                     const std::string& pinNumber,
                                                     const std::string& source,
                                                     const std::string& sourceDevice,
                                                     const std::string& marketingAccepted,
                                                     HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* RegisterChildRequest(const std::string& childProfileName,
                                                    const std::string& childGender,
                                                    const std::string& childDOB,
                                                    const std::string& avatar,
                                                    HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* RegisterChildRequestWithAvatarData(const std::string& childProfileName,
													const std::string& childGender,
													const std::string& childDOB,
													const std::string& imgData,
													HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* UpdateChildRequest(const std::string& childId,
                                                    const std::string& childProfileName,
                                                    const std::string& childGender,
                                                    const std::string& childDOB,
                                                    const std::string& avatar,
                                                    const std::string& ownerId,
                                                    HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* DeleteChild(const std::string& childId,
                                           const std::string& childProfileName,
                                           const std::string& childGender,
                                           HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* UpdateChildAvatar(const std::string& childId,
                                                 const std::string& imageData,
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
    
    static HttpRequestCreator* GetContent(const std::string& requestId, 
                                                        const std::string& childId,
                                                        const std::string& contentID,
                                                        HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* ResetPaswordRequest(const std::string& forEmailAddress,
                                                   HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetContentPoolRequest(const std::string& childId,
                                                     HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetHQStructureDataRequest(const std::string& childId,
                                                         HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* UpdateParentDetailsRequest(const std::string& parentId,
                                                          const std::string& displayName,
                                                          const std::string& pinNumber,
                                                          HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* UpdateParentPasswordRequest(const std::string& parentId,
                                                          const std::string& oldPassword,
                                                          const std::string& newPassword,
                                                          HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* getParentDetailsRequest(const std::string& parentId,
                                                       HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* UpdateChildNameRequest(const std::string& childId,
                                                      const std::string& newName,
                                                      HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* AddVoucher(const std::string& parentId,
										  const std::string& voucherCode,
										  HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* GetVideoProgress(const std::string& childId,
												const std::string& videoId,
												HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* UpdateVideoProgress(const std::string& childId,
												   const std::string& videoId,
												   int videoProgressSeconds,
												   HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* UpdateContentProgressMeta(const std::string& childId,
														 const std::string& metaBody,
														 HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* GetOomeeMakerAssets(const std::string& childId,
												   HttpRequestCreatorResponseDelegate* delegate);
	
#pragma mark - Friend Requests
    
    static HttpRequestCreator* FriendRequest(const std::string& senderChildId,
                                             const std::string& senderChildName,
                                             const std::string& inviteCode,
                                             HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* FriendRequestReaction(bool confirmed,
                                                     const std::string& respondentChildId,
                                                     const std::string& invitationId,
                                                     const std::string& senderName,
                                                     HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetPendingFriendRequests(HttpRequestCreatorResponseDelegate* delegate);
    
#pragma mark - Sharing
    
    // Get the chat list for childId
    // userId must be the currently logged in child or parent, or the request will fail
    static HttpRequestCreator* GetChatListRequest(const std::string& userId,
                                                  HttpRequestCreatorResponseDelegate* delegate);
    
    // Get the chat list between childId and friendId
    // userId must be the currently logged in child or parent, or the request will fail
    static HttpRequestCreator* GetChatMessagesRequest(const std::string& userId,
                                                      const std::string& friendId,
                                                      int pageNumber,
                                                      HttpRequestCreatorResponseDelegate* delegate);
    
    // Send a chat message to friendId
    // userId must be the currently logged in child or parent, or the request will fail
    // params must be a Json object of type Message
    static HttpRequestCreator* SendChatMessageRequest(const std::string& userId,
                                                      const std::string& friendId,
                                                      const JsonObjectRepresentation& jsonObject,
                                                      HttpRequestCreatorResponseDelegate* delegate);
    
    // Mark messages between user and friend as read
    // userId must be the currently logged in child or parent, or the request will fail
    static HttpRequestCreator* MarkReadMessageRequest(const std::string& userId,
                                                      const std::string& friendId,
                                                      const uint64_t& readAt,
                                                      HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* SendChatReportRequest(const std::string& userId,
                                                     const std::string& friendId,
                                                     HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* ResetReportedChatRequest(const std::string& userId,
                                                     const std::string& friendId,
                                                     HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetTimelineSummary(const std::string& userId,
                                                        HttpRequestCreatorResponseDelegate* delegate);
    
#pragma mark - Rewards
	
	static HttpRequestCreator* RedeemReward(const std::string& rewardId,
											HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* GetPendingRewards(const std::string& userId,
												 HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* BuyReward(const std::string& purchaseUrl,
										 HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* GetInventory(const std::string& userId,
											HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* RewardCallback(const std::string& url,
											  HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* GetShopFeed(HttpRequestCreatorResponseDelegate* delegate);
	
#pragma mark - Vodacom
	static HttpRequestCreator* GetVodacomTransactionId(const std::string& userId,
													   HttpRequestCreatorResponseDelegate* delegate);
};
  
NS_AZOOMEE_END

#endif
