#ifndef AzoomeeChat_Sticker_h
#define AzoomeeChat_Sticker_h

#include "../AzoomeeChat.h"
#include <AzoomeeCommon/Data/Json.h>
#include <string>
#include <memory>


USING_NS_TZ

NS_AZ_CHAT_BEGIN

// forward decleration
class Sticker;
typedef std::shared_ptr<Sticker> StickerRef;
typedef std::vector<StickerRef> StickerList;

/**
 * A Sticker is a contact in the chat list.
 */
class Sticker
{
private:
    
    std::string _imageURL;
    std::string _imageLocalPath;
    
    // no direct construction
    Sticker();
    
public:
    
    /// Create a Sticker
    static StickerRef create(const std::string& url, const std::string& localPath);
    
    std::string imageURL() const;
    std::string imageLocalPath() const;
};

NS_AZ_CHAT_END

#endif
