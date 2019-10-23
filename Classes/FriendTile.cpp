//
//  FriendTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 23/10/2019.
//

#include "FriendTile.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool FriendTile::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _frame = ui::ImageView::create("res/hqscene/circle.png");
    _frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _frame->setColor(Style::Color::macaroniAndCheese);
    addChild(_frame);
    
    _clippingStencil = Sprite::create("res/hqscene/circle.png");
    _clippingStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    
    _contentClipper = ClippingNode::create(_clippingStencil);
    _contentClipper->setAlphaThreshold(0.5f);
    _contentClipper->setIgnoreAnchorPointForPosition(false);
    _contentClipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    addChild(_contentClipper);
    
    _frame = ui::ImageView::create("res/hqscene/circle.png");
    _frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _frame->setColor(Style::Color::darkIndigoThree);
    _contentClipper->addChild(_frame);
    
    _pattern = ui::ImageView::create("res/decoration/main_pattern_small.png");
    _pattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _pattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _contentClipper->addChild(_pattern);
    
    _oomee = ui::ImageView::create();
    _oomee->ignoreContentAdaptWithSize(false);
    _oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _oomee->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _contentClipper->addChild(_oomee);
    
    _friendName = DynamicText::create("", Style::Font::PoppinsBold(), 70);
    _friendName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _friendName->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _friendName->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _friendName->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    _friendName->setColor(Color3B::WHITE);
    addChild(_friendName);
    
    return true;
}

void FriendTile::onEnter()
{
    Super::onEnter();
}

void FriendTile::onExit()
{
    Super::onExit();
}

void FriendTile::onSizeChanged()
{
    Super::onSizeChanged();
}

void FriendTile::setFriendData(const Chat::FriendRef& friendData)
{
    _friendData = friendData;
    if(_friendData)
    {
        _friendName->setString(_friendData->friendName());
        ImageDownloaderRef avatarDownloader = ImageDownloader::create("avatars", ImageDownloader::CacheMode::File);
        avatarDownloader->downloadImage(this, _friendData->avatarURL());
    }
}

void FriendTile::setTileWidth(float width)
{
    setContentSize(Size(width, width * 1.5f));
    resizeContent();
}

void FriendTile::resizeContent()
{
    const Size& contentSize = getContentSize();
    _frame->setContentSize(Size(contentSize.width, contentSize.width));
    const Size& clipperSize = Size(contentSize.width - kFrameThickness, contentSize.width - kFrameThickness);
    _contentClipper->setContentSize(clipperSize);
    _contentClipper->setPosition(Vec2(contentSize.width / 2.0f, contentSize.height - (0.5f * kFrameThickness)));
    _oomee->setContentSize(clipperSize);
}

// delegate functions
void FriendTile::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _oomee->loadTexture(downloader->getLocalImagePath());
}

void FriendTile::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
