#include "ContentItemManager.h"
#include "../Child/ChildManager.h"
#include "../../Utils/StringFunctions.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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

NS_AZOOMEE_END
