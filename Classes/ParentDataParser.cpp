#include "ParentDataParser.h"
#include "ChildDatastorage.h"
#include "ParentDataStorage.h"
#include "ModalMessages.h"
#include "LoginScene.h"
#include "CrashLyticsConfig.h"
#include "AnalyticsSingleton.h"

#include "HQDataStorage.h"

using namespace cocos2d;

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
    ParentDataStorage::getInstance()->parentLoginData.Parse(responseData.c_str());
    
    if(ParentDataStorage::getInstance()->parentLoginData.HasMember("code"))
    {
        if(ParentDataStorage::getInstance()->parentLoginData["code"] != "INVALID_CREDENTIALS")
        {
            ParentDataStorage::getInstance()->loggedInParentId = ParentDataStorage::getInstance()->parentLoginData["id"].GetString();
            ParentDataStorage::getInstance()->loggedInParentCdnSessionId = ParentDataStorage::getInstance()->parentLoginData["cdn-sessionid"].GetString();
            ParentDataStorage::getInstance()->loggedInParentApiSecret = ParentDataStorage::getInstance()->parentLoginData["apiSecret"].GetString();
            ParentDataStorage::getInstance()->loggedInParentApiKey = ParentDataStorage::getInstance()->parentLoginData["apiKey"].GetString();
            ParentDataStorage::getInstance()->loggedInParentActorStatus = ParentDataStorage::getInstance()->parentLoginData["actorStatus"].GetString();
            
            if(ParentDataStorage::getInstance()->parentLoginData.HasMember("pinNumber"))
            {
                if(ParentDataStorage::getInstance()->parentLoginData["pinNumber"].IsString())
                {
                    ParentDataStorage::getInstance()->loggedInParentPin = ParentDataStorage::getInstance()->parentLoginData["pinNumber"].GetString();
                }
                else
                {
                    ParentDataStorage::getInstance()->loggedInParentPin = "";
                }
            }
            else
            {
                ParentDataStorage::getInstance()->loggedInParentPin = "";
            }
            
            addParentLoginDataToUserDefaults();
            
            HQDataStorage::getInstance()->HQListTitles.clear();
            HQDataStorage::getInstance()->HQListElements.clear();
            HQDataStorage::getInstance()->HQElementHighlights.clear();
            HQDataStorage::getInstance()->HQData.clear();
            HQDataStorage::getInstance()->HQGetContentUrls.clear();
            
            createCrashlyticsUserInfo(ParentDataStorage::getInstance()->loggedInParentId, "");
            AnalyticsSingleton::getInstance()->registerParentID(ParentDataStorage::getInstance()->loggedInParentId);
            AnalyticsSingleton::getInstance()->registerAccountStatus(ParentDataStorage::getInstance()->loggedInParentActorStatus);
            
            return true;
        }
        else
        {
            auto loginScene = LoginScene::createScene(ERROR_CODE_INVALID_CREDENTIALS);
            Director::getInstance()->replaceScene(loginScene);
        }
    }
    
    return false;
}

bool ParentDataParser::parseUpdateParentData(std::string responseData)
{
    rapidjson::Document updateData;
    updateData.Parse(responseData.c_str());
    
    if(updateData.HasMember("pinNumber"))
    {
        if(updateData["pinNumber"].IsString())
        {
            ParentDataStorage::getInstance()->loggedInParentPin = updateData["pinNumber"].GetString();
        }
        else
        {
            ParentDataStorage::getInstance()->loggedInParentPin = "";
        }
    }
    else
    {
        ParentDataStorage::getInstance()->loggedInParentPin = "";
    }
    
    if(updateData.HasMember("actorStatus")) ParentDataStorage::getInstance()->loggedInParentActorStatus = updateData["actorStatus"].GetString();
    else return false;
    
    return true;
}

