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
    std::vector<HQContentItemObjectRef> getContentItemsFromIDs(const std::vector<std::string> &itemidList) const;
    
    /// Returns a pair of vectors.
    /// The first is the content filtered to only show 1 content item per group.
    /// The second contains the group of the last played content if available, or if no group is available, the content item itself.
    std::pair<std::vector<HQContentItemObjectRef>, std::vector<HQContentItemObjectRef>> filterContentItemsByUniqueGroup(const std::vector<HQContentItemObjectRef>& items) const;
    
    std::string getThumbnailUrlForItem(const std::string &itemId) const;
    std::string getThumbnailUrlForItem(HQContentItemObjectRef element, const cocos2d::Vec2 &shape) const;
    
    bool isSameContentPool(const std::string& etag) const;
    void setPoolEtag(const std::string& etag);
	
	void parseContentItemPool(const std::string& ContentItemManagerString);
	void parseContentItem(const std::string& contentId, const rapidjson::Value& contentItemData);
	void parseContentItemURI(const std::string& contentId, const std::string& uri);
	void parseEntitlementData(const std::string &entitlementData);
	
private:
    ContentItemManager();
    
    std::string convertShapeToThumbnailKey(const cocos2d::Vec2 &shape) const;
    
    /// Maps a content id to a content item
    std::map<std::string, MutableHQContentItemObjectRef> _contentItems;
    /// Maps a content id to it's parent (group) content item, if there is one
    std::map<std::string, MutableHQContentItemObjectRef> _contentItemToGroup;
    std::string _currentPoolEtag;
};

NS_AZOOMEE_END

#endif
