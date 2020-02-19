//
//  FeaturedVideosHolder.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 04/09/2019.
//

#include "FeaturedVideosHolder.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/UIConsts.h>
#include "HQConstants.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float FeaturedVideosHolder::kTileSpacing = 16.0f;
const int FeaturedVideosHolder::kColumns = 3;

bool FeaturedVideosHolder::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _tile1 = FeaturedTile::create();
    _tile1->setImageScaleMode(ContentTile::ImageScaleMode::FILL_ALL);
    _tile1->setImageShape(TILESIZE_1X2);
    _tile1->setPlaceholderFilename(CONTENT_PLACEHOLDER_VIDEO_1X2);
    _tile1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _tile1->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _tile1->setSizeType(SizeType::PERCENT);
    _tile1->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content, 0);
        }
    });
    _contentLayout->addChild(_tile1);
    
    _tile2 = FeaturedTile::create();
    _tile2->setImageScaleMode(ContentTile::ImageScaleMode::FILL_ALL);
    _tile2->setImageShape(TILESIZE_1X2);
    _tile2->setPlaceholderFilename(CONTENT_PLACEHOLDER_VIDEO_1X2);
    _tile2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _tile2->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _tile2->setSizeType(SizeType::PERCENT);
    _tile2->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content, 1);
        }
    });
    _contentLayout->addChild(_tile2);
    
    _tile3 = FeaturedTile::create();
    _tile3->setImageScaleMode(ContentTile::ImageScaleMode::FILL_ALL);
    _tile3->setImageShape(TILESIZE_1X2);
    _tile3->setPlaceholderFilename(CONTENT_PLACEHOLDER_VIDEO_1X2);
    _tile3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _tile3->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    _tile3->setSizeType(SizeType::PERCENT);
    _tile3->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content, 2);
        }
    });
    _contentLayout->addChild(_tile3);
    
    return true;
}

void FeaturedVideosHolder::onEnter()
{
    if(_contentData)
    {
        auto items = _contentData->getContentItems();
        if(items.size() > 0)
        {
            _tile1->setContentItemData(items.at(0));
        }
        if(items.size() > 1)
        {
            _tile2->setContentItemData(items.at(1));
        }
        if(items.size() > 2)
        {
            _tile3->setContentItemData(items.at(2));
        }
    }
    
    Super::onEnter();
}

void FeaturedVideosHolder::onExit()
{
    Super::onExit();
}

void FeaturedVideosHolder::onSizeChanged()
{
    Super::onSizeChanged();
    
    Size contentSize = getContentSize();// - Size(kTileSpacing, 0);
    
    Size paddingPercent = Size(kTileSpacing / contentSize.width, kTileSpacing / contentSize.height);
    float tileWidth = (1.0f - (2.0f * paddingPercent.width)) / kColumns;
    
    if(_useFixedHeight)
    {
        if((tileWidth * contentSize.width) / contentSize.height > AspectRatio2x3) // cap size of tiles to max 2:3 aspect ratio
        {
            contentSize = Size(2 * (kTileSpacing + contentSize.height), contentSize.height); //
            paddingPercent = Size(kTileSpacing / contentSize.width, kTileSpacing / contentSize.height);
            tileWidth = (1.0f - (2.0f * paddingPercent.width)) / kColumns;
        }
    }
    else
    {
        contentSize.height = contentSize.width * (tileWidth * 1.5f);
        paddingPercent.height = kTileSpacing / contentSize.height;
        if(getSizeType() != SizeType::PERCENT && !getContentSize().equals(contentSize))
        {
            setContentSize(contentSize);
        }
    }
    
    _tile1->setSizePercent(Vec2(tileWidth, 1.0f));
    _tile2->setSizePercent(Vec2(tileWidth, 1.0f));
    _tile3->setSizePercent(Vec2(tileWidth, 1.0f));
    
    _contentLayout->setContentSize(contentSize);
}

void FeaturedVideosHolder::enableFixedHeight(bool enable)
{
    _useFixedHeight = enable;
}

NS_AZOOMEE_END
