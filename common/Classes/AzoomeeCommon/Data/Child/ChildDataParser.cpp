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
    ChildDataStorage::getInstance()->setChildLoggedIn(false);
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
    
	childDataStorage->setLoggedInChild(child);
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("lastLoggedInChildId", child->getId());
    def->flush();
    
    createCrashlyticsUserInfo(ParentDataProvider::getInstance()->getLoggedInParentId(), child->getId());
    
    childDataStorage->setChildLoggedIn(true);
    return true;
}

void ChildDataParser::setChildLoggedIn(bool loggedIn)
{
    ChildDataStorage::getInstance()->setChildLoggedIn(loggedIn);
}

void ChildDataParser::loginChildOffline(const std::string &childId)
{
	auto childDataStorage = ChildDataStorage::getInstance();
	childDataStorage->setChildLoggedIn(true);
	ChildRef offlineChild = Child::create();
	offlineChild->setId(childId);
	childDataStorage->setLoggedInChild(offlineChild);
}
}

