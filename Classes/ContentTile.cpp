//
//  ContentTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 02/09/2019.
//

#include "ContentTile.h"

NS_AZOOMEE_BEGIN

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

// delegate functions
void ContentTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    
}

void ContentTile::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
