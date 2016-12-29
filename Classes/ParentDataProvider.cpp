#include "ParentDataProvider.h"
#include "ParentDataStorage.h"

using namespace cocos2d;

static ParentDataProvider *_sharedParentDataProvider = NULL;

ParentDataProvider* ParentDataProvider::getInstance()
{
    if (! _sharedParentDataProvider)
    {
        _sharedParentDataProvider = new ParentDataProvider();
        _sharedParentDataProvider->init();
    }
    
    return _sharedParentDataProvider;
}

ParentDataProvider::~ParentDataProvider(void)
{
}

bool ParentDataProvider::init(void)
{
    return true;
}

std::string ParentDataProvider::getParentLoginValue(std::string keyName)
{
    return ParentDataStorage::getInstance()->parentLoginData[keyName.c_str()].GetString();
}

std::string ParentDataProvider::getAvailableChildrenValue(std::string keyName)
{
    return ParentDataStorage::getInstance()->availableChildrenData[keyName.c_str()].GetString();
}

int ParentDataProvider::getAmountOfAvailableChildren()
{
    return (int)ParentDataStorage::getInstance()->availableChildrenData.Size();
}

std::string ParentDataProvider::getValueFromOneAvailableChild(int childNumber, std::string keyName)
{
    return ParentDataStorage::getInstance()->availableChildrenData[childNumber][keyName.c_str()].GetString();
}
