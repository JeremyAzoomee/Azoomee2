//
//  UserAccountManager.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 06/06/2019.
//

#include "UserAccountManager.h"
#include "../Child/ChildManager.h"
#include "../../Crashlytics/CrashlyticsConfig.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../../Utils/PushNotificationsHandler.h"
#include "../../API/API.h"
#include "../ConfigStorage.h"
#include "../../Utils/StringFunctions.h"
#include "../Cookie/CookieManager.h"
#include "../Child/ChildManager.h"
#include "../HQDataObject/ContentItemPoolDownloadHandler.h"
#include "../HQDataObject/HQStructureDownloadHandler.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../../Utils/IosNativeFunctionsSingleton.h"
#endif

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string UserAccountManager::kAnonLoginPW = "ToBeDecided";
const char* const UserAccountManager::kAnonEmailKey = "anonEmail";
const char* const UserAccountManager::kStoredUsernameKey = "username";
const char* const UserAccountManager::kAnonOnboardingCompleteKey = "anonOnboardingComplete";
const std::string UserAccountManager::kUserHasLoggedInOnDeviceKey = "FirstSlideShowSeen";

static std::auto_ptr<UserAccountManager> sUserAccountManagerSharedInstance;

UserAccountManager* UserAccountManager::getInstance()
{
	if(!sUserAccountManagerSharedInstance.get())
	{
		sUserAccountManagerSharedInstance.reset(new UserAccountManager());
	}
	return sUserAccountManagerSharedInstance.get();
}

UserAccountManager::~UserAccountManager()
{
	
}

void UserAccountManager::setBillingData(const BillingDataRef& billingData)
{
	_billingData = billingData;
}
BillingDataRef UserAccountManager::getBillingData() const
{
	return _billingData;
}
void UserAccountManager::setBillingDataAvailable(bool available)
{
	_isBillingDataAvailable = available;
}
bool UserAccountManager::isBillingDataAvailable() const
{
	return _isBillingDataAvailable;
}

void UserAccountManager::setParent(const MutableParentRef& parent)
{
	_parent = parent;
}
ParentRef UserAccountManager::getParent() const
{
	return _parent;
}

void UserAccountManager::clearAvailableChildren()
{
	_availableChildren.clear();
	_availableChildrenById.clear();
}
void UserAccountManager::addChild(const MutableChildRef& child)
{
	_availableChildren.push_back(child);
	_availableChildrenById[child->getId()] = child;
}

/*std::vector<ChildRef> UserAccountManager::getAvailableChildren() const
{
	return _availableChildren;
}
std::map<std::string, ChildRef> UserAccountManager::getAvailableChildrenById() const
{
	return _availableChildrenById;
}*/

void UserAccountManager::setPendingFriendRequests(const std::vector<FriendRequestRef>& pendingRequests)
{
	_pendingFriendRequests = pendingRequests;
}
std::vector<FriendRequestRef> UserAccountManager::getPendingFriendRequests() const
{
	return _pendingFriendRequests;
}

std::string UserAccountManager::getLoggedInParentActorStatus() const
{
	if(_parent)
	{
		return _parent->getActorStatus();
	}
	return "";
}


std::string UserAccountManager::getLoggedInParentId() const
{
	if(_parent)
	{
		return _parent->getId();
	}
	return "";
}

std::string UserAccountManager::getLoggedInParentCdnSessionId() const
{
	if(_parent)
	{
		return _parent->getCDNSessionId();
	}
	return "";
}


std::string UserAccountManager::getLoggedInParentApiKey() const
{
	if(_parent)
	{
		return _parent->getAPIKey();
	}
	return "";
}

std::string UserAccountManager::getParentPin() const
{
	if(_parent)
	{
		return _parent->getPin();
	}
	return "";
}

std::string UserAccountManager::getLoggedInParentApiSecret() const
{
	if(_parent)
	{
		return _parent->getAPISecret();
	}
	return "";
}

