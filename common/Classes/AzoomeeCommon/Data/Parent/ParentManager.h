//
//  ParentManager.h
//  AzoomeeCommon
//
//  Created by Macauley on 06/06/2019.
//

#ifndef ParentManager_h
#define ParentManager_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "BillingData.h"
#include "../Child/Child.h"
#include "Parent.h"
#include "FriendRequest.h"
#include <map>

NS_AZOOMEE_BEGIN

class ParentManager
{
		
public:
	static ParentManager* getInstance();
	
	virtual ~ParentManager();
	
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
	//std::vector<ChildRef> getAvailableChildren() const;
	//std::map<std::string, ChildRef> getAvailableChildrenById() const;
};

NS_AZOOMEE_END

#endif /* ParentManager_h */
