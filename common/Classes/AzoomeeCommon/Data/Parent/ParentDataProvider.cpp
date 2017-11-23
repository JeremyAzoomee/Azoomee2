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
    
std::string ParentDataProvider::getLoggedInParentCdnSessionId()
{
    return ParentDataStorage::getInstance()->loggedInParentCdnSessionId;
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
    
std::string ParentDataProvider::getLoggedInParentAvatarId()
{
    return ParentDataStorage::getInstance()->loggedInParentAvatarId;
}
    
std::string ParentDataProvider::getLoggedInParentCountryCode()
{
    return ParentDataStorage::getInstance()->loggedInParentCountryCode;
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
    
bool ParentDataProvider::isLoggedInParentAnonymous()
{
    return ParentDataStorage::getInstance()->isLoggedInParentAnonymous;
}

bool ParentDataProvider::isBillingDataAvailable()
{
    return ParentDataStorage::getInstance()->isBillingDataAvailable;
}
    
bool ParentDataProvider::isPaidUser()
{
    return (ParentDataStorage::getInstance()->loggedInParentBillingStatus == "SUBSCRIBED") || (ParentDataStorage::getInstance()->loggedInParentBillingStatus == "FREE_TRIAL");
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
    
std::string ParentDataProvider::getIDForAvailableChildren(int childNumber)
{
    return ParentDataStorage::getInstance()->availableChildren.at(childNumber)["id"];
}
    
std::string ParentDataProvider::getInviteCodeForAvailableChildren(int childNumber)
{
    return ParentDataStorage::getInstance()->availableChildren.at(childNumber)["inviteCode"];
}
    
//-----------Pending Friend Requests-------------
int ParentDataProvider::getNoOfPendingFriendRequest()
{
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    // If this is called before any data is parsed
    if(!parentData->pendingFriendRequestData.IsArray())
    {
        return 0;
    }
    return (int)parentData->pendingFriendRequestData.Size();
}
    
std::string ParentDataProvider::getPendingFriendRequestSenderName(int pendingFriendRequestNo)
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["senderName"];
}

std::string ParentDataProvider::getPendingFriendRequestFriendName(int pendingFriendRequestNo)
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["friendName"];
}
    
std::string ParentDataProvider::getPendingFriendRequestInviteCode(int pendingFriendRequestNo)
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["inviteeCode"];
}
    
std::string ParentDataProvider::getPendingFriendRequestRequestID(int pendingFriendRequestNo)
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["id"];
}
    
std::string ParentDataProvider::getPendingFriendRequestSenderID(int pendingFriendRequestNo)
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["senderId"];
}

std::string ParentDataProvider::getPendingFriendRequestRespondentID(int pendingFriendRequestNo)
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["respondentId"];
}
    
}