std::string UserAccountManager::getLoggedInParentAvatarId() const
{
	if(_parent)
	{
		return _parent->getAvatar();
	}
	return "";
}

std::string UserAccountManager::getParentEmail() const
{
	if(_parent)
	{
		return _parent->getEmail();
	}
	return "";
}

std::string UserAccountManager::getParentDisplayName() const
{
	if(_parent)
	{
		return _parent->getDisplayName();
	}
	return "";
}

std::string UserAccountManager::getLoggedInParentCountryCode() const
{
	if(_parent)
	{
		return _parent->getCountryCode();
	}
	return "";
}

std::string UserAccountManager::getBillingStatus() const
{
	if(_billingData)
	{
		return _billingData->getBillingStatusStr();
	}
	return "";
}

std::string UserAccountManager::getBillingDate() const
{
	if(_billingData)
	{
		return _billingData->getNextBillDate();
	}
	return "";
}

std::string UserAccountManager::getBillingProvider() const
{
	if(_billingData)
	{
		return _billingData->getPaymentProvider();
	}
	return "";
}

bool UserAccountManager::isLoggedInParentAnonymous()
{
	if(_parent)
	{
		return _parent->isAnonymous();
	}
	return true;
}

bool UserAccountManager::isBillingDataAvailable()
{
	return _isBillingDataAvailable;
}

bool UserAccountManager::isPaidUser()
{
	if(_billingData)
	{
		return (_billingData->getBillingStatusStr() == "SUBSCRIBED") || (_billingData->getBillingStatusStr() == "FREE_TRIAL");
	}
	return false;
}

bool UserAccountManager::emailRequiresVerification()
{
	if(_parent)
	{
		return (_parent->getActorStatus() != "VERIFIED") && (_parent->getActorStatus() != "ACTIVE");
	}
	return false;
}

bool UserAccountManager::isUserLoggedIn()
{
	return (!isLoggedInParentAnonymous() && getLoggedInParentId() != "");
}

//------------------------------------getting information from available children------------------------------------------

ChildRef UserAccountManager::getChildForId(const std::string& childId) const
{
	if(_availableChildrenById.find(childId) != _availableChildrenById.end())
	{
		return _availableChildrenById.at(childId);
	}
	return nullptr;
}
ChildRef UserAccountManager::getChild(int childNumber) const
{
	if(childNumber < _availableChildren.size())
	{
		return _availableChildren.at(childNumber);
	}
	return nullptr;
}


int UserAccountManager::getAmountOfAvailableChildren()
{
	return (int)_availableChildren.size();
}

//-----------Pending Friend Requests-------------
int UserAccountManager::getNoOfPendingFriendRequest()
{
	return (int)_pendingFriendRequests.size();
}

FriendRequestRef UserAccountManager::getPendingFriendRequest(int pendingFriendRequestNo) const
{
	if(pendingFriendRequestNo >= _pendingFriendRequests.size())
	{
		return nullptr;
	}
	return _pendingFriendRequests.at(pendingFriendRequestNo);
}

bool UserAccountManager::parseParentLoginData(const std::string &responseData)
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

bool UserAccountManager::parseUpdateParentData(const std::string &responseData)
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

bool UserAccountManager::parseAvailableChildren(const std::string &responseData)
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

void UserAccountManager::parseParentBillingData(const std::string &responseData)
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

void UserAccountManager::parseParentSessionData(const std::string &responseData)
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

void UserAccountManager::parseParentDetails(const std::string &responseData)
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

void UserAccountManager::parseChildUpdateData(const std::string& childId, const std::string &responseData)
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

bool UserAccountManager::parseChildLoginData(const std::string &responseData)
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
	
	createCrashlyticsUserInfo(UserAccountManager::getInstance()->getLoggedInParentId(), child->getId());
	
	ChildManager::getInstance()->setChildLoggedIn(true);
	return true;
}

