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

HQDataObjectRef HQDataObjectStorage::getHQDataObjectForKey(const std::string &key)
{
    if(_hqDataObjects.find(key) == _hqDataObjects.end())
    {
        _hqDataObjects[key] = HQDataObject::create();
    }
    
    return _hqDataObjects[key];
}

void HQDataObjectStorage::clearAllHQData()
{
    _hqDataObjects.clear();
}

bool HQDataObjectStorage::isSameHQData(const std::string &etag)
{
    return _currentHQDataEtag == etag;
}

void HQDataObjectStorage::setHQDataEtag(const std::string &etag)
{
    _currentHQDataEtag = etag;
}

NS_AZOOMEE_END
