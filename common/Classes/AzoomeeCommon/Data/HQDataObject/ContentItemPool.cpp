#include "ContentItemPool.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<ContentItemPool> _sharedContentItemPool;

ContentItemPool* ContentItemPool::getInstance()
{
    if (! _sharedContentItemPool.get())
    {
        _sharedContentItemPool.reset(new ContentItemPool);
    }
    
    return _sharedContentItemPool.get();
}

ContentItemPool::~ContentItemPool()
{
}

ContentItemPool::ContentItemPool()
{
}

void ContentItemPool::addContentItemToPool(const HQContentItemObjectRef &contentItem)
{
    _contentItems[contentItem->getContentItemId()] = contentItem;
}

void ContentItemPool::emptyContentItemPool()
{
    _contentItems.clear();
}

bool ContentItemPool::contentExistsForKey(const std::string &contentId)
{
    if(_contentItems.find(contentId) != _contentItems.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<HQContentItemObjectRef> ContentItemPool::getContentItems()
{
    std::vector<HQContentItemObjectRef> returnArray;
    
    for(std::map<std::string, HQContentItemObjectRef>::iterator item = _contentItems.begin(); item != _contentItems.end(); ++item)
    {
        returnArray.push_back(item->second);
    }
    
    return returnArray;
}

HQContentItemObjectRef ContentItemPool::getContentItemForId(const std::string &contentId)
{
    if(contentExistsForKey(contentId))
    {
        return _contentItems.at(contentId);
    }
    else
    {
        return nullptr;
    }
}

NS_AZOOMEE_END
