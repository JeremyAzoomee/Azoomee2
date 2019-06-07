#include "ChildDataProvider.h"
#include "ChildDataStorage.h"
#include "../Parent/ParentDataStorage.h"
#include "../../Analytics/AnalyticsSingleton.h"

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
    if(isChildLoggedIn())
    {
        return ChildDataStorage::getInstance()->getLoggedInChild()->getId();
    }
    else
    {
        return ParentDataStorage::getInstance()->getParent()->getId();
    }
}

std::string ChildDataProvider::getParentOrChildCdnSessionId() const
{
    if(isChildLoggedIn())
    {
        return ChildDataStorage::getInstance()->getLoggedInChild()->getCDNSessionId();
    }
    else
    {
        return ParentDataStorage::getInstance()->getParent()->getCDNSessionId();
    }
}

std::string ChildDataProvider::getParentOrChildApiSecret() const
{
    if(isChildLoggedIn())
    {
        return ChildDataStorage::getInstance()->getLoggedInChild()->getAPISecret();
    }
    else
    {
        return ParentDataStorage::getInstance()->getParent()->getAPISecret();
    }
}

std::string ChildDataProvider::getParentOrChildApiKey() const
{
    if(isChildLoggedIn())
    {
        return ChildDataStorage::getInstance()->getLoggedInChild()->getAPIKey();
    }
    else
    {
        return ParentDataStorage::getInstance()->getParent()->getAPIKey();
    }
}
    
std::string ChildDataProvider::getParentOrChildAvatarId() const
{
    if(isChildLoggedIn())
    {
        return ChildDataStorage::getInstance()->getLoggedInChild()->getAvatar();
    }
    else
    {
        return ParentDataStorage::getInstance()->getParent()->getAvatar();
    }
}

std::string ChildDataProvider::getParentOrChildName() const
{
	if(isChildLoggedIn())
	{
		return ChildDataStorage::getInstance()->getLoggedInChild()->getProfileName();
	}
	else
	{
		return ParentDataStorage::getInstance()->getParent()->getDisplayName();
	}
}

bool ChildDataProvider::isChildLoggedIn() const
{
	if(!ChildDataStorage::getInstance()->isChildLoggedIn() && ChildDataStorage::getInstance()->getLoggedInChild())
	{
		AnalyticsSingleton::getInstance()->debugEvent("childLoginFlagFalse", {{"parentId",ParentDataStorage::getInstance()->getParent()->getId()},{"childId",ChildDataStorage::getInstance()->getLoggedInChild()->getId()}});
	}
	return ChildDataStorage::getInstance()->isChildLoggedIn() && ChildDataStorage::getInstance()->getLoggedInChild();
}

ChildRef ChildDataProvider::getLoggedInChild() const
{
	return ChildDataStorage::getInstance()->getLoggedInChild();
}
	
}
