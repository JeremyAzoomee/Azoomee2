#include "ParentDataParser.h"
#include "../Child/ChildDataStorage.h"
#include "../Parent/ParentDataStorage.h"
#include "../../UI/ModalMessages.h"
#include "../../Crashlytics/CrashlyticsConfig.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../../ErrorCodes.h"
#include "../../Utils/PushNotificationsHandler.h"

using namespace cocos2d;

namespace Azoomee
{

static ParentDataParser *_sharedParentDataParser = NULL;

ParentDataParser* ParentDataParser::getInstance()
{
    if (! _sharedParentDataParser)
    {
        _sharedParentDataParser = new ParentDataParser();
        _sharedParentDataParser->init();
    }
    
    return _sharedParentDataParser;
}

ParentDataParser::~ParentDataParser(void)
{
}

bool ParentDataParser::init(void)
{
    return true;
}

bool ParentDataParser::parseParentLoginData(const std::string &responseData)
{
    logoutChild();
	rapidjson::Document parentData;
    parentData.Parse(responseData.c_str());
    
    if(parentData.HasParseError())
    {
        return false;
    }
	
	if(getStringFromJson("code", parentData) != "INVALID_CREDENTIALS")
	{
		ParentRef parent = Parent::createWithJson(parentData);
		ParentDataStorage::getInstance()->_parent = parent;
		if(!parent->isAnonymous())
		{
			addParentLoginDataToUserDefaults();
		}
		createCrashlyticsUserInfo(parent->getId(), "");
		AnalyticsSingleton::getInstance()->registerAccountStatus(parent->getActorStatus());
		
		PushNotificationsHandler::getInstance()->setNamedUserIdentifierForPushChannel(parent->getId());
		
		return true;
	}
    
    return false;
}

bool ParentDataParser::parseUpdateParentData(const std::string &responseData)
{
    rapidjson::Document updateData;
    updateData.Parse(responseData.c_str());
    
    if(updateData.HasParseError())
    {
        return false;
    }
    
    ParentRef parentData = ParentDataStorage::getInstance()->_parent;
    if(!parentData)
	{
		return false;
	}
    parentData->setPin(getStringFromJson("pinNumber", updateData));
    
    if(updateData.HasMember("actorStatus"))
    {
        parentData->setActorStatus(getStringFromJson("actorStatus", updateData));
    }
    else
    {
        return false;
    }
    
    return true;
}

bool ParentDataParser::parseAvailableChildren(const std::string &responseData)
{
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
	rapidjson::Document childData;
	childData.Parse(responseData.c_str());
    
    if(childData.HasParseError())
    {
        return false;
    }
    
    parentData->_availableChildren.clear();
    parentData->_availableChildrenById.clear();
    //parentData->isLoggedInParentAnonymous = false; //if user has children, it must be non-anonymous
    
    for(int i = 0; i < childData.Size(); i++)
    {
        const rapidjson::Value &currentKidObj = childData[i];
        
		ChildRef child = Child::createWithJson(currentKidObj);
        
        parentData->_availableChildren.push_back(child);
        parentData->_availableChildrenById[child->getId()] = child;
    }
    
    return true;
}

void ParentDataParser::parseParentBillingData(const std::string &responseData)
{
    rapidjson::Document billingData;
    billingData.Parse(responseData.c_str());
	
	ParentDataStorage* parentData = ParentDataStorage::getInstance();
	
	parentData->_billingData = BillingData::createWithJson(billingData);
	
	AnalyticsSingleton::getInstance()->registerBillingData(parentData->_billingData);
	
    parentData->isBillingDataAvailable = true;
	
}

    void ParentDataParser::parseParentSessionData(const std::string &responseData)
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
    
    ParentRef parentData = ParentDataStorage::getInstance()->_parent;
	if(!parentData)
	{
		return;
	}
    parentData->setCDNSessionId(sessionId);
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("loggedInParentCdnSessionId", parentData->getCDNSessionId());
	 
}

void ParentDataParser::parseParentDetails(const std::string &responseData)
{
    rapidjson::Document parentData;
    parentData.Parse(responseData.c_str());
    if(parentData.HasParseError())
    {
        return;
    }
    
    ParentRef parentDataStorage = ParentDataStorage::getInstance()->_parent;
    parentDataStorage->setEmail(getStringFromJson("emailAddress", parentData));
    parentDataStorage->setDisplayName(getStringFromJson("displayName", parentData));
    parentDataStorage->setPin(getStringFromJson("pinNumber", parentData));
    parentDataStorage->setAvatar(getStringFromJson("avatar", parentData));
    
    
}
    
