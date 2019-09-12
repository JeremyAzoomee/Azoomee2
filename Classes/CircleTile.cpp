//
//  CircleTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 05/09/2019.
//

#include "CircleTile.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "HQDataProvider.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float CircleTile::kFrameThickness = 12.0f;

bool CircleTile::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    cocos2d::ui::ImageView* bg = ui::ImageView::create("res/hqscene/circle.png");
    bg->ignoreContentAdaptWithSize(false);
    bg->setSizeType(SizeType::PERCENT);
    bg->setSizePercent(Vec2(1.0f, 1.0f));
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    bg->setColor(Style::Color::macaroniAndCheese);
    addChild(bg);
    
    _clippingStencil = Sprite::create("res/hqscene/circle.png");
    _clippingStencil->setContentSize(getContentSize() - Size(kFrameThickness,kFrameThickness));
    _clippingStencil->setPosition(getContentSize() / 2.0f);
    _clippingStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
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

void CircleTile::onEnter()
{
    startCheckingForOnScreenPosition(this);
    Super::onEnter();
}

void CircleTile::onExit()
{
    endCheck();
    _imageDownloader->setDelegate(nullptr);
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

void CircleTile::elementDisappeared(cocos2d::Node *sender)
{
    _contentImage->loadTexture("res/contentPlaceholders/Games1X1.png");
    onSizeChanged();
}

void CircleTile::elementAppeared(cocos2d::Node *sender)
{
    if(_contentItem)
    {
        _imageDownloader->downloadImage(this, HQDataProvider::getInstance()->getThumbnailUrlForItem(_contentItem, TILESIZE_1X1));
    }
    else
    {
        elementOnScreen = false;
    }
}

// delegate functions
void CircleTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _contentImage->loadTexture(downloader->getLocalImagePath());
    onSizeChanged();
}
void CircleTile::onImageDownloadFailed()
{
    elementOnScreen = false;
}

NS_AZOOMEE_END


