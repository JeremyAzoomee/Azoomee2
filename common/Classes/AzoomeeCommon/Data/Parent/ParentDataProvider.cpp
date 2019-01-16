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

std::string ParentDataProvider::getLoggedInParentActorStatus() const
{
    return ParentDataStorage::getInstance()->loggedInParentActorStatus;
}


std::string ParentDataProvider::getLoggedInParentId() const
{
    return ParentDataStorage::getInstance()->loggedInParentId;
}
    
std::string ParentDataProvider::getLoggedInParentCdnSessionId() const
{
    return ParentDataStorage::getInstance()->loggedInParentCdnSessionId;
}


std::string ParentDataProvider::getLoggedInParentApiKey() const
{
    return ParentDataStorage::getInstance()->loggedInParentApiKey;
}

std::string ParentDataProvider::getParentPin() const
{
    return ParentDataStorage::getInstance()->loggedInParentPin;
}

std::string ParentDataProvider::getLoggedInParentApiSecret() const
{
    return ParentDataStorage::getInstance()->loggedInParentApiSecret;
}
    
std::string ParentDataProvider::getLoggedInParentAvatarId() const
{
    return ParentDataStorage::getInstance()->loggedInParentAvatarId;
}

std::string ParentDataProvider::getParentEmail() const
{
    return ParentDataStorage::getInstance()->loggedInParentEmail;
}
    
std::string ParentDataProvider::getParentDisplayName() const
{
    return ParentDataStorage::getInstance()->loggedInParentDisplayName;
}

std::string ParentDataProvider::getLoggedInParentCountryCode() const
{
    return ParentDataStorage::getInstance()->loggedInParentCountryCode;
}

std::string ParentDataProvider::getBillingStatus() const
{
	if(ParentDataStorage::getInstance()->_billingData)
	{
    	return ParentDataStorage::getInstance()->_billingData->getBillingStatusStr();
	}
	return "";
}

std::string ParentDataProvider::getBillingDate() const
{
	if(ParentDataStorage::getInstance()->_billingData)
	{
    	return ParentDataStorage::getInstance()->_billingData->getNextBillDate();
	}
	return "";
}

std::string ParentDataProvider::getBillingProvider() const
{
	if(ParentDataStorage::getInstance()->_billingData)
	{
    	return ParentDataStorage::getInstance()->_billingData->getPaymentProvider();
	}
	return "";
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
	if(ParentDataStorage::getInstance()->_billingData)
	{
    	return (ParentDataStorage::getInstance()->_billingData->getBillingStatusStr() == "SUBSCRIBED") || (ParentDataStorage::getInstance()->_billingData->getBillingStatusStr() == "FREE_TRIAL");
	}
	return false;
}

bool ParentDataProvider::emailRequiresVerification()
{
    return (ParentDataStorage::getInstance()->loggedInParentActorStatus != "VERIFIED") && (ParentDataStorage::getInstance()->loggedInParentActorStatus != "ACTIVE");
}
    
bool ParentDataProvider::isUserLoggedIn()
{
    return (!isLoggedInParentAnonymous() && getLoggedInParentId() != "");
}

//------------------------------------getting information from available children------------------------------------------

ChildRef ParentDataProvider::getChildForId(const std::string& childId) const
{
	auto childMap = ParentDataStorage::getInstance()->_availableChildrenById;
	if(childMap.find(childId) != childMap.end())
	{
		return childMap.at(childId);
	}
	return nullptr;
}
ChildRef ParentDataProvider::getChild(int childNumber) const
{
	auto childList = ParentDataStorage::getInstance()->_availableChildren;
	if(childNumber < childList.size())
	{
		return childList.at(childNumber);
	}
	return nullptr;
}
	

int ParentDataProvider::getAmountOfAvailableChildren()
{
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
	return (int)parentData->_availableChildren.size();
    // If this is called before any data is parsed
    //if(!parentData->availableChildrenData.IsArray())
    //{
    //    return 0;
    //}
    //return (int)parentData->availableChildrenData.Size();
}

std::string ParentDataProvider::getProfileNameForAnAvailableChild(int childNumber) const
{
    return ParentDataStorage::getInstance()->_availableChildren.at(childNumber)->getProfileName();
}
    
std::string ParentDataProvider::getProfileNameForAnAvailableChildById(const std::string& childId) const
{
    ParentDataStorage* data = ParentDataStorage::getInstance();
    auto it = data->_availableChildrenById.find(childId);
    if(it != data->_availableChildrenById.end())
    {
		return it->second->getProfileName();
    }
    // No child with childId found
    cocos2d::log("Warning: no child found with id: %s", childId.c_str());
    return "";
}

std::string ParentDataProvider::getAvatarForAnAvailableChild(int childNumber) const
{
    return ParentDataStorage::getInstance()->_availableChildren.at(childNumber)->getAvatar();
}
    
std::string ParentDataProvider::getAvatarForAnAvailableChildById(const std::string& childId) const
{
    ParentDataStorage* data = ParentDataStorage::getInstance();
    auto it = data->_availableChildrenById.find(childId);
    if(it != data->_availableChildrenById.end())
    {
		return it->second->getAvatar();
    }
    // No child with childId found
    cocos2d::log("Warning: no child found with id: %s", childId.c_str());
    return "";
}

std::string ParentDataProvider::getDOBForAnAvailableChild(int childNumber) const
{
    return ParentDataStorage::getInstance()->_availableChildren.at(childNumber)->getDOB();
}

std::string ParentDataProvider::getSexForAnAvailableChild(int childNumber) const
{
    return ParentDataStorage::getInstance()->_availableChildren.at(childNumber)->getSex();
}
    
std::string ParentDataProvider::getIDForAvailableChildren(int childNumber) const
{
    return ParentDataStorage::getInstance()->_availableChildren.at(childNumber)->getId();
}
    
std::string ParentDataProvider::getInviteCodeForAnAvailableChild(int childNumber) const
{
    return ParentDataStorage::getInstance()->_availableChildren.at(childNumber)->getInviteCode();
}
    
std::string ParentDataProvider::getInviteCodeForChildName(const std::string& name) const
{
    for(auto child : ParentDataStorage::getInstance()->_availableChildren)
    {
        if(child->getProfileName() == name)
        {
            return child->getInviteCode();
        }
    }
    return "";
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
    
std::string ParentDataProvider::getPendingFriendRequestSenderName(int pendingFriendRequestNo) const
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["senderName"];
}

std::string ParentDataProvider::getPendingFriendRequestFriendName(int pendingFriendRequestNo) const
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["friendName"];
}
    
std::string ParentDataProvider::getPendingFriendRequestInviteCode(int pendingFriendRequestNo) const
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["inviteeCode"];
}
    
std::string ParentDataProvider::getPendingFriendRequestRequestID(int pendingFriendRequestNo) const
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["id"];
}
    
std::string ParentDataProvider::getPendingFriendRequestSenderID(int pendingFriendRequestNo) const
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["senderId"];
}

std::string ParentDataProvider::getPendingFriendRequestRespondentID(int pendingFriendRequestNo) const
{
    return ParentDataStorage::getInstance()->pendingFriendRequests.at(pendingFriendRequestNo)["respondentId"];
}
    
}
