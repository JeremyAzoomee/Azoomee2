#include "StickerCache.h"
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <dirent.h>

using namespace cocos2d;


USING_NS_TZ

NS_AZ_CHAT_BEGIN

const char* const kStickersLocalDirectory = "res/chat/stickers/";
const char* const kStickersCataloguePath = "res/chat/stickers/catalogue.json";

static std::auto_ptr<StickerCache> sStickerCacheSharedInstance;

StickerCache* StickerCache::getInstance()
{
    if(!sStickerCacheSharedInstance.get())
    {
        sStickerCacheSharedInstance.reset(new StickerCache());
        sStickerCacheSharedInstance->init();
    }
    return sStickerCacheSharedInstance.get();
}

StickerCache::StickerCache()
{
}

void StickerCache::init()
{
    // Load the Sticker catalogue and parse the json
    const std::string& fileData = FileUtils::getInstance()->getStringFromFile(kStickersCataloguePath);
    rapidjson::Document json;
    json.Parse(fileData.c_str());
    
    const rapidjson::Value& categories = json["categories"];
    for(auto it = categories.Begin(); it != categories.End(); ++it)
    {
        const auto& jsonEntry = *it;
        const StickerCategoryRef& category = StickerCategory::createFromJson(jsonEntry);
        _categories.push_back(category);
    }
    
    // Create the sticker indexes
    for(const auto& category : _categories)
    {
        for(const auto& sticker : category->stickers())
        {
            _stickersByURL[sticker->imageURL()] = sticker;
        }
    }
}

#pragma mark - Public

std::string StickerCache::localBundlePath() const
{
    return kStickersLocalDirectory;
}

StickerCategoryList StickerCache::categories() const
{
    return _categories;
}

StickerRef StickerCache::findStickerByURL(const std::string& url) const
{
    const auto& it = _stickersByURL.find(url);
    if(it != _stickersByURL.end())
    {
        return it->second;
    }
    return StickerRef();
}

NS_AZ_CHAT_END
