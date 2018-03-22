#ifndef AzoomeeCommon_ParentDataProvider_h
#define AzoomeeCommon_ParentDataProvider_h

#include <cocos/cocos2d.h>
#include "../Json.h"


namespace Azoomee
{

class ParentDataProvider : public cocos2d::Ref
{
    
public:
    static ParentDataProvider* getInstance(void);
    
public:
    virtual ~ParentDataProvider();
    bool init(void);
    
    int getAmountOfAvailableChildren();
    std::string getProfileNameForAnAvailableChild(int childNumber);
    std::string getProfileNameForAnAvailableChildById(const std::string& childId);
    std::string getAvatarForAnAvailableChild(int childNumber);
    std::string getAvatarForAnAvailableChildById(const std::string& childId);
    std::string getDOBForAnAvailableChild(int childNumber);
    std::string getSexForAnAvailableChild(int childNumber);
    std::string getIDForAvailableChildren(int childNumber);
    std::string getInviteCodeForAnAvailableChild(int childNumber);
    
    std::string getLoggedInParentActorStatus();
    std::string getLoggedInParentId();
    std::string getLoggedInParentCdnSessionId();
    std::string getLoggedInParentApiKey();
    std::string getLoggedInParentApiSecret();
    std::string getLoggedInParentAvatarId();
    std::string getLoggedInParentCountryCode();
    std::string getParentPin();
    std::string getBillingStatus();
    std::string getBillingDate();
    std::string getBillingProvider();
    bool isLoggedInParentAnonymous();
    bool isBillingDataAvailable();
    bool isPaidUser();
    bool emailRequiresVerification();
    bool isUserLoggedIn();
    
    //-----------Pending Friend Requests-------------
    int getNoOfPendingFriendRequest();
    std::string getPendingFriendRequestSenderName(int pendingFriendRequestNo);
    std::string getPendingFriendRequestFriendName(int pendingFriendRequestNo);
    std::string getPendingFriendRequestInviteCode(int pendingFriendRequestNo);
    std::string getPendingFriendRequestRequestID(int pendingFriendRequestNo);
    std::string getPendingFriendRequestSenderID(int pendingFriendRequestNo);
    std::string getPendingFriendRequestRespondentID(int pendingFriendRequestNo);
};

}

#endif
