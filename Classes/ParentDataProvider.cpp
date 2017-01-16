#include "ParentDataProvider.h"
#include "ParentDataStorage.h"

using namespace cocos2d;

static ParentDataProvider *_sharedParentDataProvider = NULL;

ParentDataProvider* ParentDataProvider::getInstance()
{
    if (! _sharedParentDataProvider)
    {
        _sharedParentDataProvider = new ParentDataProvider();
        _sharedParentDataProvider->init();
    }
    
    return _sharedParentDataProvider;
}

ParentDataProvider::~ParentDataProvider(void)
{
}

bool ParentDataProvider::init(void)
{
    return true;
}

std::string ParentDataProvider::getLoggedInParentActorStatus()
{
    return ParentDataStorage::getInstance()->loggedInParentActorStatus;
}


std::string ParentDataProvider::getLoggedInParentId()
{
    return ParentDataStorage::getInstance()->loggedInParentId;
}


std::string ParentDataProvider::getLoggedInParentApiKey()
{
    return ParentDataStorage::getInstance()->loggedInParentApiKey;
}

//------------------------------------getting information from available children------------------------------------------


std::string ParentDataProvider::getAvailableChildrenValue(std::string keyName)
{
    return ParentDataStorage::getInstance()->availableChildrenData[keyName.c_str()].GetString();
}

int ParentDataProvider::getAmountOfAvailableChildren()
{
    return (int)ParentDataStorage::getInstance()->availableChildrenData.Size();
}

std::string ParentDataProvider::getValueFromOneAvailableChild(int childNumber, std::string keyName)
{
    return ParentDataStorage::getInstance()->availableChildrenData[childNumber][keyName.c_str()].GetString();
}
