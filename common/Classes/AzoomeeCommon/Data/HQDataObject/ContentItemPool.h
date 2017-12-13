#ifndef Azoomee_ContentItemPool_h
#define Azoomee_ContentItemPool_h

#include "../../Azoomee.h"
#include <cocos/cocos2d.h>
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class ContentItemPool;
typedef std::shared_ptr<ContentItemPool> ContentItemPoolRef;

class ContentItemPool
{
private:
    std::map<std::string, HQContentItemObjectRef> _contentItems;
    
public:
    ContentItemPool();
    static ContentItemPoolRef create();
    
    void addContentItemToPool(const HQContentItemObjectRef &contentItem);
    void emptyContentItemPool();
    
    std::vector<HQContentItemObjectRef> getContentItems();
    HQContentItemObjectRef getContentItemForId(const std::string &contentId);
};

NS_AZOOMEE_END

#endif
