//
//  ContentTileHolder.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 02/09/2019.
//

#include "ContentTileHolder.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool ContentTileHolder::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f, 1.0f));
    
    _contentLayout = ui::Layout::create();
    _contentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _contentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    addChild(_contentLayout);
    
    return true;
}

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

NS_AZ_END
