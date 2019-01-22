//
//  Parent.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 17/01/2019.
//

#include "Parent.h"

NS_AZOOMEE_BEGIN

Parent::Parent()
{
	
}

ParentRef Parent::createWithJson(const rapidjson::Document& parentData)
{
	ParentRef parent = ParentRef(new Parent);

	parent->_id = getStringFromJson("id", parentData);
	parent->_cdnSessionId = getStringFromJson("cdn-sessionid", parentData);
	parent->_apiSecret = getStringFromJson("apiSecret", parentData);
	parent->_apiKey = getStringFromJson("apiKey", parentData);
	parent->_actorStatus = getStringFromJson("actorStatus", parentData);
	parent->_pin = getStringFromJson("pinNumber", parentData);
	parent->_anonymous = getStringFromJson("userType", parentData) == "ANONYMOUS";
	
	return parent;
	
}
ParentRef Parent::create()
{
	return ParentRef(new Parent());
}

void Parent::setAvatar(const std::string& avatarUrl)
{
	_avatar = avatarUrl;
}
void Parent::setPin(const std::string& pin)
{
	_pin = pin;
}
void Parent::setDisplayName(const std::string& name)
{
	_displayName = name;
}
void Parent::setId(const std::string& parentId)
{
	_id = parentId;
}
void Parent::setEmail(const std::string& email)
{
	_email = email;
}
void Parent::setActorStatus(const std::string &actorStatus)
{
	_actorStatus = actorStatus;
}

void Parent::setCDNSessionId(const std::string& sessionId)
{
	_cdnSessionId = sessionId;
}
void Parent::setAPISecret(const std::string& apiSecret)
{
	_apiSecret = apiSecret;
}
void Parent::setAPIKey(const std::string& apiKey)
{
	_apiKey = apiKey;
}

void Parent::setCountryCode(const std::string &countryCode)
{
	_countryCode = countryCode;
}

void Parent::setAnonymous(bool anonymous)
{
	_anonymous = anonymous;
}

std::string Parent::getDisplayName() const
{
	return _displayName;
}
std::string Parent::getAvatar() const
{
	return _avatar;
}
std::string Parent::getId() const
{
	return _id;
}
std::string Parent::getPin() const
{
	return _pin;
}
std::string Parent::getEmail() const
{
	return _email;
}
std::string Parent::getActorStatus() const
{
	return _actorStatus;
}

std::string Parent::getCDNSessionId() const
{
	return _cdnSessionId;
}
std::string Parent::getAPISecret() const
{
	return _apiSecret;
}
std::string Parent::getAPIKey() const
{
	return _apiKey;
}

std::string Parent::getCountryCode() const
{
	return _countryCode;
}

bool Parent::isAnonymous() const
{
	return _anonymous;
}


NS_AZOOMEE_END
