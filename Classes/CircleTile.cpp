//
//  CircleTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 05/09/2019.
//

#include "CircleTile.h"
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/ImageDownloader/ImageDownloaderCacheCleanerLogic.h>
#include <TinizineCommon/Data/HQDataObject/ContentItemManager.h>
#include "HQConstants.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

const float CircleTile::kFrameThickness = 12.0f;

bool CircleTile::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _bgColour = ui::ImageView::create("res/hqscene/circle.png");
    _bgColour->ignoreContentAdaptWithSize(false);
    _bgColour->setSizeType(SizeType::PERCENT);
    _bgColour->setSizePercent(Vec2(1.0f, 1.0f));
    _bgColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgColour->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgColour->setColor(Colours::Color_3B::darkIndigoThree);
    addChild(_bgColour);
    
    _clippingStencil = Sprite::create("res/hqscene/circle.png");
    _clippingStencil->setContentSize(getContentSize() - Size(kFrameThickness,kFrameThickness));
    _clippingStencil->setPosition(getContentSize() / 2.0f);
    _clippingStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _contentClipper = ClippingNode::create(_clippingStencil);
    _contentClipper->setAlphaThreshold(0.5f);
    _contentClipper->setPosition(Vec2(0,0));
    _contentClipper->setContentSize(getContentSize());
    addChild(_contentClipper);
    
    _contentImage = ui::ImageView::create("res/hqscene/game_icon.png");
    _contentImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _contentClipper->addChild(_contentImage);
    
    return true;
}

void CircleTile::onEnter()
{
    _contentImage->loadTexture(_placholderFilename);
    startCheckingForOnScreenPosition(this);
    Super::onEnter();
}

void CircleTile::onExit()
{
    if(!checkIfElementIsOnScreen(this))
    {
        elementOnScreen = false;
        elementDisappeared(this);
    }
    stopCheckingOnScreenPosition();
    Super::onExit();
}

void CircleTile::onSizeChanged()
{
    Super::onSizeChanged();
    const Size& contentSize = getContentSize();
    _contentClipper->setContentSize(contentSize);
    _clippingStencil->setPosition(contentSize / 2.0f);
    _clippingStencil->setContentSize(contentSize - Size(kFrameThickness,kFrameThickness));
    _contentImage->setScale(MAX(contentSize.height / _contentImage->getContentSize().height, contentSize.width / _contentImage->getContentSize().width));
}

void CircleTile::setContentItemData(const HQContentItemObjectRef& contentItem)
{
    _contentItem = contentItem;
    _bgColour->setColor(_contentItem ? Colours::Color_3B::macaroniAndCheese : Colours::Color_3B::darkIndigoThree);
}

void CircleTile::setEmptyImage(const std::string& filename)
{
    _emptyImageFilename = filename;
}

void CircleTile::elementDisappeared(cocos2d::Node *sender)
{
    if(_contentImage->getRenderFile().file != _placholderFilename)
    {
        _contentImage->loadTexture(_placholderFilename);
        ImageDownloaderCacheCleanerLogic::getInstance()->imageRemoved();
        const Size& contentSize = getContentSize();
        const Size& imageSize = _contentImage->getContentSize();
        _contentImage->setScale(MAX(contentSize.height / imageSize.height, contentSize.width / imageSize.width));
    }
}

void CircleTile::elementAppeared(cocos2d::Node *sender)
{
    if(_contentItem)
    {
        _imageDownloader->downloadImage(this, ContentItemManager::getInstance()->getThumbnailUrlForItem(_contentItem, TILESIZE_3X3));
    }
    else
    {
        elementOnScreen = false;
        if(_contentImage->getRenderFile().file != _placholderFilename)
        {
            _contentImage->loadTexture(_placholderFilename);
            const Size& contentSize = getContentSize();
            const Size& imageSize = _contentImage->getContentSize();
            _contentImage->setScale(MAX(contentSize.height / imageSize.height, contentSize.width / imageSize.width));
        }
    }
}

// delegate functions
void CircleTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _contentImage->loadTexture(downloader->getLocalImagePath());
    const Size& contentSize = getContentSize();
    const Size& imageSize = _contentImage->getContentSize();
    _contentImage->setScale(MAX(contentSize.height / imageSize.height, contentSize.width / imageSize.width));
}
void CircleTile::onImageDownloadFailed()
{
    if(_imageDownloader->getUrl() == ContentItemManager::getInstance()->getThumbnailUrlForItem(_contentItem, TILESIZE_3X3))
    {
        _imageDownloader->downloadImage(this, ContentItemManager::getInstance()->getThumbnailUrlForItem(_contentItem, TILESIZE_1X1));
    }
}

NS_AZ_END


