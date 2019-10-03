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
#include "RecentlyPlayedManager.h"
#include "HQDataProvider.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

GameHQ::~GameHQ()
{
    if(_recentlyPlayedTitle)
    {
        _recentlyPlayedTitle->release();
    }
    if(_recentlyPlayedLayout)
    {
        _recentlyPlayedLayout->release();
    }
}

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
    MutableHQCarouselObjectRef recentlyPlayedData = MutableHQCarouselObject::create();
    recentlyPlayedData->addContentItemsToCarousel(RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(ConfigStorage::kGameHQName));
    recentlyPlayedData->setTitle(_("Recently played"));
    
    _recentlyPlayedLayout->setContentItemData(recentlyPlayedData);
    
    if(recentlyPlayedData->getContentItems().size() != 0 && !_recentlyPlayedLayout->getParent())
    {
        bool isFirstItem = _contentListView->getItem(0) != _featuredLayout;
        _contentListView->insertCustomItem(_recentlyPlayedTitle, isFirstItem ? 0 : 1);
        _contentListView->insertCustomItem(_recentlyPlayedLayout, isFirstItem ? 1 : 2);
    }
    
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
        _featuredLayout->enableFixedHeight(false);
        _featuredLayout->enableWideLayout(_contentListView->getContentSize().width > 1710);
        _featuredLayout->setContentSize(Size(_contentListView->getContentSize().width - (kListViewSidePadding / 4.0f), 0));
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
        _featuredLayout->setSizePercent(Vec2(0.975f,0.95f));
        _featuredLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _featuredLayout->setPositionType(PositionType::PERCENT);
        _featuredLayout->setPositionPercent(Vec2::ANCHOR_MIDDLE_LEFT);
        _featuredLayout->enableWideLayout(false);
        _featuredLayout->enableFixedHeight(true);
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
        dropdown->setContentSize(Size(contentListViewWidth - kListViewSidePadding, dropdown->getContentSize().height));
    }
    _contentListView->forceDoLayout();
    
}

void GameHQ::createFeaturedTiles()
{
    _featuredLayout = FeaturedGamesHolder::create();
    _featuredLayout->setContentItemData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGameHQName)->getHqCarousels().at(0));
    _featuredLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            _contentSelectedCallback(content, elementIndex, 0);
        }
    });
    _staticContentLayout->addChild(_featuredLayout);
    
}

void GameHQ::createRecentlyPlayedTiles()
{
    if(_recentlyPlayedTitle)
    {
        _recentlyPlayedTitle->release();
    }
    if(_recentlyPlayedLayout)
    {
        _recentlyPlayedLayout->release();
    }
    _recentlyPlayedTitle = DynamicText::create(_("Recently played"), Style::Font::PoppinsBold(), 75);
    _recentlyPlayedTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _recentlyPlayedTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _recentlyPlayedTitle->setOverflow(Label::Overflow::SHRINK);
    _recentlyPlayedTitle->setTextAreaSize(Size((_contentListView->getSizePercent().x * getContentSize().width) - kListViewSidePadding, _recentlyPlayedTitle->getContentSize().height));
    _recentlyPlayedTitle->setTextColor(Color4B::WHITE);
    _recentlyPlayedTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _recentlyPlayedTitle->retain();
    
    _recentlyPlayedLayout = CircleContentHolder::create();
    _recentlyPlayedLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _recentlyPlayedLayout->setMaxRows(1);
    _recentlyPlayedLayout->setPlaceholder("res/hqscene/game_icon.png");
    _recentlyPlayedLayout->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
    _recentlyPlayedLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            _contentSelectedCallback(content, elementIndex, -1);
        }
    });
    _recentlyPlayedLayout->retain();
}

void GameHQ::createDropdowns()
{
    const auto& carouselData = HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGameHQName)->getHqCarousels();
    for(int i = 1; i < carouselData.size(); i++)
    {
        auto carousel = carouselData.at(i);
        DropdownContentHolder* dropdown = DropdownContentHolder::create();
        dropdown->setTilePlaceholder(CONTENT_PLACEHOLDER_GAME_1X1);
        dropdown->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
        dropdown->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
        dropdown->setContentItemData(carousel);
        dropdown->setFrameColour(Color3B(carousel->getColour()));
        dropdown->setPatternColour(Color3B(carousel->getColour()));
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

NS_AZOOMEE_END
