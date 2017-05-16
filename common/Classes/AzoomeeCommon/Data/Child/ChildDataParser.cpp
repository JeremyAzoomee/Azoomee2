#include "ChildDataParser.h"
#include "ChildDataStorage.h"
#include "../../Crashlytics/CrashlyticsConfig.h"
#include "../Parent/ParentDataProvider.h"
#include "../../Analytics/AnalyticsSingleton.h"

using namespace cocos2d;
using namespace Azoomee;

namespace Azoomee
{

static ChildDataParser *_sharedChildDataParser = NULL;

ChildDataParser* ChildDataParser::getInstance()
{
    if (! _sharedChildDataParser)
    {
        _sharedChildDataParser = new ChildDataParser();
        _sharedChildDataParser->init();
    }
    
    return _sharedChildDataParser;
}

ChildDataParser::~ChildDataParser(void)
{
}

bool ChildDataParser::init(void)
{
    ChildDataStorage::getInstance()->childLoggedIn = false;
    return true;
}

bool ChildDataParser::parseChildLoginData(std::string responseData)
{
    ChildDataStorage::getInstance()->childLoggedIn = true;
    ChildDataStorage::getInstance()->childLoginData.Parse(responseData.c_str());
    
    setLoggedInChildId(ChildDataStorage::getInstance()->childLoginData["id"].GetString());
    
    ChildDataStorage::getInstance()->loggedInChildCdnSessionId = ChildDataStorage::getInstance()->childLoginData["cdn-sessionid"].GetString();
    ChildDataStorage::getInstance()->loggedInChildApiSecret = ChildDataStorage::getInstance()->childLoginData["apiSecret"].GetString();
    ChildDataStorage::getInstance()->loggedInChildApiKey = ChildDataStorage::getInstance()->childLoginData["apiKey"].GetString();
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("lastLoggedInChildId", ChildDataStorage::getInstance()->loggedInChildId);
    def->flush();
    
    createCrashlyticsUserInfo(ParentDataProvider::getInstance()->getLoggedInParentId(), ChildDataStorage::getInstance()->loggedInChildId);
    
    AnalyticsSingleton::getInstance()->registerChildID(ChildDataStorage::getInstance()->loggedInChildId);
    
    return true;
}

void ChildDataParser::parseOomeeData(std::string responseData)
{
    rapidjson::Document hqData;
    hqData.Parse(responseData.c_str());
    
    setLoggedInChildName(hqData["oomee"]["name"].GetString());
    setLoggedInChildAvatarId(hqData["oomee"]["avatar"].GetString());
}

void ChildDataParser::setLoggedInChildName(std::string childName)
{
    ChildDataStorage::getInstance()->loggedInChildName = childName;
}

void ChildDataParser::setLoggedInChildAvatarId(std::string avatarId)
{
    ChildDataStorage::getInstance()->loggedInChildAvatarId = avatarId;
}

void ChildDataParser::setLoggedInChildId(std::string id)
{
    ChildDataStorage* data = ChildDataStorage::getInstance();
    data->loggedInChildId = id;
    if(id.empty())
        data->loggedInChildName = "";
    else
        data->loggedInChildName = ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildrenById(data->loggedInChildId);
}

void ChildDataParser::setLoggedInChildNumber(int childNumber)
{
    ChildDataStorage::getInstance()->loggedInChildNumber = childNumber;
}

void ChildDataParser::setChildLoggedIn(bool loggedIn)
{
    ChildDataStorage::getInstance()->childLoggedIn = loggedIn;
}
  
}
