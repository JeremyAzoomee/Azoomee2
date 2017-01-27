#include "ParentDataParser.h"
#include "ChildDatastorage.h"
#include "ParentDataStorage.h"
#include "ModalMessages.h"
#include "LoginScene.h"

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
            
            HQDataStorage::getInstance()->HQListTitles.clear();
            HQDataStorage::getInstance()->HQListElements.clear();
            HQDataStorage::getInstance()->HQElementHighlights.clear();
            HQDataStorage::getInstance()->HQData.clear();
            HQDataStorage::getInstance()->HQGetContentUrls.clear();
            
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
