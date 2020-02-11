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
    static const char* const TagGetSessionCookies;
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
    static const char* const TagGetRewardStrategy;
	static const char* const TagGetShopFeed;
	static const char* const TagGetOomeeMakerAssets;
    static const char* const TagGetMarketingAssets;
	static const char* const TagSaveNewOomee;
    static const char* const TagSaveLocalOomee;
	static const char* const TagGetChildOomees;
	static const char* const TagUpdateChildOomee;
	static const char* const TagGetAllOomees;
	static const char* const TagDeleteChildOomee;
    
    
	static const std::string kAZCountryCodeKey;
	
    static const std::string kRewardHeaderCheckEventName;
    
#pragma mark - API Methods
    
    typedef std::function<void(const std::string&, const std::string&, const std::string&)> APIResponseSuccessCallback;
    typedef std::function<void(const std::string&, long)> APIResponseFailureCallback;
	
    static void HandleAPIResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response, HttpRequestCreatorResponseDelegate* delegate, HttpRequestCreator* request);
    static void HandleAPIResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response, HttpRequestCreator* request, const APIResponseSuccessCallback& onSuccess, const APIResponseFailureCallback& onFailure);
	static void HandleAPIError(cocos2d::network::HttpResponse *response, HttpRequestCreator* request, const APIResponseFailureCallback& onFailure);
    
    static HttpRequestCreator* IpCheck(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* OfflineCheck(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* LoginRequest(const std::string& username,
                                            const std::string& password,
                                            HttpRequestCreatorResponseDelegate* delegate);
	
    static HttpRequestCreator* LoginRequest(const std::string& username,
                                            const std::string& password,
                                            const APIResponseSuccessCallback& onSuccess,
                                            const APIResponseFailureCallback& onFailure);
    
	static HttpRequestCreator* GetAnonCredentials(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetAnonCredentials(const APIResponseSuccessCallback& onSuccess,
                                                  const APIResponseFailureCallback& onFailure);
    
	static HttpRequestCreator* UpdateBillingDataRequest(const std::string& parentId,
														HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* UpdateBillingDataRequest(const std::string& parentId,
                                                        const APIResponseSuccessCallback& onSuccess,
                                                        const APIResponseFailureCallback& onFailure);
    
    static HttpRequestCreator* GetForceUpdateInformationRequest(HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* UpdateParentPinRequest(const std::string& parentId,
                                                      HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetAvailableChildrenRequest(const std::string& parentId,
                                                           HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetAvailableChildrenRequest(const std::string& parentId,
                                                           const APIResponseSuccessCallback& onSuccess,
                                                           const APIResponseFailureCallback& onFailure);
    
    static HttpRequestCreator* ChildLoginRequest(const std::string& profileName,
                                                 HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* ChildLoginRequest(const std::string& profileName,
                                                 const APIResponseSuccessCallback& onSuccess,
                                                 const APIResponseFailureCallback& onFailure);
    
    static HttpRequestCreator* GetSessionCookiesRequest(const std::string& userId,
                                                        const std::string& sessionId,
                                                        bool parentSigned,
                                                        HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetSessionCookiesRequest(const std::string& userId,
                                                        const std::string& sessionId,
                                                        bool parentSigned,
                                                        const APIResponseSuccessCallback& onSuccess,
                                                        const APIResponseFailureCallback& onFailure);
    
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
    
    static HttpRequestCreator* VerifyGooglePaymentRequest(const std::string& parentId,
                                                          const std::string& orderId,
                                                          const std::string& iapSku,
                                                          const std::string& purchaseToken,
                                                          HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* VerifyAmazonPaymentRequest(const std::string& parentId,
                                                          const std::string& requestId,
                                                          const std::string& receiptId,
                                                          const std::string& amazonUserid,
                                                          HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* VerifyApplePaymentRequest(const std::string& parentId,
                                                         const std::string& receiptData,
                                                         const std::string& transactionID,
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
    
    static HttpRequestCreator* GetContentPoolRequest(const std::string& childId,
                                                     const APIResponseSuccessCallback& onSuccess,
                                                     const APIResponseFailureCallback& onFailure);
    
    static HttpRequestCreator* GetHQStructureDataRequest(const std::string& childId,
                                                         HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetHQStructureDataRequest(const std::string& childId,
                                                         const APIResponseSuccessCallback& onSuccess,
                                                         const APIResponseFailureCallback& onFailure);
    
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

#pragma mark - Oomee Maker
	
	static HttpRequestCreator* GetOomeeMakerAssets(const std::string& childId,
												   HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetMarketingAssets(const std::string& countryCode,
                                                  HttpRequestCreatorResponseDelegate* delegate);

	static HttpRequestCreator* SaveNewOomee(const std::string& childId,
											const std::string& adultId,
											const std::string& oomeeBodyId,
											const std::vector<std::string>& accessoryIds,
											bool selected,
											HttpRequestCreatorResponseDelegate* delegate);

	static HttpRequestCreator* GetChildOomees(const std::string& childId,
                                              bool getOnlySelected,
											  HttpRequestCreatorResponseDelegate* delegate);

	static HttpRequestCreator* UpdateChildOomee(const std::string& childId,
												const std::string& oomeeId,
												const std::string& adultId,
												const std::string& oomeeBodyId,
												const std::vector<std::string>& accessoryIds,
												bool selected,
												HttpRequestCreatorResponseDelegate* delegate);

	static HttpRequestCreator* GetAllOomees(const std::string& adultId,
                                            bool getOnlySelected,
                                            HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* DeleteChildOomee(const std::string& childId,
												const std::string& oomeeId,
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
    
    static HttpRequestCreator* GetPendingFriendRequests(const std::string &userId, HttpRequestCreatorResponseDelegate* delegate);
    
#pragma mark - Sharing
    
    // Get the chat list for childId
    // userId must be the currently logged in child or parent, or the request will fail
    static HttpRequestCreator* GetChatListRequest(const std::string& userId,
                                                  bool isParent,
                                                  HttpRequestCreatorResponseDelegate* delegate);
    
    // Get the chat list between childId and friendId
    // userId must be the currently logged in child or parent, or the request will fail
    static HttpRequestCreator* GetChatMessagesRequest(const std::string& userId,
                                                      const std::string& friendId,
                                                      int pageNumber,
                                                      bool isParent,
                                                      HttpRequestCreatorResponseDelegate* delegate);
    
    // Send a chat message to friendId
    // userId must be the currently logged in child or parent, or the request will fail
    // params must be a Json object of type Message
    static HttpRequestCreator* SendChatMessageRequest(const std::string& userId,
                                                      const std::string& friendId,
                                                      const JsonObjectRepresentation& jsonObject,
                                                      bool isParent,
                                                      HttpRequestCreatorResponseDelegate* delegate);
    
    // Mark messages between user and friend as read
    // userId must be the currently logged in child or parent, or the request will fail
    static HttpRequestCreator* MarkReadMessageRequest(const std::string& userId,
                                                      const std::string& friendId,
                                                      const uint64_t& readAt,
                                                      bool isParent,
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
											const APIResponseSuccessCallback& onSuccess,
                                            const APIResponseFailureCallback& onFailure);
	
	static HttpRequestCreator* GetPendingRewards(const std::string& userId,
												 HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* BuyReward(const std::string& purchaseUrl,
										 HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* GetInventory(const std::string& userId,
											const APIResponseSuccessCallback& onSuccess,
                                            const APIResponseFailureCallback& onFailure);
	
	static HttpRequestCreator* RewardCallback(const std::string& url,
											  HttpRequestCreatorResponseDelegate* delegate);
    
    static HttpRequestCreator* GetRewardStrategy(const std::string& method, HttpRequestCreatorResponseDelegate* delegate);
	
	static HttpRequestCreator* GetShopFeed(HttpRequestCreatorResponseDelegate* delegate);
	
#pragma mark - Vodacom
	static HttpRequestCreator* GetVodacomTransactionId(const std::string& userId,
													   HttpRequestCreatorResponseDelegate* delegate);
};
  
NS_AZOOMEE_END

#endif
