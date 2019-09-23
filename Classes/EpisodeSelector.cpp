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

bool EpisodeSelector::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f, 1.0f));
    
    _contentLayout = ui::Layout::create();
    _contentLayout->setSizeType(SizeType::PERCENT);
    _contentLayout->setSizePercent(Vec2(1.0f,1.0f));
    _contentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _contentLayout->setLayoutType(Type::VERTICAL);
    addChild(_contentLayout);
    
    _headerLayout = ui::Layout::create();
    _headerLayout->setSizeType(SizeType::PERCENT);
    _headerLayout->setSizePercent(Vec2(1.0f, 0.29f));
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
    _episodeListView->setPadding(45, 45, 45, 45);
    _episodeListView->setBackGroundColorType(BackGroundColorType::SOLID);
    _episodeListView->setBackGroundColor(Style::Color::darkIndigoThree);
    _contentLayout->addChild(_episodeListView);
    
    
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
    
    _bannerImage->setScale(_bannerImage->getContentSize().height / (getContentSize().height * 0.8f));
    _divider->setContentSize(Size(getContentSize().width, 10));
    for(auto bar : _episodeBars)
    {
        bar->setContentSize(Size(getContentSize().width - 90, 300));
    }
}

void EpisodeSelector::setHqData(const HQDataObjectRef& hqData)
{
    _hqData = hqData;
    setupEpisodeBars();
    ImageDownloaderRef downloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    downloader->downloadImage(this, _hqData->getGroupLogo());
    _divider->setBackGroundColor(Style::Color::macaroniAndCheese);
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
                bar->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
                bar->setContentItemData(item);
                bar->setEpisodeNumber(i++);
                bar->setEpisodeTagColour(Style::Color::macaroniAndCheese);
                bar->setContentSize(Size(getContentSize().width - 90, 300));
                _episodeListView->pushBackCustomItem(bar);
                _episodeBars.pushBack(bar);
            }
        }
    }
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
