#ifndef Azoomee_ContentItemManager_h
#define Azoomee_ContentItemManager_h

#include "../../Azoomee.h"
#include <cocos/cocos2d.h>
#include "HQContentItemObject.h"
#include "../Json.h"

NS_AZOOMEE_BEGIN

class ContentItemManager
{
public:
    static ContentItemManager* getInstance();
    virtual ~ContentItemManager();
    
    void addContentItemToPool(const MutableHQContentItemObjectRef &contentItem);
    void emptyContentItemPool();

    bool contentExistsForKey(const std::string &contentId) const;
    std::vector<HQContentItemObjectRef> getContentItems() const;
    HQContentItemObjectRef getContentItemForId(const std::string &contentId) const;
    HQContentItemObjectRef getParentOfContentItemForId(const std::string &contentId) const;
    
    bool isSameContentPool(const std::string& etag) const;
    void setPoolEtag(const std::string& etag);
	
	void parseContentItemPool(const std::string& ContentItemManagerString);
	void parseContentItem(const std::string& contentId, const rapidjson::Value& contentItemData);
	void parseContentItemURI(const std::string& contentId, const std::string& uri);
	void parseEntitlementData(const std::string &entitlementData);
	
private:
    ContentItemManager();
    /// Maps a content id to a content item
    std::map<std::string, MutableHQContentItemObjectRef> _contentItems;
    /// Maps a content id to it's parent (group) content item, if there is one
    std::map<std::string, MutableHQContentItemObjectRef> _contentItemToGroup;
    std::string _currentPoolEtag;
};

NS_AZOOMEE_END

#endif
