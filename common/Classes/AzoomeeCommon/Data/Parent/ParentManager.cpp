//
//  ParentManager.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 06/06/2019.
//

#include "ParentManager.h"
#include "../Child/ChildManager.h"
#include "../../Crashlytics/CrashlyticsConfig.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../../Utils/PushNotificationsHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<ParentManager> sParentManagerSharedInstance;

ParentManager* ParentManager::getInstance()
{
	if(!sParentManagerSharedInstance.get())
	{
		sParentManagerSharedInstance.reset(new ParentManager());
	}
	return sParentManagerSharedInstance.get();
}

ParentManager::~ParentManager()
{
	
}

void ParentManager::setBillingData(const BillingDataRef& billingData)
{
	_billingData = billingData;
}
BillingDataRef ParentManager::getBillingData() const
{
	return _billingData;
}
void ParentManager::setBillingDataAvailable(bool available)
{
	_isBillingDataAvailable = available;
}
bool ParentManager::isBillingDataAvailable() const
{
	return _isBillingDataAvailable;
}

void ParentManager::setParent(const MutableParentRef& parent)
{
	_parent = parent;
}
ParentRef ParentManager::getParent() const
{
	return _parent;
}

void ParentManager::clearAvailableChildren()
{
	_availableChildren.clear();
	_availableChildrenById.clear();
}
void ParentManager::addChild(const MutableChildRef& child)
{
	_availableChildren.push_back(child);
	_availableChildrenById[child->getId()] = child;
}

/*std::vector<ChildRef> ParentManager::getAvailableChildren() const
{
	return _availableChildren;
}
std::map<std::string, ChildRef> ParentManager::getAvailableChildrenById() const
{
	return _availableChildrenById;
}*/

void ParentManager::setPendingFriendRequests(const std::vector<FriendRequestRef>& pendingRequests)
{
	_pendingFriendRequests = pendingRequests;
}
std::vector<FriendRequestRef> ParentManager::getPendingFriendRequests() const
{
	return _pendingFriendRequests;
}

std::string ParentManager::getLoggedInParentActorStatus() const
{
	if(_parent)
	{
		return _parent->getActorStatus();
	}
	return "";
}


std::string ParentManager::getLoggedInParentId() const
{
	if(_parent)
	{
		return _parent->getId();
	}
	return "";
}

std::string ParentManager::getLoggedInParentCdnSessionId() const
{
	if(_parent)
	{
		return _parent->getCDNSessionId();
	}
	return "";
}


std::string ParentManager::getLoggedInParentApiKey() const
{
	if(_parent)
	{
		return _parent->getAPIKey();
	}
	return "";
}

std::string ParentManager::getParentPin() const
{
	if(_parent)
	{
		return _parent->getPin();
	}
	return "";
}

std::string ParentManager::getLoggedInParentApiSecret() const
{
	if(_parent)
	{
		return _parent->getAPISecret();
	}
	return "";
}

std::string ParentManager::getLoggedInParentAvatarId() const
{
	if(_parent)
	{
		return _parent->getAvatar();
	}
	return "";
}

std::string ParentManager::getParentEmail() const
{
	if(_parent)
	{
		return _parent->getEmail();
	}
	return "";
}

std::string ParentManager::getParentDisplayName() const
{
	if(_parent)
	{
		return _parent->getDisplayName();
	}
	return "";
}

std::string ParentManager::getLoggedInParentCountryCode() const
{
	if(_parent)
	{
		return _parent->getCountryCode();
	}
	return "";
}

std::string ParentManager::getBillingStatus() const
{
	if(_billingData)
	{
		return _billingData->getBillingStatusStr();
	}
	return "";
}

std::string ParentManager::getBillingDate() const
{
	if(_billingData)
	{
		return _billingData->getNextBillDate();
	}
	return "";
}

std::string ParentManager::getBillingProvider() const
{
	if(_billingData)
	{
		return _billingData->getPaymentProvider();
	}
	return "";
}

