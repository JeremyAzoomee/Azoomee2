//
//  Parent.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 17/01/2019.
//

#include "Parent.h"

NS_TZ_BEGIN

Parent::Parent()
{
	
}

MutableParentRef MutableParent::createWithJson(const rapidjson::Document& parentData)
{
	MutableParentRef parent = MutableParentRef(new MutableParent());

	parent->_id = getStringFromJson("id", parentData);
	parent->_cdnSessionId = getStringFromJson("cdn-sessionid", parentData);
	parent->_apiSecret = getStringFromJson("apiSecret", parentData);
	parent->_apiKey = getStringFromJson("apiKey", parentData);
	parent->_actorStatus = getStringFromJson("actorStatus", parentData);
	parent->_pin = getStringFromJson("pinNumber", parentData);
	parent->_anonymous = getBoolFromJson("isAnonymous", parentData, false);
	
	return parent;
	
}
MutableParentRef MutableParent::create()
{
	return MutableParentRef(new MutableParent());
}

void MutableParent::setAvatar(const std::string& avatarUrl)
{
	_avatar = avatarUrl;
}
void MutableParent::setPin(const std::string& pin)
{
	_pin = pin;
}
void MutableParent::setDisplayName(const std::string& name)
{
	_displayName = name;
}
void MutableParent::setId(const std::string& parentId)
{
	_id = parentId;
}
void MutableParent::setEmail(const std::string& email)
{
	_email = email;
}
void MutableParent::setActorStatus(const std::string &actorStatus)
{
	_actorStatus = actorStatus;
}

void MutableParent::setCDNSessionId(const std::string& sessionId)
{
	_cdnSessionId = sessionId;
}
void MutableParent::setAPISecret(const std::string& apiSecret)
{
	_apiSecret = apiSecret;
}
void MutableParent::setAPIKey(const std::string& apiKey)
{
	_apiKey = apiKey;
}

void MutableParent::setCountryCode(const std::string &countryCode)
{
	_countryCode = countryCode;
}

void MutableParent::setAnonymous(bool anonymous)
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


NS_TZ_END
