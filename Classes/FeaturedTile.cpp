//
//  FeaturedTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 03/09/2019.
//

#include "FeaturedTile.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool FeaturedTile::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundImage("res/hqscene/rounded_rect_20px.png");
    setBackGroundImageColor(Style::Color::macaroniAndCheese);
    setBackGroundImageScale9Enabled(true);
    
    _clippingStencil = ui::Scale9Sprite::create("res/hqscene/rounded_rect_20px.png");
    _clippingStencil->setContentSize(getContentSize() - Size(12,12));
    _clippingStencil->setPosition(Vec2(6,6));
    _clippingStencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    _contentClipper = ClippingNode::create(_clippingStencil);
    _contentClipper->setAlphaThreshold(0.5f);
    _contentClipper->setPosition(Vec2(0,0));
    _contentClipper->setContentSize(getContentSize());
    addChild(_contentClipper);
    
    _contentImage = ui::ImageView::create();
    _contentImage->ignoreContentAdaptWithSize(false);
    _contentImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->setPositionType(PositionType::PERCENT);
    _contentImage->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    _contentClipper->addChild(_contentImage);
    
    return true;
}

void FeaturedTile::onEnter()
{
    Super::onEnter();
}

void FeaturedTile::onExit()
{
    Super::onExit();
}

void FeaturedTile::onSizeChanged()
{
    Super::onSizeChanged();
    _contentClipper->setContentSize(getContentSize());
    _clippingStencil->setContentSize(getContentSize() - Size(12,12));
    if(_scaleMode == ImageScaleMode::FIT_WIDTH)
    {
        _contentImage->setScale(getContentSize().width / _contentImage->getContentSize().width);
    }
    else if(_scaleMode == ImageScaleMode::FIT_HEIGHT)
    {
        _contentImage->setScale(getContentSize().height / _contentImage->getContentSize().height);
    }
}

void FeaturedTile::setImageScaleMode(const ImageScaleMode& scaleMode)
{
    _scaleMode = scaleMode;
}

// delegate functions
void FeaturedTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _contentImage->loadTexture(downloader->getLocalImagePath());
}
void FeaturedTile::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
