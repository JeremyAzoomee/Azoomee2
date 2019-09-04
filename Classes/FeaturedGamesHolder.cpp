//
//  FeaturedGamesHolder.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 03/09/2019.
//

#include "FeaturedGamesHolder.h"
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float FeaturedGamesHolder::kTileSpacing = 16.0f;

bool FeaturedGamesHolder::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    const Size& paddingPercent = Size(kTileSpacing / getContentSize().width, kTileSpacing / getContentSize().height);
    
    setBackGroundColorType(BackGroundColorType::SOLID);
    setBackGroundColor(Color3B::YELLOW);
    
    Size contentSize = getContentSize() - Size(kTileSpacing, 0);
    if((contentSize.width / (contentSize.height * 0.66f)) > 16.0f / 9.0f)
    {
        if(!(getContentSize().width * 0.66f) * (3.0f/4.0f) >= contentSize.height)
        {
            contentSize = Size((16.0f/9.0f) * (contentSize.height * 0.66f), contentSize.height);
        }
    }
    
    _contentLayout->setContentSize(contentSize);
    _contentLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _contentLayout->setBackGroundColor(Color3B::GREEN);
    
    _mainTile = FeaturedTile::create();
    _mainTile->setImageScaleMode(ImageScaleMode::FILL_ALL);
    _mainTile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _mainTile->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _mainTile->setSizeType(SizeType::PERCENT);
    _mainTile->setSizePercent(Vec2(1.0f, 0.66f));
    _mainTile->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content);
        }
    });
    _contentLayout->addChild(_mainTile);
    
    _subTile1 = FeaturedTile::create();
    _subTile1->setImageScaleMode(ImageScaleMode::FILL_ALL);
    _subTile1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _subTile1->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    _subTile1->setSizeType(SizeType::PERCENT);
    _subTile1->setSizePercent(Vec2(0.5f - (paddingPercent.width / 2), 0.34f - paddingPercent.height));
    _subTile1->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content);
        }
    });
    _contentLayout->addChild(_subTile1);
    
    _subTile2 = FeaturedTile::create();
    _subTile2->setImageScaleMode(ImageScaleMode::FILL_ALL);
    _subTile2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _subTile2->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
    _subTile2->setSizeType(SizeType::PERCENT);
    _subTile2->setSizePercent(Vec2(0.5f - (paddingPercent.width / 2), 0.34f - paddingPercent.height));
    _subTile2->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content);
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
        if(items.size() > 2)
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
    
    bool useWideLayout = false;
    
    Size contentSize = getContentSize() - Size(kTileSpacing, 0);
    if((contentSize.width / (contentSize.height * 0.66f)) > 16.0f / 9.0f) // if main tile size exceeds 16:9 aspect ratio
    {
        if((getContentSize().width * 0.66f) * (3.0f/4.0f) >= contentSize.height) // if enough space horizonally to fit main tile and sub tile at 4:3 ratio or greater
        {
            useWideLayout = true;
        }
        else
        {
            contentSize = Size((16.0f/9.0f) * (contentSize.height * 0.66f), contentSize.height); // restrict width so tile aspect ratio no greater than 16:9
        }
    }
    
    _contentLayout->setContentSize(contentSize);
    
    const Size& paddingPercent = Size(kTileSpacing / contentSize.width, kTileSpacing / contentSize.height);
    
    if(useWideLayout) // Main tile on left, sub tiles stacked vertically on the right
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

NS_AZOOMEE_END
