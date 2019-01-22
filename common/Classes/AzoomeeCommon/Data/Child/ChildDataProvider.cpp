#include "ChildDataProvider.h"
#include "ChildDataStorage.h"
#include "../Parent/ParentDataStorage.h"

using namespace cocos2d;
using namespace Azoomee;

namespace Azoomee
{

static ChildDataProvider *_sharedChildDataProvider = NULL;

ChildDataProvider* ChildDataProvider::getInstance()
{
    if (! _sharedChildDataProvider)
    {
        _sharedChildDataProvider = new ChildDataProvider();
        _sharedChildDataProvider->init();
    }
    
    return _sharedChildDataProvider;
}

ChildDataProvider::~ChildDataProvider(void)
{
}

bool ChildDataProvider::init(void)
{
    return true;
}

std::string ChildDataProvider::getParentOrChildId() const
{
    if(getIsChildLoggedIn())
    {
        return ChildDataStorage::getInstance()->_loggedInChild->getId();
    }
    else
    {
        return ParentDataStorage::getInstance()->_parent->getId();
    }
}

std::string ChildDataProvider::getParentOrChildCdnSessionId() const
{
    if(getIsChildLoggedIn())
    {
        return ChildDataStorage::getInstance()->_loggedInChild->getCDNSessionId();
    }
    else
    {
        return ParentDataStorage::getInstance()->_parent->getCDNSessionId();
    }
}

std::string ChildDataProvider::getParentOrChildApiSecret() const
{
    if(getIsChildLoggedIn())
    {
        return ChildDataStorage::getInstance()->_loggedInChild->getAPISecret();
    }
    else
    {
        return ParentDataStorage::getInstance()->_parent->getAPISecret();
    }
}

std::string ChildDataProvider::getParentOrChildApiKey() const
{
    if(getIsChildLoggedIn())
    {
        return ChildDataStorage::getInstance()->_loggedInChild->getAPIKey();
    }
    else
    {
        return ParentDataStorage::getInstance()->_parent->getAPIKey();
    }
}
    
std::string ChildDataProvider::getParentOrChildAvatarId() const
{
    if(getIsChildLoggedIn())
    {
        return ChildDataStorage::getInstance()->_loggedInChild->getAvatar();
    }
    else
    {
        return ParentDataStorage::getInstance()->_parent->getAvatar();
    }
}

std::string ChildDataProvider::getParentOrChildName() const
{
	if(getIsChildLoggedIn())
	{
		return ChildDataStorage::getInstance()->_loggedInChild->getProfileName();
	}
	else
	{
		return ParentDataStorage::getInstance()->_parent->getDisplayName();
	}
}
/*
std::string ChildDataProvider::getLoggedInChildName() const
{
	if(ChildDataStorage::getInstance()->_loggedInChild)
	{
    	return ChildDataStorage::getInstance()->_loggedInChild->getProfileName();
	}
	return "";
}

std::string ChildDataProvider::getLoggedInChildId() const
{
	if(ChildDataStorage::getInstance()->_loggedInChild)
	{
    	return ChildDataStorage::getInstance()->_loggedInChild->getId();
	}
	return "";
}

std::string ChildDataProvider::getLoggedInChildAvatarId() const
{
	if(ChildDataStorage::getInstance()->_loggedInChild)
	{
    	return ChildDataStorage::getInstance()->_loggedInChild->getAvatar();
	}
	return "";
}
*/
bool ChildDataProvider::getIsChildLoggedIn() const
{
	return ChildDataStorage::getInstance()->childLoggedIn && ChildDataStorage::getInstance()->_loggedInChild;
}

ChildRef ChildDataProvider::getLoggedInChild() const
{
	return ChildDataStorage::getInstance()->_loggedInChild;
}

}
