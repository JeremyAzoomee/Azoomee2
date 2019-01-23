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
    
	rapidjson::Document data;
	data.Parse(responseData.c_str());
    if(data.HasParseError())
    {
        return false;
    }
	
	ChildRef child = ParentDataProvider::getInstance()->getChildForId(getStringFromJson("id", data));
	if(!child)
	{
		return false;
	}
	
	child->parseLoginData(data);
    
	childDataStorage->_loggedInChild = child;
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("lastLoggedInChildId", child->getId());
    def->flush();
    
    createCrashlyticsUserInfo(ParentDataProvider::getInstance()->getLoggedInParentId(), child->getId());
    
    childDataStorage->childLoggedIn = true;
    return true;
}

void ChildDataParser::setChildLoggedIn(bool loggedIn)
{
    ChildDataStorage::getInstance()->childLoggedIn = loggedIn;
}

void ChildDataParser::loginChildOffline(const std::string &childId)
	{
		auto childDataStorage = ChildDataStorage::getInstance();
		childDataStorage->childLoggedIn = true;
		ChildRef offlineChild = Child::create();
		offlineChild->setId(childId);
		childDataStorage->_loggedInChild = offlineChild;
	}
}

void ChildDataParser::loginAnonChild(const std::string& loginDataStr)
{
	const std::string& localChildPath = FileUtils::getInstance()->getWritablePath() + "anonLocalChild/childData.json";
	if(!FileUtils::getInstance()->isFileExist(localChildPath))
	{
		return;
	}
	
	auto childDataStorage = ChildDataStorage::getInstance();
	
	rapidjson::Document childData;
	childData.Parse(FileUtils::getInstance()->getStringFromFile(localChildPath).c_str());
	if(childData.HasParseError())
	{
		return;
	}
	
	ChildRef child = Child::createWithJson(childData);
	
	rapidjson::Document loginData;
	loginData.Parse(loginDataStr.c_str());
	if(loginData.HasParseError())
	{
		return;
	}
	
	child->parseLoginData(loginData);
	
	child->setId(getStringFromJson("id",loginData));
	childDataStorage->_loggedInChild = child;
	childDataStorage->childLoggedIn = true;
}
