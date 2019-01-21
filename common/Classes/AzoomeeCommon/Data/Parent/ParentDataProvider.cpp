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
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->getActorStatus();
	}
	return "";
}


std::string ParentDataProvider::getLoggedInParentId() const
{
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->getId();
	}
	return "";
}
    
std::string ParentDataProvider::getLoggedInParentCdnSessionId() const
{
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->getCDNSessionId();
	}
	return "";
}


std::string ParentDataProvider::getLoggedInParentApiKey() const
{
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->getAPIKey();
	}
	return "";
}

std::string ParentDataProvider::getParentPin() const
{
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->getPin();
	}
	return "";
}

std::string ParentDataProvider::getLoggedInParentApiSecret() const
{
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->getAPISecret();
	}
	return "";
}
    
std::string ParentDataProvider::getLoggedInParentAvatarId() const
{
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->getAvatar();
	}
	return "";
}

std::string ParentDataProvider::getParentEmail() const
{
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->getEmail();
	}
	return "";
}
    
std::string ParentDataProvider::getParentDisplayName() const
{
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->getDisplayName();
	}
	return "";
}

std::string ParentDataProvider::getLoggedInParentCountryCode() const
{
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->getCountryCode();
	}
	return "";
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
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return ParentDataStorage::getInstance()->_parent->isAnonymous();
	}
	return true;
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
	if(ParentDataStorage::getInstance()->_parent)
	{
    	return (ParentDataStorage::getInstance()->_parent->getActorStatus() != "VERIFIED") && (ParentDataStorage::getInstance()->_parent->getActorStatus() != "ACTIVE");
	}
	return false;
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
}

//-----------Pending Friend Requests-------------
int ParentDataProvider::getNoOfPendingFriendRequest()
{
	return (int)ParentDataStorage::getInstance()->_pendingFriendRequests.size();
}
	
FriendRequestRef ParentDataProvider::getPendingFriendRequest(int pendingFriendRequestNo) const
{
	if(pendingFriendRequestNo >= ParentDataStorage::getInstance()->_pendingFriendRequests.size())
	{
		return nullptr;
	}
	return ParentDataStorage::getInstance()->_pendingFriendRequests.at(pendingFriendRequestNo);
}

}
