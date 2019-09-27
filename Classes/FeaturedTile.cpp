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
    
    _contentImage = RoundedRectSprite::create();
    _contentImage->setTexture("res/contentPlaceholders/Games1X1.png");
    _contentImage->setCornerRadius(20);
    _contentImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addChild(_contentImage);
    
    _lockedOverlay = RoundedRectSprite::create();
    _lockedOverlay->setTexture("res/decoration/white_1px.png");
    _lockedOverlay->setCornerRadius(20);
    _lockedOverlay->setColor(Style::Color::darkIndigo);
    _lockedOverlay->setOpacity(204);
    _lockedOverlay->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _lockedOverlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lockedOverlay->setIgnoreAnchorPointForPosition(false);
    addChild(_lockedOverlay);
    
    _padlock = ui::ImageView::create("res/hqscene/oomee_padlock.png");
    _padlock->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _padlock->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
    _padlock->ignoreContentAdaptWithSize(false);
    _lockedOverlay->addChild(_padlock);
    
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
    
    const Size& contentSize = getContentSize();
    const Size& innerSize = contentSize - Size(12,12);
    _lockedOverlay->setContentSize(innerSize);
    _padlock->setContentSize(Size(innerSize.height * (0.5f / _imageShape.y), innerSize.height * (0.5f / _imageShape.y)));
    float scale = 1.0f;
    const Size& imageTexPixSize = _contentImage->getTexture()->getContentSizeInPixels();
    switch(_scaleMode)
    {
        case ImageScaleMode::FIT_WIDTH:
        {
            scale = innerSize.width / imageTexPixSize.width;
            break;
        }
        case ImageScaleMode::FIT_HEIGHT:
        {
            scale = innerSize.height / imageTexPixSize.height;
            break;
        }
        case ImageScaleMode::SHOW_ALL:
        {
            scale = MIN(innerSize.height / imageTexPixSize.height, innerSize.width / imageTexPixSize.width);
            break;
        }
        case ImageScaleMode::FILL_ALL:
        {
            scale = MAX(innerSize.height / imageTexPixSize.height, innerSize.width / imageTexPixSize.width);
            break;
        }
    }
    //_contentImage->setContentSize(imageTexPixSize * scale);
    _contentImage->setContentSize(innerSize);
}

void FeaturedTile::setContentItemData(const HQContentItemObjectRef& contentItem)
{
    _contentItem = contentItem;
    if(_contentItem)
    {
        _lockedOverlay->setVisible(!_contentItem->isEntitled());
    }
}

void FeaturedTile::setImageShape(const Vec2& imageShape)
{
    _imageShape = imageShape;
}

void FeaturedTile::elementDisappeared(cocos2d::Node *sender)
{
    _contentImage->setTexture("res/contentPlaceholders/Games1X1.png");
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
    _contentImage->setTexture(downloader->getLocalImagePath());
    onSizeChanged();
}
void FeaturedTile::onImageDownloadFailed()
{
    elementOnScreen = false;
}

NS_AZOOMEE_END
