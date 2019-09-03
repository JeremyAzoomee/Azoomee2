//
//  ContentTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 02/09/2019.
//

#include "ContentTile.h"
#include "HQDataProvider.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ContentTile::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setTouchEnabled(true);
    addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_callback)
            {
                _callback(_contentItem);
            }
        }
    });
    
    _imageDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    
    return true;
}

void ContentTile::setContentSelectedCallback(const ContentSelectedCallback& callback)
{
    _callback = callback;
}

void ContentTile::setContentItemData(const HQContentItemObjectRef& contentItem)
{
    _contentItem = contentItem;
    if(_contentItem)
    {
        _imageDownloader->downloadImage(this, HQDataProvider::getInstance()->getThumbnailUrlForItem(_contentItem, Vec2(1,1)));
    }
}

HQContentItemObjectRef ContentTile::getContentItemData() const
{
    return _contentItem;
}

void ContentTile::setPlaceholderFilename(const std::string &placeholder)
{
    _placholderFilename = placeholder;
}

// delegate functions
void ContentTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    
}

void ContentTile::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
