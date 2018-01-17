#ifndef Azoomee_ContentItemPool_h
#define Azoomee_ContentItemPool_h

#include "../../Azoomee.h"
#include <cocos/cocos2d.h>
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class ContentItemPool
{
public:
    static ContentItemPool* getInstance();
    virtual ~ContentItemPool();
    
    void addContentItemToPool(const HQContentItemObjectRef &contentItem);
    void emptyContentItemPool();

    bool contentExistsForKey(const std::string &contentId) const;
    std::vector<HQContentItemObjectRef> getContentItems() const;
    HQContentItemObjectRef getContentItemForId(const std::string &contentId) const;
    
    void backupContentItemPool();
    void restoreContentItemPool();
    
private:
    ContentItemPool();
    std::map<std::string, HQContentItemObjectRef> _contentItems;
};

NS_AZOOMEE_END

#endif
