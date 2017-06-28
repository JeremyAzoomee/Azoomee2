#include "ParentDataParser.h"
#include "../Child/ChildDataStorage.h"
#include "../Parent/ParentDataStorage.h"
#include "../../UI/ModalMessages.h"
#include "../../Crashlytics/CrashlyticsConfig.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../../ErrorCodes.h"

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

bool ParentDataParser::parseParentLoginData(std::string responseData)
{
    logoutChild();
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    parentData->parentLoginData.Parse(responseData.c_str());
    
    if(parentData->parentLoginData.HasMember("code"))
    {
        if(parentData->parentLoginData["code"] != "INVALID_CREDENTIALS")
        {
            parentData->loggedInParentId = parentData->parentLoginData["id"].GetString();
            parentData->loggedInParentCdnSessionId = parentData->parentLoginData["cdn-sessionid"].GetString();
            parentData->loggedInParentApiSecret = parentData->parentLoginData["apiSecret"].GetString();
            parentData->loggedInParentApiKey = parentData->parentLoginData["apiKey"].GetString();
            parentData->loggedInParentActorStatus = parentData->parentLoginData["actorStatus"].GetString();
            
            if(parentData->parentLoginData.HasMember("pinNumber"))
            {
                if(parentData->parentLoginData["pinNumber"].IsString())
                {
                    parentData->loggedInParentPin = parentData->parentLoginData["pinNumber"].GetString();
                }
                else
                {
                    parentData->loggedInParentPin = "";
                }
            }
            else
            {
                parentData->loggedInParentPin = "";
            }
            
            addParentLoginDataToUserDefaults();
            
            createCrashlyticsUserInfo(parentData->loggedInParentId, "");
            AnalyticsSingleton::getInstance()->registerParentID(parentData->loggedInParentId);
            AnalyticsSingleton::getInstance()->registerAccountStatus(parentData->loggedInParentActorStatus);
            
            return true;
        }
    }
    
    return false;
}

bool ParentDataParser::parseUpdateParentData(std::string responseData)
{
    rapidjson::Document updateData;
    updateData.Parse(responseData.c_str());
    
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    
    if(updateData.HasMember("pinNumber"))
    {
        if(updateData["pinNumber"].IsString())
        {
            parentData->loggedInParentPin = updateData["pinNumber"].GetString();
        }
        else
        {
            parentData->loggedInParentPin = "";
        }
    }
    else
    {
        parentData->loggedInParentPin = "";
    }
    
    if(updateData.HasMember("actorStatus")) parentData->loggedInParentActorStatus = updateData["actorStatus"].GetString();
    else return false;
    
    return true;
}

bool ParentDataParser::parseAvailableChildren(std::string responseData)
{
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    parentData->availableChildren.clear();
    parentData->availableChildrenById.clear();
    
    parentData->availableChildrenData.Parse(responseData.c_str());
    
    for(int i = 0; i < parentData->availableChildrenData.Size(); i++)
    {
        std::map<std::string, std::string> currentChild;
        currentChild["profileName"] = parentData->availableChildrenData[i]["profileName"].GetString();
        currentChild["avatar"] = parentData->availableChildrenData[i]["avatar"].GetString();

        if(parentData->availableChildrenData[i].HasMember("inviteCode"))
            if(parentData->availableChildrenData[i]["inviteCode"].IsString())
                currentChild["inviteCode"] = parentData->availableChildrenData[i]["inviteCode"].GetString();
        
        if(parentData->availableChildrenData[i].HasMember("sex"))
            if(parentData->availableChildrenData[i]["sex"].IsString())
            currentChild["sex"] = parentData->availableChildrenData[i]["sex"].GetString();
        
        if(parentData->availableChildrenData[i].HasMember("dob"))
            if(parentData->availableChildrenData[i]["dob"].IsString())
                currentChild["dob"] = parentData->availableChildrenData[i]["dob"].GetString();
        
        if(parentData->availableChildrenData[i].HasMember("id"))
            if(parentData->availableChildrenData[i]["id"].IsString())
                currentChild["id"] = parentData->availableChildrenData[i]["id"].GetString();
        
        parentData->availableChildren.push_back(currentChild);
        
        const std::string& childId = parentData->availableChildrenData[i]["id"].GetString();
        parentData->availableChildrenById[childId] = i;
    }
    
    return true;
}

void ParentDataParser::parseParentBillingData(std::string responseData)
{
    rapidjson::Document billingData;
    billingData.Parse(responseData.c_str());
    
    if(billingData.HasParseError())
    {
        CCLOG("Billing Parse Error");
        return;
    }
    
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    parentData->loggedInParentBillingDate = "";
    
    if(billingData.HasMember("billingStatus"))
    {
        if(billingData["billingStatus"].IsString())
        {
            CCLOG("Billing billingStatus OK");
            parentData->loggedInParentBillingStatus = billingData["billingStatus"].GetString();
            
            AnalyticsSingleton::getInstance()->registerBillingStatus(billingData["billingStatus"].GetString());
        }
    }
    
    //BillDate format "2017-04-04"
    if(billingData.HasMember("nextBillDate"))
    {
        if(billingData["nextBillDate"].IsString())
        {
            CCLOG("Billing nextBillDate OK");
            parentData->loggedInParentBillingDate = billingData["nextBillDate"].GetString();
        }
    }
    
    if(billingData.HasMember("paymentProvider"))
    {
        if(billingData["paymentProvider"].IsString())
        {
            CCLOG("Billing paymentProvider OK");
            parentData->loggedInParentBillingProvider = billingData["paymentProvider"].GetString();
            
            AnalyticsSingleton::getInstance()->registerBillingProvider(billingData["paymentProvider"].GetString());
        }
    }
}

