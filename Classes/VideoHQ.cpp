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
#include "HQDataProvider.h"

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

    _episodeSelector->stopAllActions();
    _episodeSelector->setTouchEnabled(_isPortrait);
    if(_isPortrait)
    {
        _contentListView->setSizePercent(Vec2(1.0f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
        
        _episodeSelector->setSizePercent(Vec2(0.975,1.0f));
        if(_episodePlayerMoving)
        {
            if(_episodePlayerOpen)
            {
                _episodeSelector->setPosition(Vec2(getContentSize().width / 2.0f, -_episodeSelector->getContentSize().height + kEpisodePlayerTabHeight));
                _episodePlayerOpen = false;
                _episodePlayerMoving = false;
            }
            else
            {
                _episodeSelector->setPosition(Vec2(getContentSize().width / 2.0f,0));
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            }
        }
        else
        {
            if(_episodePlayerOpen)
            {
                _episodeSelector->setPosition(Vec2(getContentSize().width / 2.0f,0));
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            }
            else
            {
                _episodeSelector->setPosition(Vec2(getContentSize().width / 2.0f, -_episodeSelector->getContentSize().height + kEpisodePlayerTabHeight));
                _episodePlayerOpen = false;
                _episodePlayerMoving = false;
            }
        }
        _episodeSelector->setVisible(_episodePlayerOpen);
    }
    else
    {
        _contentListView->setSizePercent(Vec2(0.5f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.5f, 1.0f));
        
        _episodeSelector->setPosition(Vec2(getContentSize().width / 4.0f,getContentSize().height * 0.05f));
        _episodeSelector->setSizePercent(Vec2(0.475,0.95f));
        _episodeSelector->setVisible(true);
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
        if(_contentSelectedCallback)
        {
            _contentSelectedCallback(content, elementIndex, 0);
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
        if(_contentSelectedCallback)
        {
            _contentSelectedCallback(content, elementIndex, -1);
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
            if(_contentSelectedCallback)
            {
                _contentSelectedCallback(content, elementIndex, i);
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
    HQDataProvider::getInstance()->getDataForGroupHQ(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kVideoHQName)->getHqCarousels().at(0)->getContentItems().at(0)->getUri());
    _episodeSelector = EpisodeSelector::create();
    _episodeSelector->setSizeType(SizeType::PERCENT);
    _episodeSelector->setSizePercent(Vec2(0.975f,1.0f));
    _episodeSelector->setHqData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName));
    _episodeSelector->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _episodeSelector->setPosition(Vec2(getContentSize().width / 2.0f,0));
    _episodeSelector->setTouchEnabled(true);
    _episodeSelector->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_episodePlayerOpen)
            {
                if(!_episodePlayerMoving)
                {
                    _episodePlayerMoving = true;
                    _episodeSelector->runAction(Sequence::createWithTwoActions(MoveTo::create(1.0f, Vec2(getContentSize().width / 2.0f,-_episodeSelector->getContentSize().height + kEpisodePlayerTabHeight)), CallFunc::create([this](){
                        _episodePlayerOpen = false;
                        _episodePlayerMoving = false;
                        _episodeSelector->setVisible(false);
                    })));
                }
            }
            else
            {
                if(!_episodePlayerMoving)
                {
                    _episodePlayerMoving = true;
                    _episodeSelector->setVisible(true);
                    _episodeSelector->runAction(Sequence::createWithTwoActions(MoveTo::create(1.0f, Vec2(getContentSize().width / 2.0f,0)), CallFunc::create([this](){
                        _episodePlayerOpen = true;
                        _episodePlayerMoving = false;
                    })));
                }
            }
        }
    });
    addChild(_episodeSelector, 1);
    
    EventListenerCustom* eventListener = EventListenerCustom::create("groupRefresh", [this](EventCustom* event){
        _episodeSelector->setHqData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName));
        if(_isPortrait && !_episodePlayerOpen && !_episodePlayerMoving)
        {
            _episodePlayerMoving = true;
            _episodeSelector->setVisible(true);
            _episodeSelector->runAction(Sequence::createWithTwoActions(MoveTo::create(1.0f, Vec2(getContentSize().width / 2.0f,0)), CallFunc::create([this](){
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            })));
        }
        else
        {
            _episodePlayerOpen = true;
        }
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    
}

NS_AZOOMEE_END
