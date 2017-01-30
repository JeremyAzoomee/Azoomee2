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

std::string ParentDataProvider::getParentPin()
{
    return ParentDataStorage::getInstance()->loggedInParentPin;
}

//------------------------------------getting information from available children------------------------------------------


int ParentDataProvider::getAmountOfAvailableChildren()
{
    return (int)ParentDataStorage::getInstance()->availableChildrenData.Size();
}

std::string ParentDataProvider::getProfileNameForAnAvailableChildren(int childNumber)
{
    return ParentDataStorage::getInstance()->availableChildren.at(childNumber)["profileName"];
}

std::string ParentDataProvider::getAvatarForAnAvailableChildren(int childNumber)
{
    return ParentDataStorage::getInstance()->availableChildren.at(childNumber)["avatar"];
}
