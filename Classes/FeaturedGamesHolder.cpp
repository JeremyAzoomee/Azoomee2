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

const cocos2d::Size FeaturedGamesHolder::kMainTileSize = Size(1280,720);
const cocos2d::Size FeaturedGamesHolder::kSubTileSize = Size(624,351);
const float FeaturedGamesHolder::kTileSpacing = 32.0f;
const cocos2d::Size FeaturedGamesHolder::kTargetSize = kMainTileSize + Size(2 * kTileSpacing, kTileSpacing + kSubTileSize.height);

bool FeaturedGamesHolder::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(BackGroundColorType::SOLID);
    setBackGroundColor(Color3B::YELLOW);
    
    _contentLayout->setContentSize(kTargetSize);
    _contentLayout->setLayoutType(Type::VERTICAL);
    _contentLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _contentLayout->setBackGroundColor(Color3B::GREEN);
    
    _mainTile = FeaturedTile::create();
    _mainTile->setImageScaleMode(ImageScaleMode::FIT_WIDTH);
    _mainTile->setContentSize(kMainTileSize);
    _mainTile->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(kTileSpacing, 0, kTileSpacing, kTileSpacing)));
    _mainTile->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content);
        }
    });
    _contentLayout->addChild(_mainTile);
    
    _subTiles = ui::Layout::create();
    _subTiles->setContentSize(Size(kMainTileSize.width, kSubTileSize.height));
    _subTiles->setLayoutType(Type::HORIZONTAL);
    _subTiles->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(kTileSpacing, 0, kTileSpacing, 0)));
    _contentLayout->addChild(_subTiles);
    
    _subTile1 = FeaturedTile::create();
    _subTile1->setImageScaleMode(ImageScaleMode::FIT_WIDTH);
    _subTile1->setContentSize(kSubTileSize);
    _subTile1->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(0, 0, kTileSpacing / 2.0f, 0)));
    _subTile1->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content);
        }
    });
    _subTiles->addChild(_subTile1);
    
    _subTile2 = FeaturedTile::create();
    _subTile2->setImageScaleMode(ImageScaleMode::FIT_WIDTH);
    _subTile2->setContentSize(kSubTileSize);
    _subTile2->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(kTileSpacing / 2.0f, 0, 0, 0)));
    _subTile2->setContentSelectedCallback([this](HQContentItemObjectRef content){
        if(_callback)
        {
            _callback(content);
        }
    });
    _subTiles->addChild(_subTile2);
    
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
    
    //Size minSize = Size(MIN(getContentSize().width, kTargetSize.width), MIN(getContentSize().height, kTargetSize.height));
    //float scaleFactor = MIN(minSize.width / kTargetSize.width ,minSize.height / kTargetSize.height);
    //_contentLayout->setScale(scaleFactor);
}

NS_AZOOMEE_END
