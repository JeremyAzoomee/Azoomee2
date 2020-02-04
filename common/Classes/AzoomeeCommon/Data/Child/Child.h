//
//  Child.h
//  AzoomeeCommon
//
//  Created by Macauley on 16/01/2019.
//

#ifndef Child_h
#define Child_h

#include "../../Azoomee.h"
#include <string>
#include <memory>
#include <chrono>
#include "../Json.h"
#include "Inventory.h"

NS_AZOOMEE_BEGIN

class Child;
class MutableChild;
typedef std::shared_ptr<Child> ChildRef;
typedef std::shared_ptr<MutableChild> MutableChildRef;

class Child
{
protected:
	std::string _profileName;
	std::string _avatar;
	std::string _inviteCode;
	std::string _sex;
	std::string _dob;
	std::string _id;
	
	std::string _cdnSessionId;
	std::string _apiSecret;
	std::string _apiKey;
	std::chrono::milliseconds _sessionExpiryTimestamp;
	
	InventoryRef _inventory = nullptr;
	
    cocos2d::Color4B _avatarColour;
    
	Child();
public:
	
	std::string getProfileName() const;
	std::string getAvatar() const;
	std::string getInviteCode() const;
	std::string getSex() const;
	std::string getDOB() const;
	std::string getId() const;
	
	std::string getCDNSessionId() const;
	std::string getAPISecret() const;
	std::string getAPIKey() const;
	bool isSessionExpired() const;
	
	InventoryRef getInventory() const;
	
    cocos2d::Color4B getAvatarColour() const;
};

class MutableChild : public Child
{
public:
	static MutableChildRef createWithJson(const rapidjson::Value& childData);
	static MutableChildRef create();
	
	void parseLoginData(const rapidjson::Document& loginData);
	void parseChildData(const rapidjson::Value& childData);
	
	void setInventory(const InventoryRef& inventory);
	
	void setAvatar(const std::string& avatarUrl);
	void setId(const std::string& childId);
	void setDOB(const std::string& dob);
	void setProfileName(const std::string& name);
	
	void setCDNSessionId(const std::string& sessionId, std::chrono::milliseconds sessionDurationMillis);
	
    void setAvatarColour(const cocos2d::Color4B& colour);
    
};

NS_AZOOMEE_END

#endif /* Child_h */
