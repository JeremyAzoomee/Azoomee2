//
//  RoundedRectTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 06/09/2019.
//

#include "RoundedRectTile.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "HQDataProvider.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const Size RoundedRectTile::kDropshadowPadding = Size(80,80);

bool RoundedRectTile::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _dropShadow = ui::ImageView::create("res/hqscene/DropDownBoxStencil.png");
    _dropShadow->setContentSize(getContentSize() + kDropshadowPadding);
    _dropShadow->setScale9Enabled(true);
    _dropShadow->ignoreContentAdaptWithSize(false);
    _dropShadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _dropShadow->setPosition(kDropshadowPadding * -0.5f);
    _dropShadow->setColor(Style::Color::brownGrey);
    _dropShadow->setOpacity(125);
    
    addChild(_dropShadow);
    
    _clippingStencil = ui::Scale9Sprite::create("res/hqscene/DropDownBoxStencil.png");
    _clippingStencil->setContentSize(getContentSize() + kDropshadowPadding);
    _clippingStencil->setPosition(Vec2(0,0));
    _clippingStencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    _contentClipper = ClippingNode::create(_clippingStencil);
    _contentClipper->setAlphaThreshold(0.9f);
    _contentClipper->setPosition(kDropshadowPadding * -0.5f);
    _contentClipper->setContentSize(getContentSize() + kDropshadowPadding);
    addChild(_contentClipper);
    
    _contentImage = ui::ImageView::create("res/contentPlaceholders/Games1X1.png");
    _contentImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _contentClipper->addChild(_contentImage);
    
    return true;
}

void RoundedRectTile::onEnter()
{
    startCheckingForOnScreenPosition(this);
    Super::onEnter();
}

void RoundedRectTile::onExit()
{
    stopCheckingOnScreenPosition();
    _imageDownloader->setDelegate(nullptr);
    Super::onExit();
}

void RoundedRectTile::onSizeChanged()
{
    Super::onSizeChanged();
    _dropShadow->setContentSize(getContentSize() + kDropshadowPadding);
    _contentClipper->setContentSize(getContentSize() + kDropshadowPadding);
    _clippingStencil->setContentSize(getContentSize() + kDropshadowPadding);
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

void RoundedRectTile::setImageShape(const Vec2& imageShape)
{
    _imageShape = imageShape;
}

void RoundedRectTile::elementDisappeared(cocos2d::Node *sender)
{
    _contentImage->loadTexture("res/contentPlaceholders/Games1X1.png");
    onSizeChanged();
}

void RoundedRectTile::elementAppeared(cocos2d::Node *sender)
{
    if(_contentItem && isVisible())
    {
        _imageDownloader->downloadImage(this, HQDataProvider::getInstance()->getThumbnailUrlForItem(_contentItem, _imageShape));
    }
    else
    {
        elementOnScreen = false;
    }
}

// delegate functions
void RoundedRectTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _contentImage->loadTexture(downloader->getLocalImagePath());
    onSizeChanged();
}
void RoundedRectTile::onImageDownloadFailed()
{
    //elementOnScreen = false;
}

NS_AZOOMEE_END
