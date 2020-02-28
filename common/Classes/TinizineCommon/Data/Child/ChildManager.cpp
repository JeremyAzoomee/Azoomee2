//
//  ChildManager.cpp
//  TinizineCommon
//
//  Created by Macauley on 05/06/2019.
//

#include "ChildManager.h"
#include "../Parent/UserAccountManager.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../../Crashlytics/CrashlyticsConfig.h"
#include "../../API/API.h"

NS_TZ_BEGIN

using namespace cocos2d;

static std::auto_ptr<ChildManager> sChildManagerSharedInstance;
const std::string ChildManager::kInventoryUpdatedEvent = "Azoomee_ChildManager_kInventoryUpdatedEvent";

ChildManager* ChildManager::getInstance()
{
	if(!sChildManagerSharedInstance.get())
	{
		sChildManagerSharedInstance.reset(new ChildManager());
	}
	return sChildManagerSharedInstance.get();
}

ChildManager::~ChildManager()
{
	
}
/*
std::string ChildManager::getParentOrChildId() const
{
	if(isChildLoggedIn())
	{
		return _loggedInChild->getId();
	}
	else
	{
		return UserAccountManager::getInstance()->getParent()->getId();
	}
}

std::string ChildManager::getParentOrChildCdnSessionId() const
{
	if(isChildLoggedIn())
	{
		return _loggedInChild->getCDNSessionId();
	}
	else
	{
		return UserAccountManager::getInstance()->getParent()->getCDNSessionId();
	}
}

std::string ChildManager::getParentOrChildApiSecret() const
{
	if(isChildLoggedIn())
	{
		return _loggedInChild->getAPISecret();
	}
	else
	{
		return UserAccountManager::getInstance()->getParent()->getAPISecret();
	}
}

std::string ChildManager::getParentOrChildApiKey() const
{
	if(isChildLoggedIn())
	{
		return _loggedInChild->getAPIKey();
	}
	else
	{
		return UserAccountManager::getInstance()->getParent()->getAPIKey();
	}
}

std::string ChildManager::getParentOrChildAvatarId() const
{
	if(isChildLoggedIn())
	{
		return _loggedInChild->getAvatar();
	}
	else
	{
		return UserAccountManager::getInstance()->getParent()->getAvatar();
	}
}

std::string ChildManager::getParentOrChildName() const
{
	if(isChildLoggedIn())
	{
		return _loggedInChild->getProfileName();
	}
	else
	{
		return UserAccountManager::getInstance()->getParent()->getDisplayName();
	}
}
*/
bool ChildManager::isChildLoggedIn() const
{
	return _childLoggedIn && _loggedInChild;
}

ChildRef ChildManager::getLoggedInChild() const
{
	return _loggedInChild;
}

void ChildManager::setChildLoggedIn(bool loggedIn)
{
	_childLoggedIn = loggedIn;
}

void ChildManager::setLoggedInChild(const MutableChildRef &child)
{
	_loggedInChild = child;
}

void ChildManager::updateInventory()
{
    CCASSERT(_loggedInChild, "Tried to update inventory when no child is logged in");

    auto onSuccess = [this](const std::string& requestTag, const std::string& headers, const std::string& body) {
        parseChildInventory(body);
    };
    
    HttpRequestCreator* request = API::GetInventory(_loggedInChild->getId(), onSuccess, nullptr);
    request->execute();
}

void ChildManager::parseChildInventory(const std::string &inventoryData)
{
    // Ignore if we're no longer logged in
    if(!_loggedInChild)
    {
        return;
    }
    
	rapidjson::Document data;
	data.Parse(inventoryData.c_str());
	if(data.HasParseError())
	{
		return;
	}
	
	const InventoryRef& inventory = Inventory::createWithJson(data);
	_loggedInChild->setInventory(inventory);
    
    // Post event to notify
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kInventoryUpdatedEvent);
}

void ChildManager::parseAvatarUpdate(const std::string& avatarData)
{
	rapidjson::Document json;
	json.Parse(avatarData.c_str());
	if(!json.HasParseError() && _loggedInChild)
	{
		_loggedInChild->setAvatar(getStringFromJson("avatar", json));
	}
}

void ChildManager::parseChildSessionUpdate(const std::string &sessionData)
{
	rapidjson::Document json;
	json.Parse(sessionData.c_str());
	if(!json.HasParseError() && _loggedInChild)
	{
		_loggedInChild->setCDNSessionId(getStringFromJson("sessionId", json), std::chrono::milliseconds(getIntFromJson("expiry", json)));
	}
}

void ChildManager::loginChildOffline(const std::string &childId)
{
	_childLoggedIn = true;
	MutableChildRef offlineChild = MutableChild::create();
	offlineChild->setId(childId);
	_loggedInChild = offlineChild;
}


NS_TZ_END
