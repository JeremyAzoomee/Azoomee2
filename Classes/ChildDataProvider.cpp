#include "ChildDataProvider.h"
#include "ChildDataParser.h"
#include "ParentDataParser.h"

using namespace cocos2d;

static ChildDataProvider *_sharedChildDataProvider = NULL;

ChildDataProvider* ChildDataProvider::getInstance()
{
    if (! _sharedChildDataProvider)
    {
        _sharedChildDataProvider = new ChildDataProvider();
        _sharedChildDataProvider->init();
    }
    
    return _sharedChildDataProvider;
}

ChildDataProvider::~ChildDataProvider(void)
{
}

bool ChildDataProvider::init(void)
{
    return true;
}

std::string ChildDataProvider::getChildLoginValue(std::string keyName)
{
    return ChildDataParser::getInstance()->childLoginData[keyName.c_str()].GetString();
}

std::string ChildDataProvider::getParentOrChildLoginValue(std::string keyName) //if no child logged in, falls back to parent data
{
    if(ChildDataParser::getInstance()->childLoggedIn)
    {
        return ChildDataParser::getInstance()->childLoginData[keyName.c_str()].GetString();
    }
    else
    {
        return ParentDataParser::getInstance()->parentLoginData[keyName.c_str()].GetString();
    }
}
