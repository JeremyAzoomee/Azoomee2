#include "ChildDataParser.h"
#include "ChildDataStorage.h"

using namespace cocos2d;

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
    return true;
}

void ChildDataParser::setLoggedInChildName(std::string childName)
{
    ChildDataStorage::getInstance()->loggedInChildName = childName;
}

void ChildDataParser::setLoggedInChildNumber(int childNumber)
{
    ChildDataStorage::getInstance()->loggedInChildNumber = childNumber;
}
