#include "HQDataObjectManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<HQDataObjectManager> _sharedHQDataObjectManager;

HQDataObjectManager* HQDataObjectManager::getInstance()
{
    if (! _sharedHQDataObjectManager.get())
    {
        _sharedHQDataObjectManager.reset(new HQDataObjectManager);
    }
    
    return _sharedHQDataObjectManager.get();
}

HQDataObjectManager::~HQDataObjectManager()
{
}

HQDataObjectManager::HQDataObjectManager()
{
}

HQDataObjectRef HQDataObjectManager::getHQDataObjectForKey(const std::string &key)
{
    if(_hqDataObjects.find(key) == _hqDataObjects.end())
    {
        _hqDataObjects[key] = HQDataObject::create();
    }
    
    return _hqDataObjects[key];
}

void HQDataObjectManager::clearAllHQData()
{
    _hqDataObjects.clear();
}

bool HQDataObjectManager::isSameHQData(const std::string &etag) const
{
    return _currentHQDataEtag == etag;
}

void HQDataObjectManager::setHQDataEtag(const std::string &etag)
{
    _currentHQDataEtag = etag;
}

NS_AZOOMEE_END
