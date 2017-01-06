#include "ChildDataStorage.h"

using namespace cocos2d;

static ChildDataStorage *_sharedChildDataStorage = NULL;

ChildDataStorage* ChildDataStorage::getInstance()
{
    if (! _sharedChildDataStorage)
    {
        _sharedChildDataStorage = new ChildDataStorage();
        _sharedChildDataStorage->init();
    }
    
    return _sharedChildDataStorage;
}

ChildDataStorage::~ChildDataStorage(void)
{
}

bool ChildDataStorage::init(void)
{
    childLoggedIn = false;
    
    return true;
}
