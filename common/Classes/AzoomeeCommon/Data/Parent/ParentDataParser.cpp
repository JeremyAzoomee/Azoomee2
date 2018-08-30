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
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    parentData->parentLoginData.Parse(responseData.c_str());
    
    if(parentData->parentLoginData.HasParseError())
    {
        return false;
    }
    
    if(parentData->parentLoginData.HasMember("code"))
    {
        if(parentData->parentLoginData["code"] != "INVALID_CREDENTIALS")
        {
            parentData->loggedInParentId = getStringFromJson("id", parentData->parentLoginData);
            parentData->loggedInParentCdnSessionId = getStringFromJson("cdn-sessionid", parentData->parentLoginData);
            parentData->loggedInParentApiSecret = getStringFromJson("apiSecret", parentData->parentLoginData);
            parentData->loggedInParentApiKey = getStringFromJson("apiKey", parentData->parentLoginData);
            parentData->loggedInParentActorStatus = getStringFromJson("actorStatus", parentData->parentLoginData);
            parentData->loggedInParentAvatarId = getStringFromJson("avatar", parentData->parentLoginData);
            parentData->loggedInParentPin = getStringFromJson("pinNumber", parentData->parentLoginData);
            parentData->isLoggedInParentAnonymous = false;
            
            addParentLoginDataToUserDefaults();
            
            createCrashlyticsUserInfo(parentData->loggedInParentId, "");
            AnalyticsSingleton::getInstance()->registerAccountStatus(parentData->loggedInParentActorStatus);
            
            PushNotificationsHandler::getInstance()->setNamedUserIdentifierForPushChannel(parentData->loggedInParentId);
            
            return true;
        }
    }
    
    return false;
}
    