bool ParentDataParser::parseAvailableChildren(std::string responseData)
{
    ParentDataStorage::getInstance()->availableChildren.clear();
    
    ParentDataStorage::getInstance()->availableChildrenData.Parse(responseData.c_str());
    
    for(int i = 0; i < ParentDataStorage::getInstance()->availableChildrenData.Size(); i++)
    {
        std::map<std::string, std::string> currentChild;
        currentChild["profileName"] = ParentDataStorage::getInstance()->availableChildrenData[i]["profileName"].GetString();
        currentChild["avatar"] = ParentDataStorage::getInstance()->availableChildrenData[i]["avatar"].GetString();

        if(ParentDataStorage::getInstance()->availableChildrenData[i].HasMember("sex"))
            if(ParentDataStorage::getInstance()->availableChildrenData[i]["sex"].IsString())
            currentChild["sex"] = ParentDataStorage::getInstance()->availableChildrenData[i]["sex"].GetString();
        
        if(ParentDataStorage::getInstance()->availableChildrenData[i].HasMember("dob"))
            if(ParentDataStorage::getInstance()->availableChildrenData[i]["dob"].IsString())
                currentChild["dob"] = ParentDataStorage::getInstance()->availableChildrenData[i]["dob"].GetString();
        
        ParentDataStorage::getInstance()->availableChildren.push_back(currentChild);
    }
    
    return true;
}

void ParentDataParser::parseParentBillingData(std::string responseData)
{
    rapidjson::Document billingData;
    billingData.Parse(responseData.c_str());
    
    if(billingData.HasParseError()) return;
    
    ParentDataStorage::getInstance()->loggedInParentBillingDate = "";
    
    if(billingData.HasMember("billingStatus"))
    {
        if(billingData["billingStatus"].IsString())
        {
            ParentDataStorage::getInstance()->loggedInParentBillingStatus = billingData["billingStatus"].GetString();
            
        }
    }
    
    //BillDate format "2017-04-04"
    if(billingData.HasMember("nextBillDate"))
    {
        if(billingData["nextBillDate"].IsString())
        {
            ParentDataStorage::getInstance()->loggedInParentBillingDate = billingData["nextBillDate"].GetString();
            
        }
    }
    
    if(billingData.HasMember("paymentProvider"))
    {
        if(billingData["nextBillDate"].IsString())
        {
            ParentDataStorage::getInstance()->loggedInParentBillingProvider = billingData["paymentProvider"].GetString();
            
        }
    }
}

void ParentDataParser::logoutChild()
{
    ChildDataStorage::getInstance()->childLoggedIn = false;
}

void ParentDataParser::addParentLoginDataToUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("loggedInParentId", ParentDataStorage::getInstance()->loggedInParentId);
    def->setStringForKey("loggedInParentCdnSessionId", ParentDataStorage::getInstance()->loggedInParentCdnSessionId);
    def->setStringForKey("loggedInParentApiSecret", ParentDataStorage::getInstance()->loggedInParentApiSecret);
    def->setStringForKey("loggedInParentApiKey", ParentDataStorage::getInstance()->loggedInParentApiKey);
    def->setStringForKey("loggedInParentActorStatus", ParentDataStorage::getInstance()->loggedInParentActorStatus);
    def->flush();
}

void ParentDataParser::retrieveParentLoginDataFromUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    ParentDataStorage::getInstance()->loggedInParentId = def->getStringForKey("loggedInParentId");
    ParentDataStorage::getInstance()->loggedInParentCdnSessionId = def->getStringForKey("loggedInParentCdnSessionId");
    ParentDataStorage::getInstance()->loggedInParentApiSecret = def->getStringForKey("loggedInParentApiSecret");
    ParentDataStorage::getInstance()->loggedInParentApiKey = def->getStringForKey("loggedInParentApiKey");
    ParentDataStorage::getInstance()->loggedInParentActorStatus = def->getStringForKey("loggedInParentActorStatus");
}

bool ParentDataParser::hasParentLoginDataInUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    if(def->getStringForKey("loggedInParentId") != "") return true;
    return false;
}
