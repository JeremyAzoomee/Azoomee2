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

const float FriendTile::kFrameThickness = 10.0f;

bool FriendTile::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _frame = ui::ImageView::create("res/hqscene/circle.png");
    _frame->ignoreContentAdaptWithSize(false);
    _frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _frame->setColor(Style::Color::macaroniAndCheese);
    addChild(_frame);
    
    _clippingStencil = Sprite::create("res/hqscene/circle.png");
    _clippingStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    
    _contentClipper = ClippingNode::create(_clippingStencil);
    _contentClipper->setAlphaThreshold(0.5f);
    addChild(_contentClipper);
    
    const Color3B& bgColour = Style::Color::darkIndigoThree;
    
    _bgColour = ui::ImageView::create("res/hqscene/circle.png");
    _bgColour->ignoreContentAdaptWithSize(false);
    _bgColour->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _bgColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _bgColour->setColor(bgColour);
    _contentClipper->addChild(_bgColour);
    
    _pattern = ui::ImageView::create("res/decoration/main_pattern_small.png");
    _pattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _pattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _pattern->setColor(Color3B::WHITE);
    _pattern->setScale(0.4f);
    _bgColour->addChild(_pattern);
    
    _circleGradient = LayerRadialGradient::create();
    _circleGradient->setStartColor(bgColour);
    _circleGradient->setStartOpacity(0);
    _circleGradient->setEndColor(bgColour);
    _circleGradient->setEndOpacity(255);
    _bgColour->addChild(_circleGradient);
    
    _oomee = ui::ImageView::create();
    _oomee->ignoreContentAdaptWithSize(false);
    _oomee->setAnchorPoint(Vec2(0.5, 0.15f));
    _oomee->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bgColour->addChild(_oomee);
    
    _friendName = DynamicText::create("", Style::Font::PoppinsBold(), 55);
    _friendName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _friendName->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _friendName->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _friendName->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    _friendName->setColor(Color3B::WHITE);
    _friendName->setOverflow(Label::Overflow::SHRINK);
    addChild(_friendName);
    
    addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_selectedCallback)
            {
                _selectedCallback(_friendData);
            }
        }
    });
    
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

void FriendTile::setSelectedCallback(const SelectedCallback &callback)
{
    _selectedCallback = callback;
}

void FriendTile::resizeContent()
{
    const Size& contentSize = getContentSize();
    _frame->setContentSize(Size(contentSize.width, contentSize.width));
    const Size& clipperSize = Size(contentSize.width - kFrameThickness, contentSize.width - kFrameThickness);
    _contentClipper->setContentSize(contentSize);
    _clippingStencil->setContentSize(clipperSize);
    _clippingStencil->setPosition(Vec2(contentSize.width / 2.0f, contentSize.height - (kFrameThickness / 2.0f)));
    _bgColour->setContentSize(clipperSize);
    _bgColour->setPosition(Vec2(contentSize.width / 2.0f, contentSize.height - (kFrameThickness / 2.0f)));
    _circleGradient->setContentSize(clipperSize);
    _circleGradient->setCenter(Vec2(clipperSize / 2.0f));
    _circleGradient->setRadius(clipperSize.height / 2.0f);
    _oomee->setContentSize(clipperSize * 1.3f);
    _friendName->setTextAreaSize(Size(contentSize.width, _friendName->getContentSize().height));
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