bool ParentDataParser::parseParentLoginDataFromAnonymousDeviceLogin(const std::string &responseData)
{
    logoutChild();
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    parentData->parentLoginData.Parse(responseData.c_str());
    
    if(parentData->parentLoginData.HasParseError())
    {
        return false;
    }
    
    if(parentData->parentLoginData.HasMember("userType"))
    {
        if(getStringFromJson("userType", parentData->parentLoginData) == "ANONYMOUS")
        {
            parentData->loggedInParentId =  getStringFromJson("id", parentData->parentLoginData);
            parentData->loggedInParentCdnSessionId = getStringFromJson("cdn-sessionid", parentData->parentLoginData);
            parentData->loggedInParentApiSecret = getStringFromJson("apiSecret", parentData->parentLoginData);
            parentData->loggedInParentApiKey = getStringFromJson("apiKey", parentData->parentLoginData);
            parentData->loggedInParentActorStatus = getStringFromJson("actorStatus", parentData->parentLoginData);
            parentData->isLoggedInParentAnonymous = true;

            parentData->loggedInParentPin = "";
            
            createCrashlyticsUserInfo(parentData->loggedInParentId, "");
            AnalyticsSingleton::getInstance()->registerAccountStatus(parentData->loggedInParentActorStatus);
            
            PushNotificationsHandler::getInstance()->setNamedUserIdentifierForPushChannel(parentData->loggedInParentId);
            
            return true;
        }
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
    
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    
    parentData->loggedInParentPin = getStringFromJson("pinNumber", updateData);
    
    if(updateData.HasMember("actorStatus"))
    {
        parentData->loggedInParentActorStatus = getStringFromJson("actorStatus", updateData);
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
    parentData->availableChildrenData.Parse(responseData.c_str());
    
    if(parentData->availableChildrenData.HasParseError())
    {
        return false;
    }
    
    parentData->availableChildren.clear();
    parentData->availableChildrenById.clear();
    parentData->isLoggedInParentAnonymous = false; //if user has children, it must be non-anonymous
    
    for(int i = 0; i < parentData->availableChildrenData.Size(); i++)
    {
        const rapidjson::Value &currentKidObj = parentData->availableChildrenData[i];
        
        std::map<std::string, std::string> currentChild;
        
        currentChild["profileName"] = getStringFromJson("profileName", currentKidObj);
        currentChild["avatar"] = getStringFromJson("avatar", currentKidObj);
        currentChild["inviteCode"] = getStringFromJson("inviteCode", currentKidObj);
        currentChild["sex"] = getStringFromJson("sex", currentKidObj);
        currentChild["dob"] = getStringFromJson("dob", currentKidObj);
        currentChild["id"] = getStringFromJson("id", currentKidObj);
        
        parentData->availableChildren.push_back(currentChild);
        
        const std::string& childId = currentChild["id"];
        parentData->availableChildrenById[childId] = i;
    }
    
    return true;
}

void ParentDataParser::parseParentBillingData(const std::string &responseData)
{
    rapidjson::Document billingData;
    billingData.Parse(responseData.c_str());
    
    if(billingData.HasParseError())
    {
        cocos2d::log("Billing Parse Error");
        return;
    }
    
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    
    parentData->loggedInParentBillingStatus = getStringFromJson("billingStatus", billingData);
    AnalyticsSingleton::getInstance()->registerBillingStatus(parentData->loggedInParentBillingStatus);
    
    parentData->loggedInParentBillingDate = getStringFromJson("nextBillDate", billingData);
    
    parentData->loggedInParentBillingProvider = getStringFromJson("paymentProvider", billingData);
    AnalyticsSingleton::getInstance()->registerBillingProvider(parentData->loggedInParentBillingProvider);
    
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
    
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    parentData->loggedInParentCdnSessionId = sessionId;
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("loggedInParentCdnSessionId", parentData->loggedInParentCdnSessionId);
    
}

void ParentDataParser::parseParentDetails(const std::string &responseData)
{
    rapidjson::Document sessionData;
    sessionData.Parse(responseData.c_str());
    if(sessionData.HasParseError())
    {
        return;
    }
    
    
}
    
void ParentDataParser::logoutChild()
{
    ChildDataStorage::getInstance()->childLoggedIn = false;
}

void ParentDataParser::addParentLoginDataToUserDefaults()
{
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("loggedInParentId", parentData->loggedInParentId);
    def->setStringForKey("loggedInParentCdnSessionId", parentData->loggedInParentCdnSessionId);
    def->setStringForKey("loggedInParentApiSecret", parentData->loggedInParentApiSecret);
    def->setStringForKey("loggedInParentApiKey", parentData->loggedInParentApiKey);
    def->setStringForKey("loggedInParentActorStatus", parentData->loggedInParentActorStatus);
    def->setStringForKey("loggedInParentAvatarId", parentData->loggedInParentAvatarId);
    def->setBoolForKey("isLoggedInParentAnonymous", parentData->isLoggedInParentAnonymous);
    def->flush();
}

void ParentDataParser::retrieveParentLoginDataFromUserDefaults()
{
    cocos2d::log("ParentDataParser::retrieveParentLoginDataFromUserDefaults");
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    
    UserDefault* def = UserDefault::getInstance();
    parentData->loggedInParentId = def->getStringForKey("loggedInParentId");
    parentData->loggedInParentCdnSessionId = def->getStringForKey("loggedInParentCdnSessionId");
    parentData->loggedInParentApiSecret = def->getStringForKey("loggedInParentApiSecret");
    parentData->loggedInParentApiKey = def->getStringForKey("loggedInParentApiKey");
    parentData->loggedInParentActorStatus = def->getStringForKey("loggedInParentActorStatus");
    parentData->loggedInParentAvatarId = def->getStringForKey("loggedInParentAvatarId");
    parentData->isLoggedInParentAnonymous = def->getBoolForKey("isLoggedInParentAnonymous");
    parentData->loggedInParentCountryCode = def->getStringForKey("loggedInParentCountryCode");
    cocos2d::log("loggedInParentId = %s", parentData->loggedInParentId.c_str());
    cocos2d::log("loggedInParentCdnSessionId = %s", parentData->loggedInParentCdnSessionId.c_str());
    cocos2d::log("loggedInParentApiSecret = %s", parentData->loggedInParentApiSecret.c_str());
    cocos2d::log("loggedInParentApiKey = %s", parentData->loggedInParentApiKey.c_str());
    cocos2d::log("loggedInParentActorStatus = %s", parentData->loggedInParentActorStatus.c_str());
    cocos2d::log("loggedInParentAvatarId = %s", parentData->loggedInParentAvatarId.c_str());
    cocos2d::log("loggedInParentCountryCode = %s", parentData->loggedInParentCountryCode.c_str());
    
    createCrashlyticsUserInfo(parentData->loggedInParentId, "");
    AnalyticsSingleton::getInstance()->registerAccountStatus(parentData->loggedInParentActorStatus);
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(def->getStringForKey("username"));
    
    PushNotificationsHandler::getInstance()->setNamedUserIdentifierForPushChannel(parentData->loggedInParentId);
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
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    parentData->pendingFriendRequests.clear();
    
    parentData->pendingFriendRequestData.Parse(responseData.c_str());
    if(parentData->pendingFriendRequestData.HasParseError())
    {
        return false;
    }
    
    for(int i = 0; i < parentData->pendingFriendRequestData.Size(); i++)
    {
        const rapidjson::Value &currentFriendRequestObj = parentData->pendingFriendRequestData[i];
        
        std::map<std::string, std::string> currentPendingFriendRequest;
        
        currentPendingFriendRequest["senderName"] = getStringFromJson("senderName", currentFriendRequestObj);
        currentPendingFriendRequest["friendName"] = getStringFromJson("friendName", currentFriendRequestObj);
        currentPendingFriendRequest["inviteeCode"] = getStringFromJson("inviteeCode", currentFriendRequestObj);
        currentPendingFriendRequest["id"] = getStringFromJson("id", currentFriendRequestObj);
        currentPendingFriendRequest["senderId"] = getStringFromJson("senderId", currentFriendRequestObj);
        currentPendingFriendRequest["respondentId"] = getStringFromJson("respondentId", currentFriendRequestObj);

        parentData->pendingFriendRequests.push_back(currentPendingFriendRequest);
    }
    
    return true;
}
    
void ParentDataParser::setBillingDataAvailable(bool isAvailable)
{
    ParentDataStorage::getInstance()->isBillingDataAvailable = isAvailable;
}

void ParentDataParser::setLoggedInParentCountryCode(const std::string &countryCode)
{
    ParentDataStorage::getInstance()->loggedInParentCountryCode = countryCode;
    UserDefault::getInstance()->setStringForKey("loggedInParentCountryCode", countryCode);
}
    

}
