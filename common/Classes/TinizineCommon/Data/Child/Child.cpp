//
//  Child.cpp
//  TinizineCommon
//
//  Created by Macauley on 16/01/2019.
//

#include "Child.h"
#include "../../Utils/TimeUtils.h"

NS_TZ_BEGIN

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

InventoryRef Child::getInventory() const
{
	return _inventory;
}

cocos2d::Color4B Child::getAvatarColour() const
{
    return _avatarColour;
}

bool Child::isSessionExpired() const
{
	return _sessionExpiryTimestamp < std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

MutableChildRef MutableChild::create()
{
	return MutableChildRef(new MutableChild());
}

MutableChildRef MutableChild::createWithJson(const rapidjson::Value& childData)
{
	MutableChildRef child = MutableChildRef(new MutableChild());
	
	child->parseChildData(childData);
	
	child->setInventory(Inventory::create());
	
	return child;
}

void MutableChild::parseLoginData(const rapidjson::Document& loginData)
{
	if(loginData.HasParseError())
	{
		return;
	}
	
	_cdnSessionId = getStringFromJson("cdn-sessionid", loginData);
	_apiSecret = getStringFromJson("apiSecret", loginData);
	_apiKey = getStringFromJson("apiKey", loginData);
	_sessionExpiryTimestamp = TimeUtils::getCurrentTimeMillis() + std::chrono::milliseconds(getIntFromJson("cdn-expiry", loginData));
}

void MutableChild::parseChildData(const rapidjson::Value& childData)
{
	_profileName = getStringFromJson("profileName", childData);
	_avatar = getStringFromJson("avatar", childData);
	_inviteCode = getStringFromJson("inviteCode", childData);
	_sex = getStringFromJson("sex", childData);
	_dob = getStringFromJson("dob", childData);
	_id = getStringFromJson("id", childData);
}

void MutableChild::setInventory(const InventoryRef &inventory)
{
	_inventory = inventory;
}

void MutableChild::setAvatar(const std::string &avatarUrl)
{
	_avatar = avatarUrl;
}

void MutableChild::setId(const std::string& childId)
{
	_id = childId;
}
void MutableChild::setDOB(const std::string& dob)
{
	_dob = dob;
}
void MutableChild::setProfileName(const std::string& name)
{
	_profileName = name;
}

void MutableChild::setCDNSessionId(const std::string &sessionId, std::chrono::milliseconds sessionDurationMillis)
{
	_cdnSessionId = sessionId;
	_sessionExpiryTimestamp = TimeUtils::getCurrentTimeMillis() + std::chrono::milliseconds(sessionDurationMillis);
}

void MutableChild::setAvatarColour(const cocos2d::Color4B &colour)
{
    _avatarColour = colour;
}

NS_TZ_END
