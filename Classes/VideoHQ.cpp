//
//  VideoHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 23/08/2019.
//

#include "VideoHQ.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectManager.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include "RecentlyPlayedManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float VideoHQ::kEpisodePlayerTabHeight = 100.0f;

bool VideoHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    createFeaturedTiles();
    createRecentlyPlayedTiles();
    createDropdowns();
    createEpisodePlayer();
    
    return true;
}

void VideoHQ::onEnter()
{
    MutableHQCarouselObjectRef recentlyPlayedData = MutableHQCarouselObject::create();
    recentlyPlayedData->addContentItemsToCarousel(RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(ConfigStorage::kVideoHQName));
    recentlyPlayedData->setTitle(_("Recently watched"));
    
    _recentlyPlayedLayout->setContentItemData(recentlyPlayedData);
    
    Super::onEnter();
}

void VideoHQ::onExit()
{
    Super::onExit();
}

void VideoHQ::onSizeChanged()
{
    Super::onSizeChanged();

    _episodePlayer->stopAllActions();
    _episodePlayer->setTouchEnabled(_isPortrait);
    if(_isPortrait)
    {
        _contentListView->setSizePercent(Vec2(1.0f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
        
        _episodePlayer->setSizePercent(Vec2(1.0,1.0f));
        if(_episodePlayerMoving)
        {
            if(_episodePlayerOpen)
            {
                _episodePlayer->setPosition(Vec2(0, -_episodePlayer->getContentSize().height + kEpisodePlayerTabHeight));
                _episodePlayerOpen = false;
                _episodePlayerMoving = false;
            }
            else
            {
                _episodePlayer->setPosition(Vec2(0,0));
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            }
        }
        else
        {
            if(_episodePlayerOpen)
            {
                _episodePlayer->setPosition(Vec2(0,0));
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            }
            else
            {
                _episodePlayer->setPosition(Vec2(0, -_episodePlayer->getContentSize().height + kEpisodePlayerTabHeight));
                _episodePlayerOpen = false;
                _episodePlayerMoving = false;
            }
        }
    }
    else
    {
        _contentListView->setSizePercent(Vec2(0.5f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.5f, 1.0f));
        
        _episodePlayer->setPosition(Vec2(0,0));
        _episodePlayer->setSizePercent(Vec2(0.5,1.0f));
        _episodePlayerOpen = false;
        _episodePlayerMoving = false;
    }
    
    const float contentListViewWidth = _contentListView->getSizePercent().x * getContentSize().width;
    
    _recentlyPlayedTitle->setTextAreaSize(Size(contentListViewWidth - kListViewSidePadding, _recentlyPlayedTitle->getContentSize().height));
    _recentlyPlayedLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _recentlyPlayedLayout->setContentSize(Size(contentListViewWidth, 0));
    _featuredLayout->setContentSize(Size(contentListViewWidth, _isPortrait ? kFeaturedContentHeightPortrait : kFeaturedContentHeightLandscape));
    
    for(auto dropdown : _dropdownLayouts)
    {
        dropdown->setContentSize(Size(contentListViewWidth - kListViewSidePadding, dropdown->getContentSize().height));
    }
    
    _contentListView->forceDoLayout();
}

void VideoHQ::createFeaturedTiles()
{
    _featuredLayout = FeaturedVideosHolder::create();
    _featuredLayout->setContentItemData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kVideoHQName)->getHqCarousels().at(0));
    _featuredLayout->setSizeType(SizeType::ABSOLUTE);
    _featuredLayout->setContentSize(Size(_contentListView->getContentSize().width, kFeaturedContentHeightPortrait));
    _featuredLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _featuredLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSceletedCallback)
        {
            _contentSceletedCallback(content, elementIndex, 0);
        }
    });
    _contentListView->pushBackCustomItem(_featuredLayout);
    
}

