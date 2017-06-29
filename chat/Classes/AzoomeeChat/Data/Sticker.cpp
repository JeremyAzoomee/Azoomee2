#include "Sticker.h"
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "StickerCache.h"

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

StickerRef Sticker::create(const std::string& url, const std::string& localPath)
{
    StickerRef sticker(new Sticker());
    sticker->_imageURL = url;
    sticker->_imageLocalPath = localPath;
    return sticker;
}

Sticker::Sticker()
{
}

const std::string& Sticker::imageURL() const
{
    return _imageURL;
}

const std::string& Sticker::imageLocalPath() const
{
    return _imageLocalPath;
}

NS_AZOOMEE_CHAT_END