void ParentDataParser::parseChildUpdateData(const ChildRef& child, const std::string &responseData)
{
    rapidjson::Document childData;
    childData.Parse(responseData.c_str());
    if(childData.HasParseError())
    {
        return;
    }
	child->parseChildData(childData);
}
    
void ParentDataParser::logoutChild()
{
    ChildDataStorage::getInstance()->childLoggedIn = false;
}

void ParentDataParser::addParentLoginDataToUserDefaults()
{
    ParentRef parentData = ParentDataStorage::getInstance()->_parent;
	if(!parentData)
	{
		return;
	}
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("loggedInParentId", parentData->getId());
    def->setStringForKey("loggedInParentCdnSessionId", parentData->getCDNSessionId());
    def->setStringForKey("loggedInParentApiSecret", parentData->getAPISecret());
    def->setStringForKey("loggedInParentApiKey", parentData->getAPIKey());
    def->setStringForKey("loggedInParentActorStatus", parentData->getActorStatus());
    def->setBoolForKey("isLoggedInParentAnonymous", parentData->isAnonymous());
    def->flush();
}

void ParentDataParser::retrieveParentLoginDataFromUserDefaults()
{
	ParentRef parent = Parent::create();
	
    UserDefault* def = UserDefault::getInstance();
    parent->setId(def->getStringForKey("loggedInParentId"));
    parent->setCDNSessionId(def->getStringForKey("loggedInParentCdnSessionId"));
    parent->setAPISecret(def->getStringForKey("loggedInParentApiSecret"));
    parent->setAPIKey(def->getStringForKey("loggedInParentApiKey"));
    parent->setActorStatus(def->getStringForKey("loggedInParentActorStatus"));
    parent->setAnonymous(def->getBoolForKey("isLoggedInParentAnonymous"));
    parent->setCountryCode(def->getStringForKey("loggedInParentCountryCode"));
	parent->setEmail(def->getStringForKey("username"));
	
	ParentDataStorage::getInstance()->_parent = parent;
	
    createCrashlyticsUserInfo(parent->getId(), "");
    AnalyticsSingleton::getInstance()->registerAccountStatus(parent->getActorStatus());
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(parent->getEmail());
    
    PushNotificationsHandler::getInstance()->setNamedUserIdentifierForPushChannel(parent->getId());
}

bool ParentDataParser::hasParentLoginDataInUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    if(def->getStringForKey("loggedInParentId") != "")
    {
        return true;
    }
    return false;
}

void ParentDataParser::clearParentLoginDataFromUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("loggedInParentId", "");
    def->setStringForKey("loggedInParentCdnSessionId", "");
    def->setStringForKey("loggedInParentApiSecret", "");
    def->setStringForKey("loggedInParentApiKey", "");
    def->setStringForKey("loggedInParentActorStatus", "");
    def->setStringForKey("loggedInParentCountryCode", "");
	def->setStringForKey("username", "");
    def->flush();
}
    
bool ParentDataParser::parsePendingFriendRequests(const std::string &responseData)
{
	rapidjson::Document pendingRequestData;
	pendingRequestData.Parse(responseData.c_str());
    if(pendingRequestData.HasParseError() || !pendingRequestData.IsArray())
    {
        return false;
    }
	
	std::vector<FriendRequestRef> pendingRequests;
	
    for(int i = 0; i < pendingRequestData.Size(); i++)
    {
        const rapidjson::Value &currentFriendRequestObj =pendingRequestData[i];
        
		FriendRequestRef friendRequest = FriendRequest::createWithJson(currentFriendRequestObj);

		pendingRequests.push_back(friendRequest);
    }
	
	ParentDataStorage::getInstance()->_pendingFriendRequests = pendingRequests;
    return true;
}
    
void ParentDataParser::setBillingDataAvailable(bool isAvailable)
{
    ParentDataStorage::getInstance()->isBillingDataAvailable = isAvailable;
}

void ParentDataParser::setLoggedInParentCountryCode(const std::string &countryCode)
{
	if(ParentDataStorage::getInstance()->_parent)
	{
    	ParentDataStorage::getInstance()->_parent->setCountryCode(countryCode);
    	UserDefault::getInstance()->setStringForKey("loggedInParentCountryCode", countryCode);
	}
}
    

}
