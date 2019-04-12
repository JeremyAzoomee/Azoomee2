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
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->getActorStatus();
	}
	return "";
}


std::string ParentDataProvider::getLoggedInParentId() const
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->getId();
	}
	return "";
}
    
std::string ParentDataProvider::getLoggedInParentCdnSessionId() const
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->getCDNSessionId();
	}
	return "";
}


std::string ParentDataProvider::getLoggedInParentApiKey() const
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->getAPIKey();
	}
	return "";
}

std::string ParentDataProvider::getParentPin() const
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->getPin();
	}
	return "";
}

std::string ParentDataProvider::getLoggedInParentApiSecret() const
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->getAPISecret();
	}
	return "";
}
    
std::string ParentDataProvider::getLoggedInParentAvatarId() const
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->getAvatar();
	}
	return "";
}

std::string ParentDataProvider::getParentEmail() const
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->getEmail();
	}
	return "";
}
    
std::string ParentDataProvider::getParentDisplayName() const
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->getDisplayName();
	}
	return "";
}

std::string ParentDataProvider::getLoggedInParentCountryCode() const
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->getCountryCode();
	}
	return "";
}

std::string ParentDataProvider::getBillingStatus() const
{
	if(ParentDataStorage::getInstance()->getBillingData())
	{
    	return ParentDataStorage::getInstance()->getBillingData()->getBillingStatusStr();
	}
	return "";
}

std::string ParentDataProvider::getBillingDate() const
{
	if(ParentDataStorage::getInstance()->getBillingData())
	{
    	return ParentDataStorage::getInstance()->getBillingData()->getNextBillDate();
	}
	return "";
}

std::string ParentDataProvider::getBillingProvider() const
{
	if(ParentDataStorage::getInstance()->getBillingData())
	{
    	return ParentDataStorage::getInstance()->getBillingData()->getPaymentProvider();
	}
	return "";
}
    
bool ParentDataProvider::isLoggedInParentAnonymous()
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return ParentDataStorage::getInstance()->getParent()->isAnonymous();
	}
	return true;
}

bool ParentDataProvider::isBillingDataAvailable()
{
    return ParentDataStorage::getInstance()->isBillingDataAvailable();
}
    
bool ParentDataProvider::isPaidUser()
{
	if(ParentDataStorage::getInstance()->getBillingData())
	{
    	return (ParentDataStorage::getInstance()->getBillingData()->getBillingStatusStr() == "SUBSCRIBED") || (ParentDataStorage::getInstance()->getBillingData()->getBillingStatusStr() == "FREE_TRIAL");
	}
	return false;
}

bool ParentDataProvider::emailRequiresVerification()
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	return (ParentDataStorage::getInstance()->getParent()->getActorStatus() != "VERIFIED") && (ParentDataStorage::getInstance()->getParent()->getActorStatus() != "ACTIVE");
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
	auto childMap = ParentDataStorage::getInstance()->getAvailableChildrenById();
	if(childMap.find(childId) != childMap.end())
	{
		return childMap.at(childId);
	}
	return nullptr;
}
ChildRef ParentDataProvider::getChild(int childNumber) const
{
	auto childList = ParentDataStorage::getInstance()->getAvailableChildren();
	if(childNumber < childList.size())
	{
		return childList.at(childNumber);
	}
	return nullptr;
}
	

int ParentDataProvider::getAmountOfAvailableChildren()
{
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
	return (int)parentData->getAvailableChildren().size();
}

//-----------Pending Friend Requests-------------
int ParentDataProvider::getNoOfPendingFriendRequest()
{
	return (int)ParentDataStorage::getInstance()->getPendingFriendRequests().size();
}
	
FriendRequestRef ParentDataProvider::getPendingFriendRequest(int pendingFriendRequestNo) const
{
	if(pendingFriendRequestNo >= ParentDataStorage::getInstance()->getPendingFriendRequests().size())
	{
		return nullptr;
	}
	return ParentDataStorage::getInstance()->getPendingFriendRequests().at(pendingFriendRequestNo);
}

}