bool ParentManager::isLoggedInParentAnonymous()
{
	if(_parent)
	{
		return _parent->isAnonymous();
	}
	return true;
}

bool ParentManager::isBillingDataAvailable()
{
	return _isBillingDataAvailable;
}

bool ParentManager::isPaidUser()
{
	if(_billingData)
	{
		return (_billingData->getBillingStatusStr() == "SUBSCRIBED") || (_billingData->getBillingStatusStr() == "FREE_TRIAL");
	}
	return false;
}

bool ParentManager::emailRequiresVerification()
{
	if(_parent)
	{
		return (_parent->getActorStatus() != "VERIFIED") && (_parent->getActorStatus() != "ACTIVE");
	}
	return false;
}

bool ParentManager::isUserLoggedIn()
{
	return (!isLoggedInParentAnonymous() && getLoggedInParentId() != "");
}

//------------------------------------getting information from available children------------------------------------------

ChildRef ParentManager::getChildForId(const std::string& childId) const
{
	if(_availableChildrenById.find(childId) != _availableChildrenById.end())
	{
		return _availableChildrenById.at(childId);
	}
	return nullptr;
}
ChildRef ParentManager::getChild(int childNumber) const
{
	if(childNumber < _availableChildren.size())
	{
		return _availableChildren.at(childNumber);
	}
	return nullptr;
}


int ParentManager::getAmountOfAvailableChildren()
{
	return (int)_availableChildren.size();
}

//-----------Pending Friend Requests-------------
int ParentManager::getNoOfPendingFriendRequest()
{
	return (int)_pendingFriendRequests.size();
}

FriendRequestRef ParentManager::getPendingFriendRequest(int pendingFriendRequestNo) const
{
	if(pendingFriendRequestNo >= _pendingFriendRequests.size())
	{
		return nullptr;
	}
	return _pendingFriendRequests.at(pendingFriendRequestNo);
}

bool ParentManager::parseParentLoginData(const std::string &responseData)
{
	logoutChild();
    _billingData = nullptr;
	rapidjson::Document parentData;
	parentData.Parse(responseData.c_str());
	
	if(parentData.HasParseError())
	{
		return false;
	}
	
	if(getStringFromJson("code", parentData) != "INVALID_CREDENTIALS")
	{
		_parent = MutableParent::createWithJson(parentData);
		if(!_parent->isAnonymous())
		{
			addParentLoginDataToUserDefaults();
		}
		createCrashlyticsUserInfo(_parent->getId(), "");
		AnalyticsSingleton::getInstance()->registerAccountStatus(_parent->getActorStatus());
		
		PushNotificationsHandler::getInstance()->setNamedUserIdentifierForPushChannel(_parent->getId());
		
		return true;
	}
	
	return false;
}

bool ParentManager::parseUpdateParentData(const std::string &responseData)
{
	rapidjson::Document updateData;
	updateData.Parse(responseData.c_str());
	
	if(updateData.HasParseError())
	{
		return false;
	}
	
	if(!_parent)
	{
		return false;
	}
	_parent->setPin(getStringFromJson("pinNumber", updateData));
	
	if(updateData.HasMember("actorStatus"))
	{
		_parent->setActorStatus(getStringFromJson("actorStatus", updateData));
	}
	else
	{
		return false;
	}
	
	return true;
}

bool ParentManager::parseAvailableChildren(const std::string &responseData)
{
	rapidjson::Document childData;
	childData.Parse(responseData.c_str());
	
	if(childData.HasParseError())
	{
		return false;
	}
	
	clearAvailableChildren();
	
	for(int i = 0; i < childData.Size(); i++)
	{
		const rapidjson::Value &currentKidObj = childData[i];
		
		MutableChildRef child = MutableChild::createWithJson(currentKidObj);
		
		addChild(child);
	}
	
	return true;
}

