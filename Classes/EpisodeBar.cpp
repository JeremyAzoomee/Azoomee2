//
//  EpisodeBar.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 23/09/2019.
//

#include "EpisodeBar.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Strings.h>
#include "HQDataProvider.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const Vec2 EpisodeBar::kDropShadowPadding = Vec2(50,50);

bool EpisodeBar::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _dropShadow = ui::ImageView::create("res/hqscene/episode_bar.png");
    _dropShadow->setScale9Enabled(true);
    _dropShadow->ignoreContentAdaptWithSize(false);
    _dropShadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _dropShadow->setPosition(-kDropShadowPadding);
    addChild(_dropShadow);
    
    _clippingStencil = ui::Scale9Sprite::create("res/hqscene/episode_bar.png");
    _clippingStencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _clippingStencil->setPosition(-kDropShadowPadding);
    
    _contentClipper = ClippingNode::create(_clippingStencil);
    _contentClipper->setAlphaThreshold(0.9f);
    addChild(_contentClipper);
    
    _contentImage = ui::ImageView::create();
    _contentImage->ignoreContentAdaptWithSize(false);
    _contentImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _contentImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _contentClipper->addChild(_contentImage);
    
    _playIcon = ui::ImageView::create("res/hqscene/play_icon.png");
    _playIcon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _playIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->addChild(_playIcon);
    
    _textLayout = ui::Layout::create();
    _textLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _textLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    _textLayout->setLayoutType(Type::VERTICAL);
    _contentClipper->addChild(_textLayout);
    
    _episodeTag = DynamicText::create(StringUtils::format("%s %d",_("Episode").c_str(),0), Style::Font::PoppinsMedium(), 53);
    _episodeTag->setLayoutParameter(CreateLeftLinearLayoutParam(ui::Margin(60,0,60,0)));
    _episodeTag->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _episodeTag->setTextVerticalAlignment(TextVAlignment::CENTER);
    _episodeTag->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    _textLayout->addChild(_episodeTag);
    
    _episodeTitle = DynamicText::create("", Style::Font::PoppinsBold(), 61);
    _episodeTitle->setLayoutParameter(CreateLeftLinearLayoutParam(ui::Margin(60,0,60,0)));
    _episodeTitle->setTextColor(Color4B::WHITE);
    _episodeTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _episodeTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _episodeTitle->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    _textLayout->addChild(_episodeTitle);
    
    return true;
}

void EpisodeBar::onEnter()
{
    startCheckingForOnScreenPosition(this);
    Super::onEnter();
}

void EpisodeBar::onExit()
{
    endCheck();
    _imageDownloader->setDelegate(nullptr);
    Super::onExit();
}

void EpisodeBar::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    const Size& sizeWithShadow = contentSize + Size(kDropShadowPadding * 2);
    
    _contentClipper->setContentSize(contentSize);
    _clippingStencil->setContentSize(sizeWithShadow);
    _dropShadow->setContentSize(sizeWithShadow);
    
    const float imageWidth = ((contentSize.height * 4.0f) / 3.0f);
    
    _contentImage->setContentSize(Size(imageWidth, contentSize.height));
    
    const float textMaxWidth = contentSize.width - imageWidth - 120;
    
    _episodeTag->setMaxLineWidth(textMaxWidth);
    _episodeTitle->setMaxLineWidth(textMaxWidth);
    
    _textLayout->setContentSize(Size(textMaxWidth, _episodeTag->getContentSize().height + _episodeTitle->getContentSize().height));
    
}

void EpisodeBar::setEpisodeNumber(int episodeNumber)
{
    _episodeTag->setString(StringUtils::format("%s %d",_("Episode").c_str(),episodeNumber));
    _dropShadow->setColor(episodeNumber % 2 ? Style::Color::darkIndigo: Style::Color::darkIndigoTwo);
}

void EpisodeBar::setContentItemData(const HQContentItemObjectRef& contentItem)
{
    _contentItem = contentItem;
    if(_contentItem)
    {
        _episodeTitle->setString(_contentItem->getTitle());
    }
}

void EpisodeBar::setEpisodeTagColour(const Color3B& colour)
{
    _episodeTag->setTextColor(Color4B(colour));
}

void EpisodeBar::elementDisappeared(cocos2d::Node *sender)
{
    _contentImage->loadTexture("res/contentPlaceholders/Games1X1.png");
    onSizeChanged();
}

void EpisodeBar::elementAppeared(cocos2d::Node *sender)
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
void EpisodeBar::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _contentImage->loadTexture(downloader->getLocalImagePath());
}
void EpisodeBar::onImageDownloadFailed()
{
    elementOnScreen = false;
}

NS_AZOOMEE_END
