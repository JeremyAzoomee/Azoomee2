#include "ContentItemPool.h"
#include "../Child/ChildDataProvider.h"
#include "../../Utils/StringFunctions.h"

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

bool ContentItemPool::contentExistsForKey(const std::string &contentId) const
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

std::vector<HQContentItemObjectRef> ContentItemPool::getContentItems() const
{
    std::vector<HQContentItemObjectRef> returnArray;
    
    for(auto item : _contentItems)
    {
        returnArray.push_back(item.second);
    }
    
    return returnArray;
}

HQContentItemObjectRef ContentItemPool::getContentItemForId(const std::string &contentId) const
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

bool ContentItemPool::isSameContentPool(const std::string &etag)
{
    return _currentPoolEtag == etag;
}

void ContentItemPool::setPoolEtag(const std::string &etag)
{
    _currentPoolEtag = etag;
}

void ContentItemPool::backupContentItemPool()
{
    const std::string &contentPath = FileUtils::getInstance()->getWritablePath() + "contentCache/" + ChildDataProvider::getInstance()->getParentOrChildId() + "/";
    
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

void ContentItemPool::restoreContentItemPool()
{
    emptyContentItemPool();
    
    const std::string &contentFilePath = FileUtils::getInstance()->getWritablePath() + "contentCache/" + ChildDataProvider::getInstance()->getParentOrChildId() + "/contentCache.bak";
    if(!FileUtils::getInstance()->isFileExist(contentFilePath))
    {
        return;
    }
    
    const std::string &fileData = FileUtils::getInstance()->getStringFromFile(contentFilePath);
    const std::vector<std::string> &jsonStringVector = splitStringToVector(fileData, "|");
    
    for(const std::string& currentItemJsonString : jsonStringVector)
    {
        const std::map<std::string, std::string> &elementMap = getStringMapFromJsonString(currentItemJsonString);
        HQContentItemObjectRef newContentItem = HQContentItemObject::createFromMap(elementMap);
        addContentItemToPool(newContentItem);
    }
}

NS_AZOOMEE_END
