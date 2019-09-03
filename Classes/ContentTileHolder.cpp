//
//  ContentTileHolder.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 02/09/2019.
//

#include "ContentTileHolder.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ContentTileHolder::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f, 1.0f));
    
    _contentLayout = ui::Layout::create();
    _contentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentLayout->setPositionType(PositionType::PERCENT);
    _contentLayout->setPositionPercent(Vec2::ANCHOR_MIDDLE);
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

NS_AZOOMEE_END
