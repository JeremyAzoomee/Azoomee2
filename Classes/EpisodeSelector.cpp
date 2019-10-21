//
//  EpisodeSelector.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 20/09/2019.
//

#include "EpisodeSelector.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float EpisodeSelector::kListViewPadding = 45.0f;
const float EpisodeSelector::kHeaderHeightPercent = 0.29f;
const float EpisodeSelector::kEpisodeBarSpacing = 30.0f;
const float EpisodeSelector::kEpisodeBarHeightPortrait = 300.0f;
const float EpisodeSelector::kEpisodeBarHeightLandscape = 240.0f;

bool EpisodeSelector::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f, 1.0f));
    
    _episodeBarHeight = kEpisodeBarHeightLandscape;
    
    _background = RoundedRectSprite::create();
    _background->setTexture("res/decoration/white_1px.png");
    _background->setCornerRadius(20);
    _background->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->setColor(Style::Color::darkIndigoThree);
    _background->setStretchImageEnabled(true);
    addChild(_background, -1);
    
    _contentLayout = ui::Layout::create();
    _contentLayout->setSizeType(SizeType::PERCENT);
    _contentLayout->setSizePercent(Vec2(1.0f,1.0f));
    _contentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _contentLayout->setLayoutType(Type::VERTICAL);
    addChild(_contentLayout);
    
    _headerLayout = ui::Layout::create();
    _headerLayout->setSizeType(SizeType::PERCENT);
    _headerLayout->setSizePercent(Vec2(1.0f, kHeaderHeightPercent));
    _contentLayout->addChild(_headerLayout);
    
    _bannerImage = RoundedRectSprite::create();
    _bannerImage->setRoundedCorners(false, false, true, true);
    _bannerImage->setCornerRadius(20);
    _bannerImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bannerImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bannerImage->setScaleMode(RoundedRectSprite::ScaleMode::FILL);
    _headerLayout->addChild(_bannerImage);
    
    _logoImage = Sprite::create();
    _logoImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _logoImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _headerLayout->addChild(_logoImage);
    
    _divider = cocos2d::ui::Layout::create();
    _divider->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _divider->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _divider->setBackGroundColorType(BackGroundColorType::SOLID);
    _headerLayout->addChild(_divider);
    
    _episodeListView = ui::ListView::create();
    _episodeListView->setSizeType(SizeType::PERCENT);
    _episodeListView->setSizePercent(Vec2(1.0f, 1.0f - kHeaderHeightPercent));
    _episodeListView->setDirection(ui::ListView::Direction::VERTICAL);
    _episodeListView->setBounceEnabled(true);
    _episodeListView->setItemsMargin(kEpisodeBarSpacing);
    _episodeListView->setPadding(kListViewPadding, kListViewPadding, kListViewPadding, kListViewPadding);
    _episodeListView->setGravity(ui::ListView::Gravity::LEFT);
    _contentLayout->addChild(_episodeListView);
    
    //const Color3B& gradientColour = Style::Color::darkIndigoThree;
    //_bottomGradient = LayerGradient::create(Color4B(gradientColour.r, gradientColour.g, gradientColour.b, 0), Color4B(gradientColour.r, gradientColour.g, gradientColour.b, 255));
    _bottomGradient = RoundedRectSprite::create();
    _bottomGradient->setTexture("res/hqscene/gradient_overlay_large.png");
    _bottomGradient->setStretchImageEnabled(true);
    _bottomGradient->setCornerRadius(20);
    _bottomGradient->setRoundedCorners(true, true, false, false);
    _bottomGradient->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    //_bottomGradient->setPosition(Vec2(0,-1));
    _bottomGradient->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
    _bottomGradient->setColor(Style::Color::darkIndigoThree);
    _contentLayout->addChild(_bottomGradient);
    
    _closeButton = ui::Button::create("res/hqscene/episode_select_close.png");
    _closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    _closeButton->ignoreContentAdaptWithSize(false);
    _closeButton->setContentSize(Size(175,175));
    _closeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_closeCallback)
            {
                _closeCallback();
            }
        }
    });
    _headerLayout->addChild(_closeButton);
    
    _bannerDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    _logoDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    
    return true;
}