void ParentDataParser::logoutChild()
{
    ChildDataStorage::getInstance()->childLoggedIn = false;
}

void ParentDataParser::addParentLoginDataToUserDefaults()
{
    cocos2d::log("ParentDataParser::addParentLoginDataToUserDefaults");
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("loggedInParentId", parentData->loggedInParentId);
    def->setStringForKey("loggedInParentCdnSessionId", parentData->loggedInParentCdnSessionId);
    def->setStringForKey("loggedInParentApiSecret", parentData->loggedInParentApiSecret);
    def->setStringForKey("loggedInParentApiKey", parentData->loggedInParentApiKey);
    def->setStringForKey("loggedInParentActorStatus", parentData->loggedInParentActorStatus);
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
    cocos2d::log("loggedInParentId = %s", parentData->loggedInParentId.c_str());
    cocos2d::log("loggedInParentCdnSessionId = %s", parentData->loggedInParentCdnSessionId.c_str());
    cocos2d::log("loggedInParentApiSecret = %s", parentData->loggedInParentApiSecret.c_str());
    cocos2d::log("loggedInParentApiKey = %s", parentData->loggedInParentApiKey.c_str());
    cocos2d::log("loggedInParentActorStatus = %s", parentData->loggedInParentActorStatus.c_str());
    
    createCrashlyticsUserInfo(parentData->loggedInParentId, "");
    AnalyticsSingleton::getInstance()->registerParentID(parentData->loggedInParentId);
    AnalyticsSingleton::getInstance()->registerAccountStatus(parentData->loggedInParentActorStatus);
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(def->getStringForKey("username"));
}

bool ParentDataParser::hasParentLoginDataInUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    if(def->getStringForKey("loggedInParentId") != "") return true;
    return false;
}

void ParentDataParser::clearParentLoginDataFromUserDefaults()
{
    cocos2d::log("ParentDataParser::clearParentLoginDataFromUserDefaults");
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("loggedInParentId", "");
    def->setStringForKey("loggedInParentCdnSessionId", "");
    def->setStringForKey("loggedInParentApiSecret", "");
    def->setStringForKey("loggedInParentApiKey", "");
    def->setStringForKey("loggedInParentActorStatus", "");
    def->flush();
}
    
bool ParentDataParser::parsePendingFriendRequests(std::string responseData)
{
    ParentDataStorage* parentData = ParentDataStorage::getInstance();
    parentData->pendingFriendRequests.clear();
    
    parentData->pendingFriendRequestData.Parse(responseData.c_str());
    
    for(int i = 0; i < parentData->pendingFriendRequestData.Size(); i++)
    {
        std::map<std::string, std::string> currentPendingFriendRequest;

        if(parentData->pendingFriendRequestData[i].HasMember("senderName"))
            if(parentData->pendingFriendRequestData[i]["senderName"].IsString())
                currentPendingFriendRequest["senderName"] = parentData->pendingFriendRequestData[i]["senderName"].GetString();
        
        if(parentData->pendingFriendRequestData[i].HasMember("friendName"))
            if(parentData->pendingFriendRequestData[i]["friendName"].IsString())
                currentPendingFriendRequest["friendName"] = parentData->pendingFriendRequestData[i]["friendName"].GetString();

        if(parentData->pendingFriendRequestData[i].HasMember("inviteeCode"))
            if(parentData->pendingFriendRequestData[i]["inviteeCode"].IsString())
                currentPendingFriendRequest["inviteeCode"] = parentData->pendingFriendRequestData[i]["inviteeCode"].GetString();
        
        if(parentData->pendingFriendRequestData[i].HasMember("id"))
            if(parentData->pendingFriendRequestData[i]["id"].IsString())
                currentPendingFriendRequest["id"] = parentData->pendingFriendRequestData[i]["id"].GetString();
        
        if(parentData->pendingFriendRequestData[i].HasMember("senderId"))
            if(parentData->pendingFriendRequestData[i]["senderId"].IsString())
                currentPendingFriendRequest["senderId"] = parentData->pendingFriendRequestData[i]["senderId"].GetString();
        
        if(parentData->pendingFriendRequestData[i].HasMember("respondentId"))
            if(parentData->pendingFriendRequestData[i]["respondentId"].IsString())
                currentPendingFriendRequest["respondentId"] = parentData->pendingFriendRequestData[i]["respondentId"].GetString();

        parentData->pendingFriendRequests.push_back(currentPendingFriendRequest);
    }
    
    return true;
}
  
}
