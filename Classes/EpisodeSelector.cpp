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
    _headerLayout->setSizePercent(Vec2(1.0f, 0.29f));
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
    _episodeListView->setSizePercent(Vec2(1.0f, 0.71f));
    _episodeListView->setDirection(ui::ListView::Direction::VERTICAL);
    _episodeListView->setBounceEnabled(true);
    _episodeListView->setItemsMargin(30);
    _episodeListView->setPadding(kListViewPadding, kListViewPadding, kListViewPadding, kListViewPadding);
    _episodeListView->setGravity(ui::ListView::Gravity::LEFT);
    _episodeListView->setBackGroundColorType(BackGroundColorType::SOLID);
    _episodeListView->setBackGroundColor(Style::Color::darkIndigoThree);
    _contentLayout->addChild(_episodeListView);
    
    const Color3B& gradientColour = Style::Color::darkIndigoThree;
    _bottomGradient = LayerGradient::create(Color4B(gradientColour.r, gradientColour.g, gradientColour.b, 0), Color4B(gradientColour.r, gradientColour.g, gradientColour.b, 255));
    _bottomGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _contentLayout->addChild(_bottomGradient);
    
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
    _contentClipper->setContentSize(getContentSize());
    _stencil->setContentSize(getContentSize());
    _contentLayout->updateSizeAndPosition();
    _bannerImage->setScale(_bannerImage->getContentSize().height / (getContentSize().height * 0.8f));
    _divider->setContentSize(Size(getContentSize().width, 10));
    for(auto bar : _episodeBars)
    {
        bar->setContentSize(Size(getContentSize().width - (2 * kListViewPadding), 300));
    }
    _bottomGradient->setContentSize(Size(getContentSize().width, 300));
}

void EpisodeSelector::setHqData(const HQDataObjectRef& hqData)
{
    _hqData = hqData;
    setupEpisodeBars();
    ImageDownloaderRef downloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    downloader->downloadImage(this, _hqData->getGroupLogo());
    _divider->setBackGroundColor(Style::Color::macaroniAndCheese);
    onSizeChanged();
}

void EpisodeSelector::setContentSelectedCallback(const ContentSelectedCallback& callback)
{
    _callback = callback;
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
        int i = 1;
        auto carousels = _hqData->getHqCarousels();
        for(auto carousel : carousels)
        {
            auto itemList = carousel->getContentItems();
            for(auto item : itemList)
            {
                EpisodeBar* bar = EpisodeBar::create();
                bar->setContentItemData(item);
                bar->setEpisodeNumber(i++);
                bar->setEpisodeTagColour(Style::Color::macaroniAndCheese);
                bar->setContentSize(Size(getContentSize().width - (2 * kListViewPadding), 300));
                bar->setContentSelectedCallback([this, i](HQContentItemObjectRef content){
                    if(_callback)
                    {
                        _callback(content, i);
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
    _bannerImage->setScale(_bannerImage->getContentSize().height / (getContentSize().height * 0.8f));
}
void EpisodeSelector::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