void EpisodeSelector::onEnter()
{
    Super::onEnter();
}

void EpisodeSelector::onExit()
{
    Super::onExit();
}

void EpisodeSelector::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    
    _background->setContentSize(contentSize);
    _contentLayout->updateSizeAndPosition();
    _logoImage->setScale((_headerLayout->getContentSize().height * 0.8f) / _logoImage->getContentSize().height);
    resizeBannerImage();
    _divider->setContentSize(Size(contentSize.width, 6));
    const Size& episodeBarSize = Size(contentSize.width - (2 * kListViewPadding), _episodeBarHeight);
    for(auto bar : _episodeBars)
    {
        bar->setContentSize(episodeBarSize);
    }
    _bottomGradient->setContentSize(Size(contentSize.width, _episodeBarHeight));
    _headerLayout->updateSizeAndPosition();
}

void EpisodeSelector::setHqData(const HQDataObjectRef& hqData)
{
    _hqData = hqData;
    setupEpisodeBars();
    _bannerImage->setVisible(false);
    _logoImage->setVisible(false);
    _logoDownloader->downloadImage(this, _hqData->getGroupLogo());
    _bannerDownloader->downloadImage(this, _hqData->getGroupBanner());
}

void EpisodeSelector::setContentSelectedCallback(const ContentSelectedCallback& callback)
{
    _callback = callback;
}

void EpisodeSelector::setCloseButtonCallback(const CloseButtonCallback& callback)
{
    _closeCallback = callback;
}

void EpisodeSelector::enableCloseButton(bool enable)
{
    _closeButton->setVisible(enable);
}

void EpisodeSelector::toggleBottomGradient(bool enabled)
{
    _bottomGradient->setVisible(enabled);
}

void EpisodeSelector::setLineAndTextColour(const Color3B& colour)
{
    _lineAndTextColour = colour;
    _divider->setBackGroundColor(_lineAndTextColour);
    for(auto episodeBar : _episodeBars)
    {
        episodeBar->setEpisodeTagColour(_lineAndTextColour);
    }
}

void EpisodeSelector::setEpisodeBarHeight(float height)
{
    _episodeBarHeight = height;
    const Size& episodeBarSize = Size(getContentSize().width - (2 * kListViewPadding), _episodeBarHeight);
    for(auto bar : _episodeBars)
    {
        bar->setContentSize(episodeBarSize);
    }

}

void EpisodeSelector::setupEpisodeBars()
{
    _episodeListView->removeAllItems();
    _episodeBars.clear();
    if(_hqData)
    {
        int episodeNumber = 1;
        auto carousels = _hqData->getHqCarousels();
        const Size& episodeBarSize = Size(getContentSize().width - (2 * kListViewPadding), _episodeBarHeight);
        for(auto carousel : carousels)
        {
            auto itemList = carousel->getContentItems();
            for(auto item : itemList)
            {
                EpisodeBar* bar = EpisodeBar::create();
                bar->setContentItemData(item);
                bar->setEpisodeNumber(episodeNumber++);
                bar->setEpisodeTagColour(_lineAndTextColour);
                bar->setContentSize(episodeBarSize);
                bar->setContentSelectedCallback([this, episodeNumber](HQContentItemObjectRef content){
                    if(_callback)
                    {
                        _callback(content, episodeNumber);
                    }
                });
                _episodeListView->pushBackCustomItem(bar);
                _episodeBars.pushBack(bar);
            }
        }
    }
    _episodeListView->scrollToTop(0, false);
}

void EpisodeSelector::resizeBannerImage()
{
    _bannerImage->setContentSize(_headerLayout->getContentSize());
}

// delegate functions
void EpisodeSelector::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    if(downloader == _logoDownloader)
    {
        _logoImage->setVisible(true);
        _logoImage->setTexture(downloader->getLocalImagePath());
        _logoImage->setScale((_headerLayout->getContentSize().height * 0.8f) / _logoImage->getContentSize().height);
    }
    else if(downloader == _bannerDownloader)
    {
        _bannerImage->setVisible(true);
        _bannerImage->setTexture(downloader->getLocalImagePath());
        resizeBannerImage();
    }
}
void EpisodeSelector::onImageDownloadFailed()
{

}

NS_AZOOMEE_END
