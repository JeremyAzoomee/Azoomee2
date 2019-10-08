//
//  FeaturedGamesHolder.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 03/09/2019.
//

#include "FeaturedGamesHolder.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/UIConsts.h>
#include "HQDataProvider.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float FeaturedGamesHolder::kTileSpacing = 25.0f;

bool FeaturedGamesHolder::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _mainTile = FeaturedTile::create();
    _mainTile->setImageScaleMode(ContentTile::ImageScaleMode::FIT_WIDTH);
    _mainTile->setImageShape(TILESIZE_2X2);
    _mainTile->setPlaceholderFilename(CONTENT_PLACEHOLDER_GAME_1X1);
    _mainTile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _mainTile->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _mainTile->setSizeType(SizeType::PERCENT);
    _mainTile->setSizePercent(Vec2(1.0f, 0.66f));
    _mainTile->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content, 0);
        }
    });
    _contentLayout->addChild(_mainTile);
    
    _subTile1 = FeaturedTile::create();
    _subTile1->setImageScaleMode(ContentTile::ImageScaleMode::FIT_WIDTH);
    _subTile1->setPlaceholderFilename(CONTENT_PLACEHOLDER_GAME_1X1);
    _subTile1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _subTile1->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    _subTile1->setSizeType(SizeType::PERCENT);
    _subTile1->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content, 1);
        }
    });
    _contentLayout->addChild(_subTile1);
    
    _subTile2 = FeaturedTile::create();
    _subTile2->setImageScaleMode(ContentTile::ImageScaleMode::FIT_WIDTH);
    _subTile2->setPlaceholderFilename(CONTENT_PLACEHOLDER_GAME_1X1);
    _subTile2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _subTile2->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
    _subTile2->setSizeType(SizeType::PERCENT);
    _subTile2->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content, 2);
        }
    });
    _contentLayout->addChild(_subTile2);
    
    return true;
}

void FeaturedGamesHolder::onEnter()
{
    if(_contentData)
    {
        auto items = _contentData->getContentItems();
        if(items.size() > 1)
        {
            _mainTile->setContentItemData(items.at(0));
        }
        if(items.size() > 2)
        {
            _subTile1->setContentItemData(items.at(1));
        }
        if(items.size() > 3)
        {
            _subTile2->setContentItemData(items.at(2));
        }
    }
    
    Super::onEnter();
}

void FeaturedGamesHolder::onExit()
{
    Super::onExit();
}

void FeaturedGamesHolder::onSizeChanged()
{
    Super::onSizeChanged();
    
    Size contentSize =  _useFixedHeight ? getContentSize() : Size(getContentSize().width, 0);
    if(_useFixedHeight)
    {
        if(_useWideLayout)
        {
            if(((contentSize.width * 0.66f) / contentSize.height) < AspectRatio4x3)
            {
                contentSize.width = (4.0f * contentSize.height) / (3.0f * 0.66f);
            }
        }
        else
        {
            if((contentSize.width / (contentSize.height * 0.66f)) > AspectRatio16x9) // if main tile size exceeds 16:9 aspect ratio
            {
                contentSize.width = AspectRatio16x9 * (contentSize.height * 0.66f); // restrict width so tile aspect ratio no greater than 16:9
            }
        }
    }
    else
    {
        if(_useWideLayout)
        {
            contentSize.height = (contentSize.width * 0.66f) * AspectRatio3x4;
        }
        else
        {
            contentSize.height = contentSize.width * AspectRatio9x16 * 1.5f;
        }
        if(getSizeType() != SizeType::PERCENT && !getContentSize().equals(contentSize))
        {
            setContentSize(contentSize);
        }
    }
    
    
    _contentLayout->setContentSize(contentSize);
    
    const Size& paddingPercent = Size(kTileSpacing / contentSize.width, kTileSpacing / contentSize.height);
    
    if(_useWideLayout) // Main tile on left, sub tiles stacked vertically on the right
    {
        _mainTile->setSizePercent(Vec2(0.66f, 1.0f)); // reset percentage size again so tile doesnt vanish
        _mainTile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _mainTile->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
        _subTile1->setSizePercent(Vec2(0.34f - paddingPercent.width, 0.5f - (paddingPercent.height / 2.0f)));
        _subTile1->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        _subTile1->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
        _subTile2->setSizePercent(Vec2(0.34f - paddingPercent.width, 0.5f - (paddingPercent.height / 2.0f)));
    }
    else // main tile at top, sub tiles bellow, aligned horizontally
    {
        _mainTile->setSizePercent(Vec2(1.0f, 0.66f)); // reset percentage size again so tile doesnt vanish
        _mainTile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _mainTile->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
        _subTile1->setSizePercent(Vec2(0.5f - (paddingPercent.width / 2), 0.34f - paddingPercent.height));
        _subTile1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _subTile1->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
        _subTile2->setSizePercent(Vec2(0.5f - (paddingPercent.width / 2), 0.34f - paddingPercent.height));
    }
}

void FeaturedGamesHolder::enableWideLayout(bool enable)
{
    _useWideLayout = enable;
}

void FeaturedGamesHolder::enableFixedHeight(bool enable)
{
    _useFixedHeight = enable;
}

NS_AZOOMEE_END
