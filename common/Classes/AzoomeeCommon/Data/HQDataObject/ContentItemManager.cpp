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
    if(contentExistsForKey(contentId))
    {
        return _contentItems.at(contentId);
    }
    else
    {
        return nullptr;
    }
}

bool ContentItemManager::isSameContentPool(const std::string &etag) const
{
    return _currentPoolEtag == etag;
}

void ContentItemManager::setPoolEtag(const std::string &etag)
{
    _currentPoolEtag = etag;
}

void ContentItemManager::backupContentItemPool()
{
    const std::string &contentPath = FileUtils::getInstance()->getWritablePath() + "contentCache/" + ChildManager::getInstance()->getParentOrChildId() + "/";
    
    if(!FileUtils::getInstance()->isDirectoryExist(contentPath))
    {
        FileUtils::getInstance()->createDirectory(contentPath);
    }
    
    std::string saveString = "";
    
    for(auto item : _contentItems)
    {
        if(saveString.length() > 0)
        {
            saveString += "|";
        }
        
        saveString += item.second->getJSONRepresentationOfStructure();
    }
    
    FileUtils::getInstance()->writeStringToFile(saveString, contentPath + "contentCache.bak");
}

void ContentItemManager::restoreContentItemPool()
{
    emptyContentItemPool();
    
    const std::string &contentFilePath = FileUtils::getInstance()->getWritablePath() + "contentCache/" + ChildManager::getInstance()->getParentOrChildId() + "/contentCache.bak";
    if(!FileUtils::getInstance()->isFileExist(contentFilePath))
    {
        return;
    }
    
    const std::string &fileData = FileUtils::getInstance()->getStringFromFile(contentFilePath);
    const std::vector<std::string> &jsonStringVector = splitStringToVector(fileData, "|");
    
    for(const std::string& currentItemJsonString : jsonStringVector)
    {
        const std::map<std::string, std::string> &elementMap = getStringMapFromJsonString(currentItemJsonString);
        MutableHQContentItemObjectRef newContentItem = MutableHQContentItemObject::createFromMap(elementMap);
        addContentItemToPool(newContentItem);
    }
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
