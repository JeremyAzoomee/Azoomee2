#include "HQDataObjectStorage.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<HQDataObjectStorage> _sharedHQDataObjectStorage;

HQDataObjectStorage* HQDataObjectStorage::getInstance()
{
    if (! _sharedHQDataObjectStorage.get())
    {
        _sharedHQDataObjectStorage.reset(new HQDataObjectStorage);
    }
    
    return _sharedHQDataObjectStorage.get();
}

HQDataObjectStorage::~HQDataObjectStorage()
{
}

HQDataObjectStorage::HQDataObjectStorage()
{
}

HQDataObject HQDataObjectStorage::getHQDataObjectForKey(std::string key)
{
    if(hqDataObjects.find(key) == hqDataObjects.end())
    {
        hqDataObjects[key] = HQDataObject::create();
    }
    
    return hqDataObjects[key];
}

void HQDataObjectStorage::clearAllHQData()
{
    hqDataObjects.clear();
}

NS_AZOOMEE_END
