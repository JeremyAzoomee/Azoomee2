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

const Size RoundedRectTile::kDropshadowPadding = Size(76,76);

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
    _dropShadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _dropShadow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addChild(_dropShadow);
    
    _contentImage = RoundedRectSprite::create();
    _contentImage->setCornerRadius(60);
    _contentImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addChild(_contentImage);
    
    _lockedOverlay = RoundedRectSprite::create();
    _lockedOverlay->setTexture("res/decoration/white_1px.png");
    _lockedOverlay->setCornerRadius(60);
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
    _lockedOverlay->setContentSize(contentSize);
    _padlock->setContentSize(Size(contentSize.height * 0.5f, contentSize.height * 0.5f));
    resizeContentImage();
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

void RoundedRectTile::setPlaceholderFilename(const std::string& placeholder)
{
    _placholderFilename = placeholder;
    _contentImage->setTexture(_placholderFilename);
}

void RoundedRectTile::resizeContentImage()
{
    const Size& contentSize = getContentSize();
    
    const Size& imageTexPixSize = _contentImage->getTexture()->getContentSizeInPixels();
    Rect texRect = Rect(Vec2(0,0), imageTexPixSize);
    switch(_scaleMode)
    {
        case ImageScaleMode::FIT_WIDTH:
        {
            const Size& croppedSize = Size(imageTexPixSize.width, (imageTexPixSize.width * contentSize.height) / contentSize.width);
            const Vec2& origin = (imageTexPixSize / 2.0f) - (croppedSize / 2.0f);
            texRect = Rect(origin, croppedSize);
            break;
        }
        case ImageScaleMode::FIT_HEIGHT:
        {
            const Size& croppedSize = Size((imageTexPixSize.height * contentSize.width) / contentSize.height, contentSize.height);
            const Vec2& origin = (imageTexPixSize / 2.0f) - (croppedSize / 2.0f);
            texRect = Rect(origin, croppedSize);
            break;
        }
        case ImageScaleMode::SHOW_ALL:
        {
            break;
        }
        case ImageScaleMode::FILL_ALL:
        {
            const float scaleW = contentSize.width / imageTexPixSize.width;
            const float scaleH = contentSize.height / imageTexPixSize.height;
            const bool scaleToWidth = scaleW > scaleH;
            const Size& croppedSize = scaleToWidth ? Size(imageTexPixSize.width, (imageTexPixSize.width * contentSize.height) / contentSize.width) : Size((imageTexPixSize.height * contentSize.width) / contentSize.height, contentSize.height);
            const Vec2& origin = (imageTexPixSize / 2.0f) - (croppedSize / 2.0f);
            texRect = Rect(origin, croppedSize);
            break;
        }
    }
    _contentImage->setTextureRect(texRect);
    _contentImage->setContentSize(contentSize);
}

void RoundedRectTile::elementDisappeared(cocos2d::Node *sender)
{
    if(_contentImage->getTexture()->getPath() != _placholderFilename)
    {
        _contentImage->setTexture(_placholderFilename);
        resizeContentImage();
    }
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
        if(_contentImage->getTexture()->getPath() != _placholderFilename)
        {
            _contentImage->setTexture(_placholderFilename);
            resizeContentImage();
        }
    }
}

// delegate functions
void RoundedRectTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _contentImage->setTexture(downloader->getLocalImagePath());
    resizeContentImage();
}
void RoundedRectTile::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
