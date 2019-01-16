#ifndef AzoomeeCommon_ParentDataProvider_h
#define AzoomeeCommon_ParentDataProvider_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "../Child/Child.h"


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
    std::string getProfileNameForAnAvailableChild(int childNumber) const;
    std::string getProfileNameForAnAvailableChildById(const std::string& childId) const;
    std::string getAvatarForAnAvailableChild(int childNumber) const;
    std::string getAvatarForAnAvailableChildById(const std::string& childId) const;
    std::string getDOBForAnAvailableChild(int childNumber) const;
    std::string getSexForAnAvailableChild(int childNumber) const;
    std::string getIDForAvailableChildren(int childNumber) const;
    std::string getInviteCodeForAnAvailableChild(int childNumber) const;
    std::string getInviteCodeForChildName(const std::string& name) const;
	
	ChildRef getChildForId(const std::string& childId) const;
	ChildRef getChild(int childNumber) const;
	
    std::string getLoggedInParentActorStatus() const;
    std::string getLoggedInParentId() const;
    std::string getLoggedInParentCdnSessionId() const;
    std::string getLoggedInParentApiKey() const;
    std::string getLoggedInParentApiSecret() const;
    std::string getLoggedInParentAvatarId() const;
    std::string getLoggedInParentCountryCode() const;
    std::string getParentPin() const;
    std::string getParentDisplayName() const;
    std::string getParentEmail() const;
    std::string getBillingStatus() const;
    std::string getBillingDate() const;
    std::string getBillingProvider() const;
    bool isLoggedInParentAnonymous();
    bool isBillingDataAvailable();
    bool isPaidUser();
    bool emailRequiresVerification();
    bool isUserLoggedIn();
    
    //-----------Pending Friend Requests-------------
    int getNoOfPendingFriendRequest();
    std::string getPendingFriendRequestSenderName(int pendingFriendRequestNo) const;
    std::string getPendingFriendRequestFriendName(int pendingFriendRequestNo) const;
    std::string getPendingFriendRequestInviteCode(int pendingFriendRequestNo) const;
    std::string getPendingFriendRequestRequestID(int pendingFriendRequestNo) const;
    std::string getPendingFriendRequestSenderID(int pendingFriendRequestNo) const;
    std::string getPendingFriendRequestRespondentID(int pendingFriendRequestNo) const;
};

}

#endif
