//
//  RoundedRectTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 06/09/2019.
//

#include "RoundedRectTile.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloaderCacheCleanerLogic.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemManager.h>

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
    _contentImage->setScaleMode(RoundedRectSprite::ScaleMode::FILL);
    addChild(_contentImage);
    
    _lockedOverlay = RoundedRectSprite::create();
    _lockedOverlay->setTexture("res/decoration/white_1px.png");
    _lockedOverlay->setCornerRadius(60);
    _lockedOverlay->setColor(Colours::Color_3B::darkIndigo);
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
    if(!checkIfElementIsOnScreen(this))
    {
        elementOnScreen = false;
        elementDisappeared(this);
    }
    stopCheckingOnScreenPosition();
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
    _contentImage->setContentSize(getContentSize());
}

void RoundedRectTile::elementDisappeared(cocos2d::Node *sender)
{
    if(_contentImage->getTexture()->getPath() != _placholderFilename)
    {
        _contentImage->setTexture(_placholderFilename);
        ImageDownloaderCacheCleanerLogic::getInstance()->imageRemoved();
        resizeContentImage();
    }
}

void RoundedRectTile::elementAppeared(cocos2d::Node *sender)
{
    if(_contentItem && isVisible())
    {
        _imageDownloader->downloadImage(this, ContentItemManager::getInstance()->getThumbnailUrlForItem(_contentItem, _imageShape));
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
