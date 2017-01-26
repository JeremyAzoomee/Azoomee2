#include "ParentDataParser.h"
#include "ChildDatastorage.h"
#include "ParentDataStorage.h"

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
    ChildDataStorage::getInstance()->childLoggedIn = false;
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
            
            return true;
        }
    }
    
    return false;
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
        
        ParentDataStorage::getInstance()->availableChildren.push_back(currentChild);
    }
    
    return true;
}
