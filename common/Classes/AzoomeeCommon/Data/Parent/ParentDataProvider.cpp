#include "ParentDataProvider.h"
#include "ParentDataStorage.h"


namespace Azoomee
{

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

std::string ParentDataProvider::getLoggedInParentApiSecret()
{
    return ParentDataStorage::getInstance()->loggedInParentApiSecret;
}

std::string ParentDataProvider::getBillingStatus()
{
    return ParentDataStorage::getInstance()->loggedInParentBillingStatus;
}

bool ParentDataProvider::isPaidUser()
{
    return ParentDataStorage::getInstance()->loggedInParentBillingStatus == "SUBSCRIBED";
}

bool ParentDataProvider::emailRequiresVerification()
{
    return (ParentDataStorage::getInstance()->loggedInParentActorStatus != "VERIFIED") && (ParentDataStorage::getInstance()->loggedInParentActorStatus != "ACTIVE");
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

std::string ParentDataProvider::getDOBForAnAvailableChildren(int childNumber)
{
    return ParentDataStorage::getInstance()->availableChildren.at(childNumber)["dob"];
}

std::string ParentDataProvider::getSexForAnAvailableChildren(int childNumber)
{
    return ParentDataStorage::getInstance()->availableChildren.at(childNumber)["sex"];
}
  
}
