#ifndef AzoomeeChat_Sticker_h
#define AzoomeeChat_Sticker_h

#include "../AzoomeeChat.h"
#include <AzoomeeCommon/Data/Json.h>
#include <string>
#include <memory>


NS_AZOOMEE_CHAT_BEGIN

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
    
    const std::string& imageURL() const;
    const std::string& imageLocalPath() const;
};

NS_AZOOMEE_CHAT_END

#endif
