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
    if(ChildDataStorage::getInstance()->childLoggedIn)
    {
        return ChildDataStorage::getInstance()->loggedInChildId;
    }
    else
    {
        return ParentDataStorage::getInstance()->loggedInParentId;
    }
}

std::string ChildDataProvider::getParentOrChildCdnSessionId() const
{
    if(ChildDataStorage::getInstance()->childLoggedIn)
    {
        return ChildDataStorage::getInstance()->loggedInChildCdnSessionId;
    }
    else
    {
        return ParentDataStorage::getInstance()->loggedInParentCdnSessionId;
    }
}

std::string ChildDataProvider::getParentOrChildApiSecret() const
{
    if(ChildDataStorage::getInstance()->childLoggedIn)
    {
        return ChildDataStorage::getInstance()->loggedInChildApiSecret;
    }
    else
    {
        return ParentDataStorage::getInstance()->loggedInParentApiSecret;
    }
}

std::string ChildDataProvider::getParentOrChildApiKey() const
{
    if(ChildDataStorage::getInstance()->childLoggedIn)
    {
        return ChildDataStorage::getInstance()->loggedInChildApiKey;
    }
    else
    {
        return ParentDataStorage::getInstance()->loggedInParentApiKey;
    }
}
    
std::string ChildDataProvider::getParentOrChildAvatarId() const
{
    if(ChildDataStorage::getInstance()->childLoggedIn)
    {
        return ChildDataStorage::getInstance()->loggedInChildAvatarId;
    }
    else
    {
        return ParentDataStorage::getInstance()->loggedInParentAvatarId;
    }
}

std::string ChildDataProvider::getLoggedInChildName() const
{
    return ChildDataStorage::getInstance()->loggedInChildName;
}

std::string ChildDataProvider::getLoggedInChildId() const
{
    return ChildDataStorage::getInstance()->loggedInChildId;
}

std::string ChildDataProvider::getLoggedInChildAvatarId() const
{
    return ChildDataStorage::getInstance()->loggedInChildAvatarId;
}

int ChildDataProvider::getLoggedInChildNumber()
{
    return ChildDataStorage::getInstance()->loggedInChildNumber;
}

bool ChildDataProvider::getIsChildLoggedIn()
{
    return ChildDataStorage::getInstance()->childLoggedIn;
}
  
}
