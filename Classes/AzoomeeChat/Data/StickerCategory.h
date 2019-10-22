#ifndef AzoomeeChat_StickerCategory_h
#define AzoomeeChat_StickerCategory_h

#include "../AzoomeeChat.h"
#include <AzoomeeCommon/Data/Json.h>
#include <AzoomeeCommon/Utils/SpecialCalendarEventManager.h>
#include <string>
#include <memory>
#include "Sticker.h"


NS_AZOOMEE_CHAT_BEGIN

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
    CalenderSeasons _season;
    StickerList _stickers;
    
    // no direct construction
    StickerCategory();
    
public:
    
    /// Create a StickerCategory from JSON
    static StickerCategoryRef createFromJson(const rapidjson::Value& json);
    
    std::string imageURL() const;
    std::string categoryID() const;
    std::string imageLocalPath() const;
    CalenderSeasons season() const;
    StickerList stickers() const;
};

NS_AZOOMEE_CHAT_END

#endif
