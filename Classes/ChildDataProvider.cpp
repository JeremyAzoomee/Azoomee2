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

std::string ChildDataProvider::getParentOrChildLoginValue(std::string keyName) //if no child logged in, falls back to parent data
{
    if(ChildDataStorage::getInstance()->childLoggedIn)
    {
        return ChildDataStorage::getInstance()->childLoginData[keyName.c_str()].GetString();
    }
    else
    {
        return ParentDataStorage::getInstance()->parentLoginData[keyName.c_str()].GetString();
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
