#include "ParentDataParser.h"
#include "../Child/ChildManager.h"
#include "../Parent/ParentDataStorage.h"
#include "../../UI/ModalMessages.h"
#include "../../Crashlytics/CrashlyticsConfig.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../../ErrorCodes.h"
#include "../../Utils/PushNotificationsHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
		const ParentRef& parent = Parent::createWithJson(parentData);
		ParentDataStorage::getInstance()->setParent(parent);
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
    
    const ParentRef& parentData = ParentDataStorage::getInstance()->getParent();
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
    
	parentData->clearAvailableChildren();
    
    for(int i = 0; i < childData.Size(); i++)
    {
        const rapidjson::Value &currentKidObj = childData[i];
        
		ChildRef child = Child::createWithJson(currentKidObj);
        
		parentData->addChild(child);
    }
    
    return true;
}

void ParentDataParser::parseParentBillingData(const std::string &responseData)
{
    rapidjson::Document billingData;
    billingData.Parse(responseData.c_str());
	
	ParentDataStorage* parentData = ParentDataStorage::getInstance();
	
	const BillingDataRef& billing = BillingData::createWithJson(billingData);
	if(!billing)
	{
		return;
	}
	parentData->setBillingData(billing);
	
	AnalyticsSingleton::getInstance()->registerBillingData(billing);
	
    parentData->setBillingDataAvailable(true);
	
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
    
    const ParentRef& parentData = ParentDataStorage::getInstance()->getParent();
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
    
    const ParentRef& parentDataStorage = ParentDataStorage::getInstance()->getParent();
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
    ChildManager::getInstance()->setChildLoggedIn(false);
}

void ParentDataParser::addParentLoginDataToUserDefaults()
{
    const ParentRef& parentData = ParentDataStorage::getInstance()->getParent();
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
	const ParentRef& parent = Parent::create();
	
    UserDefault* def = UserDefault::getInstance();
    parent->setId(def->getStringForKey("loggedInParentId"));
    parent->setCDNSessionId(def->getStringForKey("loggedInParentCdnSessionId"));
    parent->setAPISecret(def->getStringForKey("loggedInParentApiSecret"));
    parent->setAPIKey(def->getStringForKey("loggedInParentApiKey"));
    parent->setActorStatus(def->getStringForKey("loggedInParentActorStatus"));
    parent->setAnonymous(def->getBoolForKey("isLoggedInParentAnonymous"));
    parent->setCountryCode(def->getStringForKey("loggedInParentCountryCode"));
	parent->setEmail(def->getStringForKey(ConfigStorage::kStoredUsernameKey));
	
	ParentDataStorage::getInstance()->setParent(parent);
	
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
	
	ParentDataStorage::getInstance()->setPendingFriendRequests(pendingRequests);
    return true;
}
    
void ParentDataParser::setBillingDataAvailable(bool isAvailable)
{
    ParentDataStorage::getInstance()->setBillingDataAvailable(isAvailable);
}

void ParentDataParser::setLoggedInParentCountryCode(const std::string &countryCode)
{
	if(ParentDataStorage::getInstance()->getParent())
	{
    	ParentDataStorage::getInstance()->getParent()->setCountryCode(countryCode);
    	UserDefault::getInstance()->setStringForKey("loggedInParentCountryCode", countryCode);
	}
}
    
void ParentDataParser::saveAnonCredentialsToDevice(const std::string &userId)
{
	UserDefault* userDefault = UserDefault::getInstance();
	userDefault->setStringForKey("anonEmail", userId);
	userDefault->flush();
}
	
NS_AZOOMEE_END
