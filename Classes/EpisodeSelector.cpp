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
const float EpisodeSelector::kEpisodeBarHeight = 300.0f;

bool EpisodeSelector::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f, 1.0f));
    
    _stencil = ui::Scale9Sprite::create("res/hqscene/rounded_rect_20px.png");
    _stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    _contentClipper = ClippingNode::create(_stencil);
    _contentClipper->setAlphaThreshold(0.9f);
    _contentClipper->setPosition(Vec2(0,0));
    addChild(_contentClipper);
    
    _contentLayout = ui::Layout::create();
    _contentLayout->setSizeType(SizeType::PERCENT);
    _contentLayout->setSizePercent(Vec2(1.0f,1.0f));
    _contentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _contentLayout->setLayoutType(Type::VERTICAL);
    _contentClipper->addChild(_contentLayout);
    
    _headerLayout = ui::Layout::create();
    _headerLayout->setSizeType(SizeType::PERCENT);
    _headerLayout->setSizePercent(Vec2(1.0f, kHeaderHeightPercent));
    _headerLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _headerLayout->setBackGroundColor(Style::Color::azure);
    _contentLayout->addChild(_headerLayout);
    
    _bannerImage = Sprite::create();
    _bannerImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bannerImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _headerLayout->addChild(_bannerImage);
    
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
    _episodeListView->setBackGroundColorType(BackGroundColorType::SOLID);
    _episodeListView->setBackGroundColor(Style::Color::darkIndigoThree);
    _contentLayout->addChild(_episodeListView);
    
    const Color3B& gradientColour = Style::Color::darkIndigoThree;
    _bottomGradient = LayerGradient::create(Color4B(gradientColour.r, gradientColour.g, gradientColour.b, 0), Color4B(gradientColour.r, gradientColour.g, gradientColour.b, 255));
    _bottomGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
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
    
    _contentClipper->setContentSize(contentSize);
    _stencil->setContentSize(contentSize);
    _contentLayout->updateSizeAndPosition();
    _bannerImage->setScale(_bannerImage->getContentSize().height / (contentSize.height * 0.8f));
    _divider->setContentSize(Size(contentSize.width, 10));
    const Size& episodeBarSize = Size(contentSize.width - (2 * kListViewPadding), kEpisodeBarHeight);
    for(auto bar : _episodeBars)
    {
        bar->setContentSize(episodeBarSize);
    }
    _bottomGradient->setContentSize(Size(contentSize.width, kEpisodeBarHeight));
}

void EpisodeSelector::setHqData(const HQDataObjectRef& hqData)
{
    _hqData = hqData;
    setupEpisodeBars();
    ImageDownloaderRef downloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    downloader->downloadImage(this, _hqData->getGroupLogo());
    _divider->setBackGroundColor(Style::Color::macaroniAndCheese);
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

void EpisodeSelector::setupEpisodeBars()
{
    _episodeListView->removeAllItems();
    _episodeBars.clear();
    if(_hqData)
    {
        int episodeNumber = 1;
        auto carousels = _hqData->getHqCarousels();
        const Size& episodeBarSize = Size(getContentSize().width - (2 * kListViewPadding), kEpisodeBarHeight);
        for(auto carousel : carousels)
        {
            auto itemList = carousel->getContentItems();
            for(auto item : itemList)
            {
                EpisodeBar* bar = EpisodeBar::create();
                bar->setContentItemData(item);
                bar->setEpisodeNumber(episodeNumber++);
                bar->setEpisodeTagColour(Style::Color::macaroniAndCheese);
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

// delegate functions
void EpisodeSelector::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _bannerImage->setTexture(downloader->getLocalImagePath());
    _bannerImage->setScale(_bannerImage->getContentSize().height / (getContentSize().height * 0.8f)); //TODO: use proper banner image and scale to fill space
}
void EpisodeSelector::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
