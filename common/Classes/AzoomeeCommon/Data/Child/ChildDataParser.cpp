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

bool ChildDataParser::parseChildLoginData(const std::string &responseData)
{
    auto childDataStorage = ChildDataStorage::getInstance();
    
    childDataStorage->childLoginData.Parse(responseData.c_str());
    if(childDataStorage->childLoginData.HasParseError())
    {
        return false;
    }
    
    childDataStorage->loggedInChildCdnSessionId = getStringFromJson("cdn-sessionid", childDataStorage->childLoginData);
    childDataStorage->loggedInChildApiSecret = getStringFromJson("apiSecret", childDataStorage->childLoginData);
    childDataStorage->loggedInChildApiKey = getStringFromJson("apiKey", childDataStorage->childLoginData);
    
    setLoggedInChildId(getStringFromJson("id", childDataStorage->childLoginData));
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("lastLoggedInChildId", childDataStorage->loggedInChildId);
    def->flush();
    
    createCrashlyticsUserInfo(ParentDataProvider::getInstance()->getLoggedInParentId(), childDataStorage->loggedInChildId);
    
    AnalyticsSingleton::getInstance()->registerChildID(childDataStorage->loggedInChildId);
    
    childDataStorage->childLoggedIn = true;
    return true;
}

void ChildDataParser::parseOomeeData(const std::string &responseData)
{
    rapidjson::Document hqData;
    hqData.Parse(responseData.c_str());
    
    if(hqData.HasParseError() || !hqData.HasMember("oomee"))
    {
        return;
    }
    
    const rapidjson::Value &oomeeData = hqData["oomee"];
    
    setLoggedInChildName(getStringFromJson("name", oomeeData));
    setLoggedInChildAvatarId(getStringFromJson("avatar", oomeeData));
}

void ChildDataParser::setLoggedInChildName(const std::string &childName)
{
    ChildDataStorage::getInstance()->loggedInChildName = childName;
}

void ChildDataParser::setLoggedInChildAvatarId(const std::string &avatarId)
{
    ChildDataStorage::getInstance()->loggedInChildAvatarId = avatarId;
}

void ChildDataParser::setLoggedInChildId(const std::string &id)
{
    ChildDataStorage* data = ChildDataStorage::getInstance();
    data->loggedInChildId = id;
    if(id.empty())
    {
        data->loggedInChildName = "";
        data->loggedInChildAvatarId = "";
    }
    else
    {
        data->loggedInChildName = ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildById(data->loggedInChildId);
        data->loggedInChildAvatarId = ParentDataProvider::getInstance()->getAvatarForAnAvailableChildById(data->loggedInChildId);
    }
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
