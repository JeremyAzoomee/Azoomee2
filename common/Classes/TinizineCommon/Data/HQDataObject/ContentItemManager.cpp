#include "ContentItemManager.h"
#include "../Child/ChildManager.h"
#include "../../Utils/StringFunctions.h"

using namespace cocos2d;

NS_TZ_BEGIN

static std::auto_ptr<ContentItemManager> _sharedContentItemManager;

ContentItemManager* ContentItemManager::getInstance()
{
    if (! _sharedContentItemManager.get())
    {
        _sharedContentItemManager.reset(new ContentItemManager);
    }
    
    return _sharedContentItemManager.get();
}

ContentItemManager::~ContentItemManager()
{
}

ContentItemManager::ContentItemManager()
{
}

void ContentItemManager::addContentItemToPool(const MutableHQContentItemObjectRef &contentItem)
{
    _contentItems[contentItem->getContentItemId()] = contentItem;
    
    const auto& subItems = contentItem->getItems();
    if(!subItems.empty())
    {
        for(const std::string& id : subItems)
        {
            _contentItemToGroup[id] = contentItem;
        }
    }
}

void ContentItemManager::emptyContentItemPool()
{
    _contentItems.clear();
}

bool ContentItemManager::contentExistsForKey(const std::string &contentId) const
{
	return _contentItems.find(contentId) != _contentItems.end();
}

std::vector<HQContentItemObjectRef> ContentItemManager::getContentItems() const
{
    std::vector<HQContentItemObjectRef> returnArray;
    
    for(auto item : _contentItems)
    {
        returnArray.push_back(item.second);
    }
    
    return returnArray;
}

HQContentItemObjectRef ContentItemManager::getContentItemForId(const std::string &contentId) const
{
    auto it = _contentItems.find(contentId);
    if(it != _contentItems.end())
    {
        return it->second;
    }
    return nullptr;
}

HQContentItemObjectRef ContentItemManager::getParentOfContentItemForId(const std::string &contentId) const
{
    auto it = _contentItemToGroup.find(contentId);
    if(it != _contentItemToGroup.end())
    {
        return it->second;
    }
    return nullptr;
}

std::vector<HQContentItemObjectRef> ContentItemManager::getContentItemsFromIDs(const std::vector<std::string> &itemidList) const
{
    std::vector<HQContentItemObjectRef> items;
    
    for(const std::string& id : itemidList)
    {
        HQContentItemObjectRef content = getContentItemForId(id);
        if(content)
        {
            items.push_back(content);
        }
    }
    
    return items;
}

std::pair<std::vector<HQContentItemObjectRef>, std::vector<HQContentItemObjectRef>> ContentItemManager::filterContentItemsByUniqueGroup(const std::vector<HQContentItemObjectRef>& items) const
{
    std::vector<HQContentItemObjectRef> filteredItems;
    // Keep track of what groups we have added
    std::vector<HQContentItemObjectRef> itemGroups;
    
    ContentItemManager* contentItemManager = ContentItemManager::getInstance();
    for(const HQContentItemObjectRef& content : items)
    {
        // Check if the item has a group
        const HQContentItemObjectRef& groupForContent = contentItemManager->getParentOfContentItemForId(content->getContentItemId());
        
        // If the item doesn't have a group, or we haven't added an item from this group yet
        if(!groupForContent || std::find(itemGroups.begin(), itemGroups.end(), groupForContent) == itemGroups.end())
        {
            filteredItems.push_back(content);
            itemGroups.push_back(groupForContent ? groupForContent : content);
        }
    }
    
    return std::make_pair(filteredItems, itemGroups);
}

std::string ContentItemManager::getThumbnailUrlForItem(const std::string &itemId) const
{
    HQContentItemObjectRef element = getContentItemForId(itemId);
    if(element)
    {
        return getThumbnailUrlForItem(element, Vec2(1,1));
    }
    
    return "";
}

