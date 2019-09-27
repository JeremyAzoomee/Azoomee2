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
    
    const Size& contentSize = getContentSize();
    
    _dropShadow->setContentSize(contentSize + kDropshadowPadding);
    _contentClipper->setContentSize(contentSize + kDropshadowPadding);
    _clippingStencil->setContentSize(contentSize + kDropshadowPadding);
    _lockedOverlay->setContentSize(contentSize);
    _padlock->setContentSize(Size(contentSize.height * 0.5f, contentSize.height * 0.5f));
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

void RoundedRectTile::setContentItemData(const HQContentItemObjectRef& contentItem)
{
    _contentItem = contentItem;
    if(_contentItem)
    {
        _lockedOverlay->setVisible(!_contentItem->isEntitled());
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
    
}

NS_AZOOMEE_END
