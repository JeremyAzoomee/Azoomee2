#ifndef AzoomeeChat_StickerCategory_h
#define AzoomeeChat_StickerCategory_h

#include "../AzoomeeChat.h"
#include <TinizineCommon/Data/Json.h>
#include <string>
#include <memory>
#include "Sticker.h"


USING_NS_TZ

NS_AZ_CHAT_BEGIN

// forward decleration
class StickerCategory;
typedef std::shared_ptr<StickerCategory> StickerCategoryRef;
typedef std::vector<StickerCategoryRef> StickerCategoryList;

/**
 * A StickerCategory is a contact in the chat list.
 */
class StickerCategory
{
private:
    
    std::string _imageURL;
    std::string _categoryID;
    StickerList _stickers;
    
    // no direct construction
    StickerCategory();
    
public:
    
    /// Create a StickerCategory from JSON
    static StickerCategoryRef createFromJson(const rapidjson::Value& json);
    
    std::string imageURL() const;
    std::string categoryID() const;
    std::string imageLocalPath() const;
    StickerList stickers() const;
};

NS_AZ_CHAT_END

#endif