void VideoHQ::createRecentlyPlayedTiles()
{
 
    _recentlyPlayedTitle = DynamicText::create(_("Recently watched"), Style::Font::PoppinsBold(), 80);
    _recentlyPlayedTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _recentlyPlayedTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _recentlyPlayedTitle->setOverflow(Label::Overflow::SHRINK);
    _recentlyPlayedTitle->setTextAreaSize(Size((_contentListView->getSizePercent().x * getContentSize().width) - kListViewSidePadding, _recentlyPlayedTitle->getContentSize().height));
    _recentlyPlayedTitle->setTextColor(Color4B::WHITE);
    _recentlyPlayedTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentListView->pushBackCustomItem(_recentlyPlayedTitle);
    
    MutableHQCarouselObjectRef recentlyPlayedData = MutableHQCarouselObject::create();
    recentlyPlayedData->addContentItemsToCarousel(RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(ConfigStorage::kVideoHQName));
    recentlyPlayedData->setTitle(_("Recently watched"));
    
    _recentlyPlayedLayout = CircleContentHolder::create();
    _recentlyPlayedLayout->setContentItemData(recentlyPlayedData);
    _recentlyPlayedLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _recentlyPlayedLayout->setMaxRows(1);
    _recentlyPlayedLayout->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
    _recentlyPlayedLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSceletedCallback)
        {
            _contentSceletedCallback(content, elementIndex, -1);
        }
    });
    _contentListView->pushBackCustomItem(_recentlyPlayedLayout);
}

void VideoHQ::createDropdowns()
{
    const auto& carouselData = HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kVideoHQName)->getHqCarousels();
    for(int i = 1; i < carouselData.size(); i++)
    {
        auto carousel = carouselData.at(i);
        DropdownContentHolder* dropdown = DropdownContentHolder::create();
        dropdown->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
        dropdown->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
        dropdown->setContentItemData(carousel);
        dropdown->setFrameColour(Style::Color::azure);
        dropdown->setPatternColour(Style::Color::azure);
        dropdown->setContentSelectedCallback([this, i](HQContentItemObjectRef content, int elementIndex){
            if(_contentSceletedCallback)
            {
                _contentSceletedCallback(content, elementIndex, i);
            }
        });
        dropdown->setOnResizeCallback([this](){
            this->listviewDropdownResizeCallback();
        });
        dropdown->setTouchEnabled(true);
        dropdown->addTouchEventListener([dropdown, this](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                this->dropdownAutoOpenCloseLogic(dropdown, _dropdownLayouts);
            }
        });
        _contentListView->pushBackCustomItem(dropdown);
        _dropdownLayouts.pushBack(dropdown);
    }
}

void VideoHQ::createEpisodePlayer()
{
    _episodePlayer = ui::Layout::create();
    _episodePlayer->setSizeType(SizeType::PERCENT);
    _episodePlayer->setSizePercent(Vec2(1.0f,1.0f));
    _episodePlayer->setBackGroundColorType(BackGroundColorType::SOLID);
    _episodePlayer->setBackGroundColor(Color3B::GREEN);
    _episodePlayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _episodePlayer->setPosition(Vec2(0,0));
    _episodePlayer->setTouchEnabled(true);
    _episodePlayer->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_episodePlayerOpen)
            {
                if(!_episodePlayerMoving)
                {
                    _episodePlayerMoving = true;
                    _episodePlayer->runAction(Sequence::createWithTwoActions(MoveTo::create(1.0f, Vec2(0,-_episodePlayer->getContentSize().height + kEpisodePlayerTabHeight)), CallFunc::create([this](){
                        _episodePlayerOpen = false;
                        _episodePlayerMoving = false;
                    })));
                }
            }
            else
            {
                if(!_episodePlayerMoving)
                {
                    _episodePlayerMoving = true;
                    _episodePlayer->runAction(Sequence::createWithTwoActions(MoveTo::create(1.0f, Vec2(0,0)), CallFunc::create([this](){
                        _episodePlayerOpen = true;
                        _episodePlayerMoving = false;
                    })));
                }
            }
        }
    });
    addChild(_episodePlayer, 1);
    
}

NS_AZOOMEE_END
