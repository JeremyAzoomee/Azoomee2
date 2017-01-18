#include "ChildDataProvider.h"
#include "ChildDataStorage.h"
#include "ParentDataStorage.h"

using namespace cocos2d;

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

std::string ChildDataProvider::getParentOrChildId()
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

std::string ChildDataProvider::getParentOrChildCdnSessionId()
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

std::string ChildDataProvider::getParentOrChildApiSecret()
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

std::string ChildDataProvider::getParentOrChildApiKey()
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

std::string ChildDataProvider::getLoggedInChildName()
{
    return ChildDataStorage::getInstance()->loggedInChildName;
}

std::string ChildDataProvider::getLoggedInChildId()
{
    return ChildDataStorage::getInstance()->loggedInChildId;
}

std::string ChildDataProvider::getLoggedInChildAvatarId()
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
