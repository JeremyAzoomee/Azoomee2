//
//  FeaturedTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 03/09/2019.
//

#include "FeaturedTile.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloaderCacheCleanerLogic.h>
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
    Super::onEnter();
    startCheckingForOnScreenPosition(this);
}

void FeaturedTile::onExit()
{
    if(!checkIfElementIsOnScreen(this))
    {
        elementOnScreen = false;
        elementDisappeared(this);
    }
    stopCheckingOnScreenPosition();
    Super::onExit();
}

void FeaturedTile::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    const Size& innerSize = contentSize - Size(12,12);
    _lockedOverlay->setContentSize(innerSize);
    _padlock->setContentSize(Size(innerSize.height * (0.5f / _imageShape.y), innerSize.height * (0.5f / _imageShape.y)));
    resizeContentImage();
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

void FeaturedTile::setPlaceholderFilename(const std::string& placeholder)
{
    _placholderFilename = placeholder;
    _contentImage->setTexture(_placholderFilename);
}

void FeaturedTile::resizeContentImage()
{
    const Size& innerSize = getContentSize() - Size(12,12);
    _contentImage->setContentSize(innerSize);
}

void FeaturedTile::elementDisappeared(cocos2d::Node *sender)
{
    if(_contentImage->getTexture()->getPath() != _placholderFilename)
    {
        _contentImage->setTexture(_placholderFilename);
        ImageDownloaderCacheCleanerLogic::getInstance()->imageRemoved();
        resizeContentImage();
    }
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
        if(_contentImage->getTexture()->getPath() != _placholderFilename)
        {
            _contentImage->setTexture(_placholderFilename);
            resizeContentImage();
        }
    }
}

// delegate functions
void FeaturedTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _contentImage->setTexture(downloader->getLocalImagePath());
    resizeContentImage();
}
void FeaturedTile::onImageDownloadFailed()
{
    if(_imageShape != TILESIZE_1X1)
    {
        _imageShape = TILESIZE_1X1;
        _imageDownloader->downloadImage(this, HQDataProvider::getInstance()->getThumbnailUrlForItem(_contentItem, TILESIZE_1X1));
    }
}

NS_AZOOMEE_END
