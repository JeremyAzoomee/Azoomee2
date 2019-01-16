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
#include "../Json.h"

NS_AZOOMEE_BEGIN

class Child;
typedef std::shared_ptr<Child> ChildRef;

class Child
{
private:
	std::string _profileName;
	std::string _avatar;
	std::string _inviteCode;
	std::string _sex;
	std::string _dob;
	std::string _id;
	
	std::string _cdnSessionId;
	std::string _apiSecret;
	std::string _apiKey;
	
	Child();
public:
	
	static ChildRef createWithJson(const rapidjson::Value& childData);
	static ChildRef create();
	
	void parseLoginData(const rapidjson::Document& loginData);
	
	void setAvatar(const std::string& avatarUrl);
	void setId(const std::string& childId);
	void setDOB(const std::string& dob);
	void setProfileName(const std::string& name);
	
	std::string getProfileName() const;
	std::string getAvatar() const;
	std::string getInviteCode() const;
	std::string getSex() const;
	std::string getDOB() const;
	std::string getId() const;
	
	std::string getCDNSessionId() const;
	std::string getAPISecret() const;
	std::string getAPIKey() const;
	
};

NS_AZOOMEE_END

#endif /* Child_h */
