#include "ContentItemPool.h"

NS_AZOOMEE_BEGIN

ContentItemPoolRef ContentItemPool::create()
{
    return std::make_shared<ContentItemPool>();
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
    if(_contentItems.find(contentId) == _contentItems.end())
    {
        return nullptr;
    }
    
    return _contentItems.at(contentId);
}

NS_AZOOMEE_END
