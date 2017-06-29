#include "StickerCategory.h"
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "StickerCache.h"

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

StickerCategoryRef StickerCategory::createFromJson(const rapidjson::Value& json)
{
    // Parse the JSON
    if(!json.IsObject())
    {
        // Invalid JSON
        return StickerCategoryRef();
    }
    
    // Example response:
    //{
    //    "image_location": "<url>",
    //    "id": "food",
    //    "stickers": [
    //        "ball/s_012.png",
    //        "ball/s_013.png",
    //        "ball/s_014.png",
    //        "ball/s_015.png",
    //        "ball/s_016.png"
    //    ]
    //}
    
    const std::string& imageURL = json["image_location"].GetString();
    const std::string& categoryID = json["id"].GetString();
    
    StickerList stickers;
    const std::string& stickerLocalRoot = StickerCache::getInstance()->localBundlePath();
    const rapidjson::Value& stickersJson = json["stickers"];
    for(auto it = stickersJson.Begin(); it != stickersJson.End(); ++it)
    {
        const auto& jsonEntry = *it;
        const std::string& stickerPath = jsonEntry.GetString();
        const StickerRef& sticker = Sticker::create("azoomee://" + stickerPath, stickerLocalRoot + stickerPath);
        stickers.push_back(sticker);
    }
    
    StickerCategoryRef category(new StickerCategory());
    category->_imageURL = stickerLocalRoot + imageURL;
    category->_categoryID = categoryID;
    category->_stickers = stickers;
    return category;
}

StickerCategory::StickerCategory()
{
}

const std::string& StickerCategory::imageURL() const
{
    return _imageURL;
}

const std::string& StickerCategory::categoryID() const
{
    return _categoryID;
}

const std::string& StickerCategory::imageLocalPath() const
{
    // For now, the imageURL is just the local path
    return _imageURL;
}

const StickerList& StickerCategory::stickers() const
{
    return _stickers;
}

NS_AZOOMEE_CHAT_END
