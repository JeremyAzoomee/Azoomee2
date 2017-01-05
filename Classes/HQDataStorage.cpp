#include "HQDataStorage.h"

using namespace cocos2d;

static HQDataStorage *_sharedHQDataStorage = NULL;

HQDataStorage* HQDataStorage::getInstance()
{
    if (! _sharedHQDataStorage)
    {
        _sharedHQDataStorage = new HQDataStorage();
        _sharedHQDataStorage->init();
    }
    
    return _sharedHQDataStorage;
}

HQDataStorage::~HQDataStorage(void)
{
}

bool HQDataStorage::init(void)
{
    return true;
}
