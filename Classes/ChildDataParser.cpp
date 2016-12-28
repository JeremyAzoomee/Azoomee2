#include "ChildDataParser.h"

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
    childLoggedIn = false;
    
    return true;
}

bool ChildDataParser::parseChildLoginData(std::string responseData)
{
    childLoggedIn = true;
    childLoginData.Parse(responseData.c_str());
    return true;
}