void UserAccountManager::logoutChild()
{
	ChildManager::getInstance()->setChildLoggedIn(false);
    ChildManager::getInstance()->setLoggedInChild(nullptr);
}

void UserAccountManager::addParentLoginDataToUserDefaults()
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

void UserAccountManager::retrieveParentLoginDataFromUserDefaults()
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
	parent->setEmail(def->getStringForKey(UserAccountManager::kStoredUsernameKey));
	
	_parent = parent;
	
	createCrashlyticsUserInfo(parent->getId(), "");
	AnalyticsSingleton::getInstance()->registerAccountStatus(parent->getActorStatus());
	AnalyticsSingleton::getInstance()->registerAzoomeeEmail(parent->getEmail());
	
	PushNotificationsHandler::getInstance()->setNamedUserIdentifierForPushChannel(parent->getId());
}

bool UserAccountManager::hasParentLoginDataInUserDefaults()
{
	UserDefault* def = UserDefault::getInstance();
	if(def->getStringForKey("loggedInParentId") != "")
	{
		return true;
	}
	return false;
}

void UserAccountManager::clearParentLoginDataFromUserDefaults()
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

bool UserAccountManager::parsePendingFriendRequests(const std::string &responseData)
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

void UserAccountManager::setLoggedInParentCountryCode(const std::string &countryCode)
{
	if(_parent)
	{
		_parent->setCountryCode(countryCode);
		UserDefault::getInstance()->setStringForKey("loggedInParentCountryCode", countryCode);
	}
}

void UserAccountManager::saveAnonCredentialsToDevice(const std::string &userId)
{
	UserDefault* userDefault = UserDefault::getInstance();
	userDefault->setStringForKey("anonEmail", userId);
	userDefault->flush();
}

void UserAccountManager::setAvatarColourForChild(const std::string& childId, const cocos2d::Color4B& colour)
{
    auto childItt = _availableChildrenById.find(childId);
    if(childItt != _availableChildrenById.end())
    {
        childItt->second->setAvatarColour(colour);
    }
}

void UserAccountManager::logoutParent()
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

bool UserAccountManager::localLogin()
{
    if(hasParentLoginDataInUserDefaults())
    {
        retrieveParentLoginDataFromUserDefaults();
        return true;
    }
    return false;
}

void UserAccountManager::login(const std::string& email, const std::string& password, const OnCompleteCallback& callback)
{
     auto onSuccess = [callback, this](const std::string& tag, const std::string& headers, const std::string& body){
         parseParentLoginData(body);
         getBillingDataForLoggedInParent([callback, this](bool success, long errorcode){
             getChildrenForLoggedInParent(callback);
         });
    };
       
    auto onFailed = [callback](const std::string& tag, long errorCode){
        if(callback)
        {
            callback(false, errorCode);
        }
    };
       
    HttpRequestCreator* request = API::LoginRequest(email, password, onSuccess, onFailed);
    request->execute();
}

void UserAccountManager::anonLogin(const OnCompleteCallback& callback)
{
    cocos2d::UserDefault* userDefault = cocos2d::UserDefault::getInstance();
    const std::string& anonEmail = userDefault->getStringForKey(UserAccountManager::kAnonEmailKey, "");
    
    if(anonEmail == "")
    {
        auto onSuccess = [this, callback](const std::string& tag, const std::string& headers, const std::string& body){
            rapidjson::Document json;
            json.Parse(body.c_str());
            const std::string& userId = getStringFromJson("id", json);
            saveAnonCredentialsToDevice(userId);
            this->login(userId, UserAccountManager::kAnonLoginPW, callback);
        };
        
        auto onFailed = [callback](const std::string& tag, long errorCode){
            if(callback)
            {
                callback(false, errorCode);
            }
        };
        HttpRequestCreator* request = API::GetAnonCredentials(onSuccess, onFailed);
        request->execute();
    }
    else
    {
        login(anonEmail, UserAccountManager::kAnonLoginPW, callback);
    }
}

