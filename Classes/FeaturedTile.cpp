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
    
    const Color3B& overlayColour = Style::Color::darkIndigo;
    _lockedOverlay = LayerColor::create(Color4B(overlayColour.r, overlayColour.g, overlayColour.b, 204));
    _lockedOverlay->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _lockedOverlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lockedOverlay->setIgnoreAnchorPointForPosition(false);
    _contentClipper->addChild(_lockedOverlay);
    
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
    
    _contentClipper->setContentSize(contentSize);
    _clippingStencil->setContentSize(contentSize - Size(12,12));
    _lockedOverlay->setContentSize(contentSize);
    _padlock->setContentSize(Size(contentSize.height * (0.5f / _imageShape.y), contentSize.height * (0.5f / _imageShape.y)));
    switch(_scaleMode)
    {
        case ImageScaleMode::FIT_WIDTH:
        {
            _contentImage->setScale(contentSize.width / _contentImage->getContentSize().width);
            break;
        }
        case ImageScaleMode::FIT_HEIGHT:
        {
            _contentImage->setScale(contentSize.height / _contentImage->getContentSize().height);
            break;
        }
        case ImageScaleMode::SHOW_ALL:
        {
            _contentImage->setScale(MIN(contentSize.height / _contentImage->getContentSize().height, contentSize.width / _contentImage->getContentSize().width));
            break;
        }
        case ImageScaleMode::FILL_ALL:
        {
            _contentImage->setScale(MAX(contentSize.height / _contentImage->getContentSize().height, contentSize.width / _contentImage->getContentSize().width));
            break;
        }
    }
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
