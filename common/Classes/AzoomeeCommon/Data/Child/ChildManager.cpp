//
//  ChildManager.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 05/06/2019.
//

#include "ChildManager.h"
#include "../Parent/ParentManager.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../../Crashlytics/CrashlyticsConfig.h"

NS_AZOOMEE_BEGIN

using namespace cocos2d;

static std::auto_ptr<ChildManager> sChildManagerSharedInstance;

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

std::string ChildManager::getParentOrChildId() const
{
	if(isChildLoggedIn())
	{
		return _loggedInChild->getId();
	}
	else
	{
		return ParentManager::getInstance()->getParent()->getId();
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
		return ParentManager::getInstance()->getParent()->getCDNSessionId();
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
		return ParentManager::getInstance()->getParent()->getAPISecret();
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
		return ParentManager::getInstance()->getParent()->getAPIKey();
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
		return ParentManager::getInstance()->getParent()->getAvatar();
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
		return ParentManager::getInstance()->getParent()->getDisplayName();
	}
}

bool ChildManager::isChildLoggedIn() const
{
	if(!_childLoggedIn && _loggedInChild)
	{
		AnalyticsSingleton::getInstance()->debugEvent("childLoginFlagFalse", {{"parentId",ParentManager::getInstance()->getParent()->getId()},{"childId",_loggedInChild->getId()}});
	}
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

void ChildManager::setLoggedInChild(const ChildRef &child)
{
	_loggedInChild = child;
}

bool ChildManager::parseChildLoginData(const std::string &responseData)
{
	rapidjson::Document data;
	data.Parse(responseData.c_str());
	if(data.HasParseError())
	{
		return false;
	}
	
	ChildRef child = ParentManager::getInstance()->getChildForId(getStringFromJson("id", data));
	if(!child)
	{
		return false;
	}
	
	child->parseLoginData(data);
	
	_loggedInChild = child;
	
	UserDefault* def = UserDefault::getInstance();
	def->setStringForKey("lastLoggedInChildId", child->getId());
	def->flush();
	
	createCrashlyticsUserInfo(ParentManager::getInstance()->getLoggedInParentId(), child->getId());
	
	_childLoggedIn = true;
	return true;
}

void ChildManager::parseChildInventory(const std::string &inventoryData)
{
	rapidjson::Document data;
	data.Parse(inventoryData.c_str());
	if(data.HasParseError())
	{
		return;
	}
	
	ChildRef child = _loggedInChild;
	if(!child)
	{
		return;
	}
	
	InventoryRef inventory = Inventory::createWithJson(data);
	
	child->setInventory(inventory);
	
}

void ChildManager::loginChildOffline(const std::string &childId)
{
	_childLoggedIn = true;
	ChildRef offlineChild = Child::create();
	offlineChild->setId(childId);
	_loggedInChild = offlineChild;
}


NS_AZOOMEE_END
