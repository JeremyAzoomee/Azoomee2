#include "ParentDataParser.h"
#include "ChildDataParser.h"

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
    ChildDataParser::getInstance()->childLoggedIn = false;
    parentLoginData.Parse(responseData.c_str());
    
    if(parentLoginData.HasMember("code"))
    {
        if(parentLoginData["code"] != "INVALID_CREDENTIALS")
        {
            return true;
        }
    }
    
    return false;
}

bool ParentDataParser::parseAvailableChildren(std::string responseData)
{
    availableChildrenData.Parse(responseData.c_str());
    return true;
}