void ParentManager::parseParentBillingData(const std::string &responseData)
{
	rapidjson::Document billingData;
	billingData.Parse(responseData.c_str());
	
	const BillingDataRef& billing = BillingData::createWithJson(billingData);
	if(!billing)
	{
		return;
	}
	setBillingData(billing);
	
	AnalyticsSingleton::getInstance()->registerBillingData(billing);
	
	setBillingDataAvailable(true);
	
}

void ParentManager::parseParentSessionData(const std::string &responseData)
{
	rapidjson::Document sessionData;
	sessionData.Parse(responseData.c_str());
	if(sessionData.HasParseError())
	{
		return;
	}
	
	const std::string& sessionId = getStringFromJson("sessionId", sessionData);
	if(sessionId == "")
	{
		return;
	}
	
	if(!_parent)
	{
		return;
	}
	_parent->setCDNSessionId(sessionId);
	
	UserDefault* def = UserDefault::getInstance();
	def->setStringForKey("loggedInParentCdnSessionId", _parent->getCDNSessionId());
	
}

void ParentManager::parseParentDetails(const std::string &responseData)
{
	rapidjson::Document parentData;
	parentData.Parse(responseData.c_str());
	if(parentData.HasParseError())
	{
		return;
	}
	if(_parent)
	{
		_parent->setEmail(getStringFromJson("emailAddress", parentData));
		_parent->setDisplayName(getStringFromJson("displayName", parentData));
		_parent->setPin(getStringFromJson("pinNumber", parentData));
		_parent->setAvatar(getStringFromJson("avatar", parentData));
	}
	
	
}

void ParentManager::parseChildUpdateData(const std::string& childId, const std::string &responseData)
{
	rapidjson::Document childData;
	childData.Parse(responseData.c_str());
	if(childData.HasParseError())
	{
		return;
	}
	MutableChildRef child = nullptr;
	if(_availableChildrenById.find(childId) != _availableChildrenById.end())
	{
		child = _availableChildrenById.at(childId);
	}
	if(child)
	{
		child->parseChildData(childData);
	}
}

bool ParentManager::parseChildLoginData(const std::string &responseData)
{
	rapidjson::Document data;
	data.Parse(responseData.c_str());
	if(data.HasParseError())
	{
		return false;
	}
	
	MutableChildRef child = nullptr;
	const std::string& childId = getStringFromJson("id", data);
	if(_availableChildrenById.find(childId) != _availableChildrenById.end())
	{
		child = _availableChildrenById.at(childId);
	}
	if(!child)
	{
		return false;
	}
	
	child->parseLoginData(data);
	
	ChildManager::getInstance()->setLoggedInChild(child);
	
	UserDefault* def = UserDefault::getInstance();
	def->setStringForKey("lastLoggedInChildId", child->getId());
	def->flush();
	
	createCrashlyticsUserInfo(ParentManager::getInstance()->getLoggedInParentId(), child->getId());
	
	ChildManager::getInstance()->setChildLoggedIn(true);
	return true;
}

void ParentManager::logoutChild()
{
	ChildManager::getInstance()->setChildLoggedIn(false);
    ChildManager::getInstance()->setLoggedInChild(nullptr);
}

void ParentManager::addParentLoginDataToUserDefaults()
{
	if(!_parent)
	{
		return;
	}
	UserDefault* def = UserDefault::getInstance();
	def->setStringForKey("loggedInParentId", _parent->getId());
	def->setStringForKey("loggedInParentCdnSessionId", _parent->getCDNSessionId());
	def->setStringForKey("loggedInParentApiSecret", _parent->getAPISecret());
	def->setStringForKey("loggedInParentApiKey", _parent->getAPIKey());
	def->setStringForKey("loggedInParentActorStatus", _parent->getActorStatus());
	def->setBoolForKey("isLoggedInParentAnonymous", _parent->isAnonymous());
	def->flush();
}

