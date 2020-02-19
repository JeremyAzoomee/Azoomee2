//
//  ContentTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 02/09/2019.
//

#include "ContentTile.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

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
            if(_callback && _contentItem)
            {
                _callback(_contentItem);
            }
        }
    });
    
    _imageDownloader = ImageDownloader::create(ImageDownloader::kImageCachePath, ImageDownloader::CacheMode::File);
    
    return true;
}

void ContentTile::onExit()
{
    _imageDownloader->setDelegate(nullptr);
    Super::onExit();
}

void ContentTile::setContentSelectedCallback(const ContentSelectedCallback& callback)
{
    _callback = callback;
}

void ContentTile::setContentItemData(const HQContentItemObjectRef& contentItem)
{
    _contentItem = contentItem;
}

HQContentItemObjectRef ContentTile::getContentItemData() const
{
    return _contentItem;
}

void ContentTile::setPlaceholderFilename(const std::string &placeholder)
{
    _placholderFilename = placeholder;
}

void ContentTile::setImageScaleMode(const ImageScaleMode &scaleMode)
{
    _scaleMode = scaleMode;
}

// delegate functions
void ContentTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    
}

void ContentTile::onImageDownloadFailed()
{
    
}

NS_AZ_END
