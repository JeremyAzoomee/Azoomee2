#include "Sticker.h"
#include <cocos/cocos2d.h>
#include <TinizineCommon/Utils/StringFunctions.h>
#include "StickerCache.h"

using namespace cocos2d;


USING_NS_TZ

NS_AZ_CHAT_BEGIN

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

std::string Sticker::imageURL() const
{
    return _imageURL;
}

std::string Sticker::imageLocalPath() const
{
    return _imageLocalPath;
}

NS_AZ_CHAT_END
