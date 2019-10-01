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

const Vec2 EpisodeBar::kDropShadowPadding = Vec2(52,52);
const float EpisodeBar::kTextPadding = 60.0f;

bool EpisodeBar::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _dropShadow = ui::ImageView::create("res/hqscene/episode_bar.png");
    _dropShadow->setScale9Enabled(true);
    _dropShadow->ignoreContentAdaptWithSize(false);
    _dropShadow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _dropShadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(_dropShadow);
    
    _contentImage = RoundedRectSprite::create();
    _contentImage->setCornerRadius(25);
    _contentImage->setRoundedCorners(true, false, true, false);
    _contentImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _contentImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    addChild(_contentImage);
    
    _lockedOverlay = RoundedRectSprite::create();
    _lockedOverlay->setTexture("res/decoration/white_1px.png");
    _lockedOverlay->setCornerRadius(25);
    _lockedOverlay->setRoundedCorners(true, false, true, false);
    _lockedOverlay->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _lockedOverlay->setColor(Style::Color::darkIndigo);
    _lockedOverlay->setOpacity(204);
    _lockedOverlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->addChild(_lockedOverlay);
    
    _playIcon = ui::ImageView::create("res/hqscene/play_icon.png");
    _playIcon->ignoreContentAdaptWithSize(false);
    _playIcon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _playIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->addChild(_playIcon);
    
    _textLayout = ui::Layout::create();
    _textLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _textLayout->setLayoutType(Type::VERTICAL);
    addChild(_textLayout);
    
    _episodeTag = DynamicText::create("", Style::Font::PoppinsMedium(), 40);
    _episodeTag->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _episodeTag->setTextVerticalAlignment(TextVAlignment::CENTER);
    _episodeTag->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    _textLayout->addChild(_episodeTag);
    
    _episodeTitle = DynamicText::create("", Style::Font::PoppinsBold(), 48);
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
    onSizeChanged();
}

void EpisodeBar::onExit()
{
    stopCheckingOnScreenPosition();
    _imageDownloader->setDelegate(nullptr);
    Super::onExit();
}

void EpisodeBar::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    const Size& sizeWithShadow = contentSize + Size(kDropShadowPadding * 2);
    
    _dropShadow->setContentSize(sizeWithShadow);
    
    resizeImageAndText();
}

void EpisodeBar::resizeImageAndText()
{
    
    const Size& contentSize = getContentSize();

    const float imageWidth = ((contentSize.height * 4.0f) / 3.0f);
    
    _contentImage->setContentSize(Size(imageWidth, contentSize.height));
    _lockedOverlay->setContentSize(_contentImage->getContentSize());
    _playIcon->setContentSize(Size(contentSize.height / 2.0f, contentSize.height / 2.0f));
    
    const float textMaxWidth = contentSize.width - imageWidth - (2 * kTextPadding);
    
    _episodeTag->setMaxLineWidth(textMaxWidth);
    _episodeTitle->setMaxLineWidth(textMaxWidth);
    
    _textLayout->setPosition(Vec2(imageWidth + kTextPadding,contentSize.height / 2.0f));
    _textLayout->setContentSize(Size(textMaxWidth, _episodeTag->getContentSize().height + _episodeTitle->getContentSize().height));
    const float maxTextHeight = contentSize.height * 0.9f;
    if(_textLayout->getContentSize().height > maxTextHeight)
    {
        _textLayout->setScale(maxTextHeight / _textLayout->getContentSize().height);
    }
    else
    {
        _textLayout->setScale(1.0f);
    }
    _textLayout->updateSizeAndPosition();
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
        _lockedOverlay->setVisible(!_contentItem->isEntitled());
        _playIcon->loadTexture(_contentItem->isEntitled() ? "res/hqscene/play_icon.png" : "res/hqscene/oomee_padlock.png");
    }
}

void EpisodeBar::setEpisodeTagColour(const Color3B& colour)
{
    _episodeTag->setTextColor(Color4B(colour));
}

void EpisodeBar::elementDisappeared(cocos2d::Node *sender)
{
    _contentImage->setTexture("res/contentPlaceholders/Games1X1.png");
    resizeImageAndText();
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
    _contentImage->setTexture(downloader->getLocalImagePath());
    resizeImageAndText();
}
void EpisodeBar::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
