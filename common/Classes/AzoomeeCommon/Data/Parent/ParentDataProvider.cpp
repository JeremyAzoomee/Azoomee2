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

std::string ParentDataProvider::getBillingDate()
{
    return ParentDataStorage::getInstance()->loggedInParentBillingDate;
}

std::string ParentDataProvider::getBillingProvider()
{
    return ParentDataStorage::getInstance()->loggedInParentBillingProvider;
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
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    // If this is called before any data is parsed
    if(!parentData->availableChildrenData.IsArray())
    {
        return 0;
    }
    return (int)parentData->availableChildrenData.Size();
}

std::string ParentDataProvider::getProfileNameForAnAvailableChildren(int childNumber)
{
    return ParentDataStorage::getInstance()->availableChildren.at(childNumber)["profileName"];
}
    
std::string ParentDataProvider::getProfileNameForAnAvailableChildrenById(const std::string& childId)
{
    ParentDataStorage* data = ParentDataStorage::getInstance();
    auto it = data->availableChildrenById.find(childId);
    if(it != data->availableChildrenById.end())
    {
        int index = it->second;
        return getProfileNameForAnAvailableChildren(index);
    }
    // No child with childId found
    cocos2d::log("Warning: no child found with id: %s", childId.c_str());
    return "";
}

std::string ParentDataProvider::getAvatarForAnAvailableChildren(int childNumber)
{
    return ParentDataStorage::getInstance()->availableChildren.at(childNumber)["avatar"];
}
    
std::string ParentDataProvider::getAvatarForAnAvailableChildrenById(const std::string& childId)
{
    ParentDataStorage* data = ParentDataStorage::getInstance();
    auto it = data->availableChildrenById.find(childId);
    if(it != data->availableChildrenById.end())
    {
        int index = it->second;
        return getAvatarForAnAvailableChildren(index);
    }
    // No child with childId found
    cocos2d::log("Warning: no child found with id: %s", childId.c_str());
    return "";
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
