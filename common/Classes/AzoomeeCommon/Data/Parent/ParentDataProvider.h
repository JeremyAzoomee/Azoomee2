#ifndef AzoomeeCommon_ParentDataProvider_h
#define AzoomeeCommon_ParentDataProvider_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "../Child/Child.h"
#include "FriendRequest.h"


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
	FriendRequestRef getPendingFriendRequest(int pendingFriendRequestNo) const;
};

}

#endif