void ParentManager::retrieveParentLoginDataFromUserDefaults()
{
	const MutableParentRef& parent = MutableParent::create();
	
	UserDefault* def = UserDefault::getInstance();
	parent->setId(def->getStringForKey("loggedInParentId"));
	parent->setCDNSessionId(def->getStringForKey("loggedInParentCdnSessionId"));
	parent->setAPISecret(def->getStringForKey("loggedInParentApiSecret"));
	parent->setAPIKey(def->getStringForKey("loggedInParentApiKey"));
	parent->setActorStatus(def->getStringForKey("loggedInParentActorStatus"));
	parent->setAnonymous(def->getBoolForKey("isLoggedInParentAnonymous"));
	parent->setCountryCode(def->getStringForKey("loggedInParentCountryCode"));
	parent->setEmail(def->getStringForKey(ConfigStorage::kStoredUsernameKey));
	
	_parent = parent;
	
	createCrashlyticsUserInfo(parent->getId(), "");
	AnalyticsSingleton::getInstance()->registerAccountStatus(parent->getActorStatus());
	AnalyticsSingleton::getInstance()->registerAzoomeeEmail(parent->getEmail());
	
	PushNotificationsHandler::getInstance()->setNamedUserIdentifierForPushChannel(parent->getId());
}

bool ParentManager::hasParentLoginDataInUserDefaults()
{
	UserDefault* def = UserDefault::getInstance();
	if(def->getStringForKey("loggedInParentId") != "")
	{
		return true;
	}
	return false;
}

void ParentManager::clearParentLoginDataFromUserDefaults()
{
	UserDefault* def = UserDefault::getInstance();
	def->setStringForKey("loggedInParentId", "");
	def->setStringForKey("loggedInParentCdnSessionId", "");
	def->setStringForKey("loggedInParentApiSecret", "");
	def->setStringForKey("loggedInParentApiKey", "");
	def->setStringForKey("loggedInParentActorStatus", "");
	def->setStringForKey("loggedInParentCountryCode", "");
	def->flush();
}

bool ParentManager::parsePendingFriendRequests(const std::string &responseData)
{
	rapidjson::Document pendingRequestData;
	pendingRequestData.Parse(responseData.c_str());
	if(pendingRequestData.HasParseError() || !pendingRequestData.IsArray())
	{
		return false;
	}
	
	_pendingFriendRequests.clear();
	
	for(int i = 0; i < pendingRequestData.Size(); i++)
	{
		const rapidjson::Value &currentFriendRequestObj =pendingRequestData[i];
		
		FriendRequestRef friendRequest = FriendRequest::createWithJson(currentFriendRequestObj);
		
		_pendingFriendRequests.push_back(friendRequest);
	}
	
	return true;
}

void ParentManager::setLoggedInParentCountryCode(const std::string &countryCode)
{
	if(_parent)
	{
		_parent->setCountryCode(countryCode);
		UserDefault::getInstance()->setStringForKey("loggedInParentCountryCode", countryCode);
	}
}

void ParentManager::saveAnonCredentialsToDevice(const std::string &userId)
{
	UserDefault* userDefault = UserDefault::getInstance();
	userDefault->setStringForKey("anonEmail", userId);
	userDefault->flush();
}

void ParentManager::setAvatarColourForChild(const std::string& childId, const cocos2d::Color4B& colour)
{
    auto childItt = _availableChildrenById.find(childId);
    if(childItt != _availableChildrenById.end())
    {
        childItt->second->setAvatarColour(colour);
    }
}

void ParentManager::logoutParent()
{
    _parent = nullptr;
    _billingData = nullptr;
    _isBillingDataAvailable = false;
    ChildManager::getInstance()->setChildLoggedIn(false);
    ChildManager::getInstance()->setLoggedInChild(nullptr);
    clearAvailableChildren();
    clearParentLoginDataFromUserDefaults();
    createCrashlyticsUserInfo("", "");
}

NS_AZOOMEE_END