void UserAccountManager::getBillingDataForLoggedInParent(const OnCompleteCallback& callback)
{
    auto onSuccess = [this, callback](const std::string& tag, const std::string& headers, const std::string& body){
        parseParentBillingData(body);
        if(callback)
        {
            callback(true, 200);
        }
    };
    
    auto onFailed = [callback](const std::string& tag, long errorCode){
        if(callback)
        {
            callback(false, errorCode);
        }
    };
    
    HttpRequestCreator* request = API::UpdateBillingDataRequest(UserAccountManager::getInstance()->getLoggedInParentId(), onSuccess, onFailed);
    request->execute();
}

void UserAccountManager::getChildrenForLoggedInParent(const OnCompleteCallback& callback)
{
    auto onSuccess = [this, callback](const std::string& tag, const std::string& headers, const std::string& body){
        parseAvailableChildren(body);
        if(callback)
        {
            callback(true, 200);
        }
    };
    
    auto onFailed = [callback](const std::string& tag, long errorCode){
        if(callback)
        {
            callback(false, errorCode);
        }
    };
    
    HttpRequestCreator* request = API::GetAvailableChildrenRequest(UserAccountManager::getInstance()->getLoggedInParentId(), onSuccess, onFailed);
    request->execute();
}

void UserAccountManager::loginChild(const std::string& profileName, const OnCompleteCallback& callback)
{
    auto onSuccess = [this, callback](const std::string& tag, const std::string& headers, const std::string& body){
        if((!UserAccountManager::getInstance()->parseChildLoginData(body)))
        {
            if(callback)
            {
                callback(false, 200);
            }
        }
        setLoggedInParentCountryCode(StringFunctions::getValueFromHttpResponseHeaderForKey(API::kAZCountryCodeKey, headers));
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        IosNativeFunctionsSingleton::getInstance()->deleteHttpCookies(); //ios handles cookies on OS level. Removal of earlier cookies is important to avoid watching premium content with a free user.
        #endif
                
        const std::string& userId = ChildManager::getInstance()->getLoggedInChild()->getId();
        const std::string& sessionId = ChildManager::getInstance()->getLoggedInChild()->getCDNSessionId();
            
        auto onSuccess = [callback](const std::string& tag, const std::string& headers, const std::string& body){
            CookieManager::getInstance()->parseDownloadCookies(headers);
            ContentItemPoolDownloadHandler::getInstance()->getLatestData([callback](bool success){ //on complete
                if(success)
                {
                    HQStructureDownloadHandler::getInstance()->getLatestData([callback](bool success){ //on complete
                        if(success)
                        {
                            if(callback)
                            {
                                callback(true, 200);
                            }
                        }
                        else
                        {
                            if(callback)
                            {
                                callback(false, 200);
                            }
                        }
                    });
                }
                else
                {
                    if(callback)
                    {
                        callback(false, 200);
                    }
                }
            });
        };
            
        auto onFailed = [callback](const std::string& tag, long errorCode){
            if(callback)
            {
                callback(false, errorCode);
            }
        };
            
        HttpRequestCreator* request = API::GetSessionCookiesRequest(userId, sessionId, false, onSuccess, onFailed);
        request->execute();
    };
    
    auto onFailed = [callback](const std::string& tag, long errorCode){
        if(callback)
        {
            callback(false, errorCode);
        }
    };
    
    HttpRequestCreator* request = API::ChildLoginRequest(profileName, onSuccess, onFailed);
    request->execute();
}


void UserAccountManager::setHasLoggedInOnDevice(bool loggedIn)
{
    UserDefault::getInstance()->setBoolForKey(kUserHasLoggedInOnDeviceKey.c_str(), loggedIn);
}

bool UserAccountManager::userHasLoggedInOnDevice()
{
    return UserDefault::getInstance()->getBoolForKey(kUserHasLoggedInOnDeviceKey.c_str(), false);
}
NS_AZOOMEE_END