std::string ContentItemManager::getThumbnailUrlForItem(HQContentItemObjectRef element, const cocos2d::Vec2 &shape) const
{
    if(element == nullptr)
    {
        return "";
    }
    
    const std::string &key = convertShapeToThumbnailKey(shape);
    const auto& images = element->getImages();
    if(images.find(key) != images.end())
    {
        return images.at(key);
    }
    else if(images.find(convertShapeToThumbnailKey(Vec2(1,1))) != images.end()) //if the queried key does not exist in images map, we try to fall back to ONE_ONE first
    {
        return images.at(convertShapeToThumbnailKey(Vec2(1,1)));
    }
    else //if ONE_ONE even does not exist, we return an empty string
    {
        return "";
    }
}

std::string ContentItemManager::convertShapeToThumbnailKey(const cocos2d::Vec2 &shape) const
{
    static const std::vector<std::string> &numbersByWords = {"ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE"};
    return numbersByWords.at(shape.x) + "_" + numbersByWords.at(shape.y);
}

bool ContentItemManager::isSameContentPool(const std::string &etag) const
{
    return _currentPoolEtag == etag;
}

void ContentItemManager::setPoolEtag(const std::string &etag)
{
    _currentPoolEtag = etag;
}

void ContentItemManager::parseContentItemPool(const std::string& contentItemPoolString)
{
	rapidjson::Document contentData;
	contentData.Parse(contentItemPoolString.c_str());
	if(contentData.HasParseError())
	{
		return;
	}
	
	if(contentData.IsObject())
	{
		if(contentData.HasMember("items"))
		{
			const rapidjson::Value& itemList = contentData["items"];
			if(itemList.IsArray())
			{
				for (auto M = itemList.MemberBegin(); M != itemList.MemberEnd(); M++)
				{
					parseContentItem(M->name.GetString(), M->value);
				}
			}
		}
		else
		{
			for (auto M = contentData.MemberBegin(); M != contentData.MemberEnd(); M++)
			{
				parseContentItem(M->name.GetString(), M->value);
			}
		}
	}
	else
	{
		if(contentData.IsArray())
		{
			for (auto M = contentData.MemberBegin(); M != contentData.MemberEnd(); M++)
			{
				parseContentItem(M->name.GetString(), M->value);
			}
		}
	}
}

void ContentItemManager::parseContentItem(const std::string &contentId, const rapidjson::Value &contentItemData)
{
	MutableHQContentItemObjectRef contentObject = MutableHQContentItemObject::create();
	
	contentObject->setContentItemId(contentId);
	contentObject->setTitle(getStringFromJson("title", contentItemData));
	contentObject->setDescription(getStringFromJson("description", contentItemData));
	contentObject->setType(getStringFromJson("type", contentItemData));
	contentObject->setUri(getStringFromJson("uri", contentItemData));
	contentObject->setEntitled(getBoolFromJson("entitled", contentItemData));
	
	if(contentItemData.HasMember("tags"))
	{
		contentObject->setTags(getStringArrayFromJson(contentItemData["tags"]));
	}
	
	if(contentItemData.HasMember("images"))
	{
		contentObject->setImages(getStringMapFromJson(contentItemData["images"]));
	}
    
    if(contentItemData.HasMember("items"))
    {
        contentObject->setItems(getStringArrayFromJson(contentItemData["items"]));
    }
	
	addContentItemToPool(contentObject);
}

void ContentItemManager::parseContentItemURI(const std::string &contentId, const std::string &uri)
{
	if(uri == "")
	{
		return;
	}
	if(contentExistsForKey(contentId))
	{
		_contentItems.at(contentId)->setUri(uri);
	}
}

void ContentItemManager::parseEntitlementData(const std::string &entitlementData)
{
	rapidjson::Document data;
	data.Parse(entitlementData.c_str());
	
	if (data.HasParseError())
	{
		return; //JSON HAS ERRORS IN IT
	}
	
	for (auto M = data.MemberBegin(); M != data.MemberEnd(); M++)
	{
		if(contentExistsForKey(M->name.GetString()))
		{
			_contentItems.at(M->name.GetString())->setEntitled(M->value.GetBool());
		}
	}
}

NS_TZ_END
