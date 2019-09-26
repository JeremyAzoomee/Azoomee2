//
//  FeaturedTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 03/09/2019.
//

#include "FeaturedTile.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "HQDataProvider.h"

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
    
    _contentImage = ui::ImageView::create("res/contentPlaceholders/Games1X1.png");
    _contentImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _contentClipper->addChild(_contentImage);
    
    return true;
}

void FeaturedTile::onEnter()
{
    startCheckingForOnScreenPosition(this);
    Super::onEnter();
}

void FeaturedTile::onExit()
{
    stopCheckingOnScreenPosition();
    _imageDownloader->setDelegate(nullptr);
    Super::onExit();
}

void FeaturedTile::onSizeChanged()
{
    Super::onSizeChanged();
    _contentClipper->setContentSize(getContentSize());
    _clippingStencil->setContentSize(getContentSize() - Size(12,12));
    switch(_scaleMode)
    {
        case ImageScaleMode::FIT_WIDTH:
        {
            _contentImage->setScale(getContentSize().width / _contentImage->getContentSize().width);
            break;
        }
        case ImageScaleMode::FIT_HEIGHT:
        {
            _contentImage->setScale(getContentSize().height / _contentImage->getContentSize().height);
            break;
        }
        case ImageScaleMode::SHOW_ALL:
        {
            _contentImage->setScale(MIN(getContentSize().height / _contentImage->getContentSize().height, getContentSize().width / _contentImage->getContentSize().width));
            break;
        }
        case ImageScaleMode::FILL_ALL:
        {
            _contentImage->setScale(MAX(getContentSize().height / _contentImage->getContentSize().height, getContentSize().width / _contentImage->getContentSize().width));
            break;
        }
    }
}

void FeaturedTile::setImageShape(const Vec2& imageShape)
{
    _imageShape = imageShape;
}

void FeaturedTile::elementDisappeared(cocos2d::Node *sender)
{
    _contentImage->loadTexture("res/contentPlaceholders/Games1X1.png");
    onSizeChanged();
}

void FeaturedTile::elementAppeared(cocos2d::Node *sender)
{
    if(_contentItem)
    {
        _imageDownloader->downloadImage(this, HQDataProvider::getInstance()->getThumbnailUrlForItem(_contentItem, _imageShape));
    }
    else
    {
        elementOnScreen = false;
    }
}

// delegate functions
void FeaturedTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _contentImage->loadTexture(downloader->getLocalImagePath());
    onSizeChanged();
}
void FeaturedTile::onImageDownloadFailed()
{
    elementOnScreen = false;
}

NS_AZOOMEE_END
