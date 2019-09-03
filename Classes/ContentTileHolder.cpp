//
//  ContentTileHolder.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 02/09/2019.
//

#include "ContentTileHolder.h"

NS_AZOOMEE_BEGIN

void ContentTileHolder::setContentSelectedCallback(const ContentSelectedCallback& callback)
{
    _callback = callback;
}

void ContentTileHolder::setContentItemData(const HQCarouselObjectRef& contentData)
{
    _contentData = contentData;
}

HQCarouselObjectRef ContentTileHolder::getContentItemData() const
{
    return _contentData;
}

NS_AZOOMEE_END
