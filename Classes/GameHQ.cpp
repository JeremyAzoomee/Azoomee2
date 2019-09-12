//
//  GameHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/08/2019.
//

#include "GameHQ.h"
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectManager.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool GameHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    createFeaturedTiles();
    createRecentlyPlayedTiles();
    createDropdowns();
    
    return true;
}

void GameHQ::onEnter()
{
    Super::onEnter();
}

void GameHQ::onExit()
{
    Super::onExit();
}

void GameHQ::onSizeChanged()
{
    Super::onSizeChanged();

    if(_isPortrait)
    {
        _contentListView->setSizePercent(Vec2(1.0f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
        
        _featuredLayout->setSizeType(SizeType::ABSOLUTE);
        _featuredLayout->setPositionType(PositionType::ABSOLUTE);
        _featuredLayout->setContentSize(Size(_contentListView->getContentSize().width - (kListViewSidePadding / 4.0f), kFeaturedContentHeightPortrait));
        if(_featuredLayout->getParent() == _staticContentLayout)
        {
            _featuredLayout->retain();
            _featuredLayout->removeFromParent();
            _contentListView->insertCustomItem(_featuredLayout, 0);
            _featuredLayout->release();
        }
    }
    else
    {
        _contentListView->setSizePercent(Vec2(0.5f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.5f, 1.0f));
        
        _featuredLayout->setSizeType(SizeType::PERCENT);
        _featuredLayout->setSizePercent(Vec2(0.95f,0.95f));
        _featuredLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _featuredLayout->setPositionType(PositionType::PERCENT);
        _featuredLayout->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        if(_contentListView->getItem(0) == _featuredLayout)
        {
            _featuredLayout->retain();
            _contentListView->removeItem(0);
            _staticContentLayout->addChild(_featuredLayout);
            _featuredLayout->release();
        }
    }

    const float contentListViewWidth = _contentListView->getSizePercent().x * getContentSize().width;

    _recentlyPlayedTitle->setTextAreaSize(Size(contentListViewWidth - kListViewSidePadding, _recentlyPlayedTitle->getContentSize().height));
    _recentlyPlayedLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _recentlyPlayedLayout->setContentSize(Size(contentListViewWidth, 0));
    
    for(auto dropdown : _dropdownLayouts)
    {
        dropdown->setContentSize(Size(contentListViewWidth - 64, dropdown->getContentSize().height));
    }
    _contentListView->forceDoLayout();
    
}

void GameHQ::createFeaturedTiles()
{
    _featuredLayout = FeaturedGamesHolder::create();
    _featuredLayout->setContentItemData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGameHQName)->getHqCarousels().at(0));
    _staticContentLayout->addChild(_featuredLayout);
    
}

void GameHQ::createRecentlyPlayedTiles()
{
    _recentlyPlayedTitle = DynamicText::create(_("Recently played"), Style::Font::PoppinsBold(), 80);
    _recentlyPlayedTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _recentlyPlayedTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _recentlyPlayedTitle->setOverflow(Label::Overflow::SHRINK);
    _recentlyPlayedTitle->setTextAreaSize(Size((_contentListView->getSizePercent().x * getContentSize().width) - kListViewSidePadding, _recentlyPlayedTitle->getContentSize().height));
    _recentlyPlayedTitle->setTextColor(Color4B::WHITE);
    _recentlyPlayedTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentListView->pushBackCustomItem(_recentlyPlayedTitle);
    
    _recentlyPlayedLayout = CircleContentHolder::create();
    _recentlyPlayedLayout->setContentItemData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGameHQName)->getHqCarousels().at(3)); //TODO: replace with data from RecentlyPlayedSingleton
    _recentlyPlayedLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _recentlyPlayedLayout->setMaxRows(1);
    _recentlyPlayedLayout->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
    _contentListView->pushBackCustomItem(_recentlyPlayedLayout);
}

void GameHQ::createDropdowns()
{
    const auto& carouselData = HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGameHQName)->getHqCarousels();
    for(int i = 1; i < carouselData.size(); i++)
    {
        auto carousel = carouselData.at(i);
        DropdownContentHolder* dropdown = DropdownContentHolder::create();
        dropdown->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
        dropdown->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
        dropdown->setContentItemData(carousel);
        dropdown->setFrameColour(Style::Color::azure);
        dropdown->setPatternColour(Style::Color::azure);
        dropdown->setOnResizeCallback([this, dropdown](){
            _contentListView->forceDoLayout();
            float minY = _contentListView->getContentSize().height - _contentListView->getInnerContainerSize().height;
            float h = -minY;
            if(_resizingPositionLock.y < minY)
            {
                _contentListView->setInnerContainerPosition(Vec2(_resizingPositionLock.x, minY));
            }
            else if(_resizingPositionLock.y > minY + h)
            {
                _contentListView->setInnerContainerPosition(Vec2(_resizingPositionLock.x,  minY + h));
            }
            else
            {
                _contentListView->setInnerContainerPosition(_resizingPositionLock);
            }
        });
        dropdown->setTouchEnabled(true);
        dropdown->addTouchEventListener([dropdown, this](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                for(auto dd : _dropdownLayouts)
                {
                    if(dd != dropdown && dd->isOpen())
                    {
                        dd->toggleOpened(false);
                    }
                }
                dropdown->toggleOpened(!dropdown->isOpen());
                _resizingPositionLock = _contentListView->getInnerContainerPosition();
            }
        });
        _contentListView->pushBackCustomItem(dropdown);
        _dropdownLayouts.pushBack(dropdown);
    }
}

NS_AZOOMEE_END
