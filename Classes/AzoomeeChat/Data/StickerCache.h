#ifndef AzoomeeChat_StickerCache_h
#define AzoomeeChat_StickerCache_h

#include "../AzoomeeChat.h"
#include "StickerCategory.h"
#include "Sticker.h"
#include <map>


NS_AZOOMEE_CHAT_BEGIN

/**
 * Manages the cache of Stickers used in the chat app.
 */
class StickerCache
{
private:
    
    /// List of sticker categories
    StickerCategoryList _categories;
    /// Index sticker URL to Sticker
    std::map<std::string, StickerRef> _stickersByURL;
    
    // no direct construction
    StickerCache();
    /// Initialise and load available stickers
    void init();
    
public:
    
    /// Singleton instance
    static StickerCache* getInstance();
    
    /// Return the path to the local bundled stickers
    std::string localBundlePath() const;
    
    /// Sticker categories
    StickerCategoryList categories() const;
    
    /// Find a Sticker by URL, if it is recognised
    StickerRef findStickerByURL(const std::string& url) const;
};

NS_AZOOMEE_CHAT_END

#endif
