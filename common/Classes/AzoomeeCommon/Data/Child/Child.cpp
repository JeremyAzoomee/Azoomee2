//
//  Child.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 16/01/2019.
//

#include "Child.h"

NS_AZOOMEE_BEGIN

ChildRef Child::create()
{
	return ChildRef(new Child());
}

ChildRef Child::createWithJson(const rapidjson::Value& childData)
{
	ChildRef child = ChildRef(new Child());
	
	child->_profileName = getStringFromJson("profileName", childData);
	child->_avatar = getStringFromJson("avatar", childData);
	child->_inviteCode = getStringFromJson("inviteCode", childData);
	child->_sex = getStringFromJson("sex", childData);
	child->_dob = getStringFromJson("dob", childData);
	child->_id = getStringFromJson("id", childData);
	
	return child;
}

void Child::parseLoginData(const rapidjson::Document& loginData)
{
	if(loginData.HasParseError())
	{
		return;
	}
	
	_cdnSessionId = getStringFromJson("cdn-sessionid", loginData);
	_apiSecret = getStringFromJson("apiSecret", loginData);
	_apiKey = getStringFromJson("apiKey", loginData);
}

void Child::setAvatar(const std::string &avatarUrl)
{
	_avatar = avatarUrl;
}

void Child::setId(const std::string& childId)
{
	_id = childId;
}
void Child::setDOB(const std::string& dob)
{
	_dob = dob;
}
void Child::setProfileName(const std::string& name)
{
	_profileName = name;
}

Child::Child()
{
	
}

std::string Child::getProfileName() const
{
	return _profileName;
}
std::string Child::getAvatar() const
{
	return _avatar;
}
std::string Child::getInviteCode() const
{
	return _inviteCode;
}
std::string Child::getSex() const
{
	return _sex;
}
std::string Child::getDOB() const
{
	return _dob;
}
std::string Child::getId() const
{
	return _id;
}

std::string Child::getCDNSessionId() const
{
	return _cdnSessionId;
}
std::string Child::getAPISecret() const
{
	return _apiSecret;
}
std::string Child::getAPIKey() const
{
	return _apiKey;
}

NS_AZOOMEE_END
