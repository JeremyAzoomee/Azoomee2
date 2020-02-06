//
//  RecentMessageBar.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 28/10/2019.
//

#include "RecentMessageBar.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const Vec2 RecentMessageBar::kDropShadowPadding = Vec2(52,52);
const float RecentMessageBar::kTextPadding = 60.0f;
const cocos2d::Vec2 RecentMessageBar::kAvatarTileAspectRatio = Vec2(4.0f ,3.0f);

bool RecentMessageBar::init()
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
    
    _avatarBG = RoundedRectSprite::create();
    _avatarBG->setTexture("res/decoration/white_1px.png");
    _avatarBG->setCornerRadius(25);
    _avatarBG->setRoundedCorners(true, false, true, false);
    _avatarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _avatarBG->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _avatarBG->setStretchImageEnabled(true);
    _avatarBG->setColor(Style::Color::darkIndigoThree);
    addChild(_avatarBG);
    
    _pattern = RoundedRectSprite::create();
    _pattern->setTexture("res/decoration/pattern_stem_tile.png");
    _pattern->setCornerRadius(25);
    _pattern->setRoundedCorners(true, false, true, false);
    _pattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _pattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _pattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    _pattern->setTileScaleFactor(0.5f);
    _pattern->setOpacity(100);
    addChild(_pattern);
    
    _avatarClip = ui::Layout::create();
    _avatarClip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _avatarClip->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _avatarClip->setClippingEnabled(true);
    addChild(_avatarClip);
    
    _avatar = ui::ImageView::create();
    _avatar->setAnchorPoint(Vec2(0.5, 0.15f));
    _avatar->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _avatar->ignoreContentAdaptWithSize(false);
    _avatarClip->addChild(_avatar);
    
    _textLayout = ui::Layout::create();
    _textLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _textLayout->setLayoutType(Type::VERTICAL);
    addChild(_textLayout);
    
    _senderName = DynamicText::create("", Style::Font::PoppinsMedium(), 40);
    _senderName->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _senderName->setTextVerticalAlignment(TextVAlignment::CENTER);
    _senderName->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    _senderName->setTextColor(Color4B(Style::Color::macaroniAndCheese));
    _textLayout->addChild(_senderName);
    
    _messageText = DynamicText::create("", Style::Font::PoppinsBold(), 48);
    _messageText->setTextColor(Color4B::WHITE);
    _messageText->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _messageText->setTextVerticalAlignment(TextVAlignment::TOP);
    _messageText->setOverflow(Label::Overflow::CLAMP);
    _textLayout->addChild(_messageText);
    
    _imageDownloader = ImageDownloader::create(ImageDownloader::kAvatarImageCacheFolder, ImageDownloader::CacheMode::File);
    
    addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_callback && _message.first)
            {
                _callback(_message.first);
            }
        }
    });
    
    return true;
}

void RecentMessageBar::onEnter()
{
    Super::onEnter();
}

void RecentMessageBar::onExit()
{
    _imageDownloader->setDelegate(nullptr);
    Super::onExit();
}

void RecentMessageBar::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    const Size& sizeWithShadow = contentSize + Size(kDropShadowPadding * 2);
    
    _dropShadow->setContentSize(sizeWithShadow);
    
    resizeImageAndText();
    forceDoLayout();
}

void RecentMessageBar::resizeImageAndText()
{
    
    const Size& contentSize = getContentSize();
    
    const float imageWidth = ((contentSize.height * kAvatarTileAspectRatio.x) / kAvatarTileAspectRatio.y);
    
    const Size& imgSize = Size(imageWidth, contentSize.height);
    
    _avatarBG->setContentSize(imgSize);
    _pattern->setContentSize(imgSize);
    _avatarClip->setContentSize(imgSize);
    _avatar->setContentSize(Size(imageWidth, imageWidth));
    
    const float textMaxWidth = contentSize.width - imageWidth - (2 * kTextPadding);
    
    _senderName->setMaxLineWidth(textMaxWidth);

    const std::string& text = _message.second->messageType() != Chat::Message::MessageTypeText ? _(Chat::Message::kSentMessageDescription.at(_message.second->messageType())) : _message.second->messageText();
    _messageText->setString(text);
    if(_messageText->getContentSize().width > textMaxWidth && textMaxWidth > 0)
    {
        reduceLabelTextToFitWidth(_messageText, textMaxWidth);
    }
    
    _textLayout->setPosition(Vec2(imageWidth + kTextPadding,contentSize.height / 2.0f));
    _textLayout->setContentSize(Size(textMaxWidth, _senderName->getContentSize().height + _messageText->getContentSize().height));
    _textLayout->updateSizeAndPosition();
}

void RecentMessageBar::setMessageData(const RecentMessage& message)
{
    _message = message;
    if(_message.first)
    {
        _imageDownloader->downloadImage(this, _message.first->avatarURL());
        _senderName->setString(_message.first->friendName());
        setTouchEnabled(true);
    }
}

void RecentMessageBar::setMessageSelectedCallback(const MessageSelectedCallback &callback)
{
    _callback = callback;
}

void RecentMessageBar::setBackgroundColour(const cocos2d::Color3B &colour)
{
    _dropShadow->setColor(colour);
}

// delegate functions
void RecentMessageBar::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _avatar->loadTexture(downloader->getLocalImagePath());
    resizeImageAndText();
}
void RecentMessageBar::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END

