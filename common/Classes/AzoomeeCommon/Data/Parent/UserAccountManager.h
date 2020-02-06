//
//  UserAccountManager.h
//  AzoomeeCommon
//
//  Created by Macauley on 06/06/2019.
//

#ifndef UserAccountManager_h
#define UserAccountManager_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "BillingData.h"
#include "../Child/Child.h"
#include "Parent.h"
#include "FriendRequest.h"
#include <map>

NS_AZOOMEE_BEGIN

class UserAccountManager
{
    typedef std::function<void(bool, long)> OnCompleteCallback;
public:
    static const std::string kAnonLoginPW;
    static const char* const kAnonEmailKey;
    static const char* const kStoredUsernameKey;
    static const char* const kAnonOnboardingCompleteKey;
    
	static UserAccountManager* getInstance();
	
	virtual ~UserAccountManager();
	
	void setBillingData(const BillingDataRef& billingData);
	BillingDataRef getBillingData() const;
	void setBillingDataAvailable(bool available);
	bool isBillingDataAvailable() const;
	
	void setParent(const MutableParentRef& parent);
	ParentRef getParent() const;
	
	void setPendingFriendRequests(const std::vector<FriendRequestRef>& pendingRequests);
	std::vector<FriendRequestRef> getPendingFriendRequests() const;
	
	
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
	
	bool parseParentLoginData(const std::string &responseData);
	bool parseAvailableChildren(const std::string &responseData);
	bool parseUpdateParentData(const std::string &responseData);
	void parseParentBillingData(const std::string &responseData);
	void parseParentSessionData(const std::string &responseData);
	void parseParentDetails(const std::string &responseData);
	void parseChildUpdateData(const std::string& childId, const std::string& responseData);
	bool parseChildLoginData(const std::string& responseData);
	void logoutChild();
	void retrieveParentLoginDataFromUserDefaults();
	bool hasParentLoginDataInUserDefaults();
	void clearParentLoginDataFromUserDefaults();
	bool parsePendingFriendRequests(const std::string &responseData);
	void setLoggedInParentCountryCode(const std::string &countryCode);
	
	void saveAnonCredentialsToDevice(const std::string& userId);
	
    void setAvatarColourForChild(const std::string& childId, const cocos2d::Color4B& colour);
	
    void logoutParent();
    
    bool localLogin();
    void login(const std::string& email, const std::string& password, const OnCompleteCallback& callback);
    void anonLogin(const OnCompleteCallback& callback);
    
    void getChildrenForLoggedInParent(const OnCompleteCallback& callback);
    
    void getBillingDataForLoggedInParent(const OnCompleteCallback& callback);
    
    void loginChild(const std::string& profileName, const OnCompleteCallback& callback);
    
    void setHasLoggedInOnDevice(bool loggedIn);
    bool userHasLoggedInOnDevice();
    
private:
	BillingDataRef _billingData = nullptr;
	MutableParentRef _parent = nullptr;
	
	bool _isBillingDataAvailable = false;
	
	std::vector<MutableChildRef> _availableChildren;
	std::map<std::string, MutableChildRef> _availableChildrenById;
	
	std::vector<FriendRequestRef> _pendingFriendRequests;
	
	void addParentLoginDataToUserDefaults();
	void clearAvailableChildren();
	void addChild(const MutableChildRef& child);
	
    static const std::string kUserHasLoggedInOnDeviceKey;
};

NS_AZOOMEE_END

#endif /* UserAccountManager_h */
