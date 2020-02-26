//
//  Parent.h
//  AzoomeeCommon
//
//  Created by Macauley on 17/01/2019.
//

#ifndef Parent_h
#define Parent_h

#include "../../Tinizine.h"
#include <string>
#include <memory>
#include "../Json.h"

NS_TZ_BEGIN

class Parent;
class MutableParent;
typedef std::shared_ptr<Parent> ParentRef;
typedef std::shared_ptr<MutableParent> MutableParentRef;


class Parent
{
protected:
	std::string _avatar;
	std::string _id;
	std::string _actorStatus;
	std::string _pin;
	std::string _displayName;
	std::string _email;
	
	std::string _cdnSessionId;
	std::string _apiSecret;
	std::string _apiKey;
	
	std::string _countryCode;
	
	bool _anonymous;
	
	Parent();
public:

	std::string getDisplayName() const;
	std::string getAvatar() const;
	std::string getId() const;
	std::string getPin() const;
	std::string getEmail() const;
	std::string getActorStatus() const;
	
	std::string getCDNSessionId() const;
	std::string getAPISecret() const;
	std::string getAPIKey() const;
	
	std::string getCountryCode() const;
	
	bool isAnonymous() const;
	
};

class MutableParent : public Parent
{
public:
	static MutableParentRef createWithJson(const rapidjson::Document& parentData);
	static MutableParentRef create();
	
	void setAvatar(const std::string& avatarUrl);
	void setPin(const std::string& pin);
	void setDisplayName(const std::string& name);
	void setId(const std::string& parentId);
	void setEmail(const std::string& email);
	void setActorStatus(const std::string& actorStatus);
	
	void setCDNSessionId(const std::string& sessionId);
	void setAPISecret(const std::string& apiSecret);
	void setAPIKey(const std::string& apiKey);
	
	void setCountryCode(const std::string& countryCode);
	
	void setAnonymous(bool anonymous);
};

NS_TZ_END

#endif /* Parent_h */
