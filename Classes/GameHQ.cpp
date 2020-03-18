//
//  GameHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/08/2019.
//

#include "GameHQ.h"
#include <TinizineCommon/Data/HQDataObject/HQDataObjectManager.h>
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/ContentDataManagers/RecentlyPlayedManager.h>
#include <TinizineCommon/ContentDataManagers/GameDataManager.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool GameHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    createFeaturedTiles();
    createRecentlyPlayedTiles();
    createDropdowns();
    
    const Color3B& gradColour = Colours::Color_3B::darkIndigo;
    _topScrollGradient = LayerGradient::create(Color4B(gradColour), Color4B(gradColour.r, gradColour.g, gradColour.b, 0));
    _topScrollGradient->setIgnoreAnchorPointForPosition(false);
    _topScrollGradient->setContentSize(Size(_contentListView->getContentSize().width, 0));
    _topScrollGradient->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _topScrollGradient->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    addChild(_topScrollGradient, 1);
    
    _contentListView->addEventListener([this](Ref* pSender, ui::ScrollView::EventType eType){
        if(eType == ui::ScrollView::EventType::CONTAINER_MOVED)
        {
            const float minY = _contentListView->getContentSize().height - _contentListView->getInnerContainerSize().height;
            float scrollDist = MAX(_contentListView->getInnerContainerPosition().y - minY, 0);
            
            _topScrollGradient->setContentSize(Size(_contentListView->getContentSize().width, MIN(scrollDist,160)));
            
        }
    });
    
    return true;
}

void GameHQ::onEnter()
{
    MutableHQCarouselObjectRef recentlyPlayedData = MutableHQCarouselObject::create();
    recentlyPlayedData->addContentItemsToCarousel(RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(HQConsts::kGameHQName));
    recentlyPlayedData->setTitle(_("Recently played"));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _recentlyPlayedLayout->setContentItemData(GameDataManager::getInstance()->createFilteredCarouselForBundledGames(recentlyPlayedData));
#else
    _recentlyPlayedLayout->setContentItemData(recentlyPlayedData);
#endif
    
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
        _featuredLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _featuredLayout->setPositionType(PositionType::PERCENT);
        _featuredLayout->setPositionPercent(Vec2::ANCHOR_TOP_LEFT);
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

    _recentlyPlayedTitle->setFontSize(_isPortrait ? 89 : 75);
    _recentlyPlayedTitle->setTextAreaSize(Size(contentListViewWidth - kListViewSidePadding, _recentlyPlayedTitle->getContentSize().height));
    _recentlyPlayedLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _recentlyPlayedLayout->setContentSize(Size(contentListViewWidth - kListViewSidePadding, 0));
    
    _topScrollGradient->setContentSize(Size(contentListViewWidth, _topScrollGradient->getContentSize().height));
    
    for(auto dropdown : _dropdownLayouts)
    {
        dropdown->setContentSize(Size(contentListViewWidth - kListViewSidePadding, dropdown->getContentSize().height));
        dropdown->setUsingBigBg(_isPortrait);
    }
    _contentListView->forceDoLayout();
    
}

void GameHQ::createFeaturedTiles()
{
    const auto& hqData = HQDataObjectManager::getInstance()->getHQDataObjectForKey(HQConsts::kGameHQName);
    HQCarouselObjectRef carouselData = nullptr;
    if(hqData)
    {
        if(hqData->getHqCarousels().size() > 0)
        {
            carouselData = hqData->getHqCarousels().at(0);
        }
    }
    _featuredLayout = FeaturedGamesHolder::create();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _featuredLayout->setContentItemData(carouselData ? GameDataManager::getInstance()->createFilteredCarouselForBundledGames(carouselData) : nullptr);
#else
    _featuredLayout->setContentItemData(carouselData);
#endif
    _featuredLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            const std::string& location = StringUtils::format("%s%d", HQConsts::kContentLocFeature, elementIndex);
            _contentSelectedCallback(content, elementIndex, 0, location);
        }
    });
    _staticContentLayout->addChild(_featuredLayout);
    
}

void GameHQ::createRecentlyPlayedTiles()
{
    _recentlyPlayedTitle = DynamicText::create(_("Recently Played"), Style::Font::PoppinsBold(), 75);
    _recentlyPlayedTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _recentlyPlayedTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _recentlyPlayedTitle->setOverflow(Label::Overflow::SHRINK);
    _recentlyPlayedTitle->setTextAreaSize(Size((_contentListView->getSizePercent().x * getContentSize().width) - kListViewSidePadding, _recentlyPlayedTitle->getContentSize().height));
    _recentlyPlayedTitle->setTextColor(Color4B::WHITE);
    _recentlyPlayedTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentListView->pushBackCustomItem(_recentlyPlayedTitle);
    
    _recentlyPlayedLayout = CircleContentHolder::create();
    _recentlyPlayedLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _recentlyPlayedLayout->setMaxRows(1);
    _recentlyPlayedLayout->setPlaceholder("res/hqscene/game_icon.png");
    _recentlyPlayedLayout->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
    _recentlyPlayedLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _recentlyPlayedLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            _contentSelectedCallback(content, elementIndex, -1, HQConsts::kContentLocRecentPlayed);
        }
    });
    _contentListView->pushBackCustomItem(_recentlyPlayedLayout);
}

void GameHQ::createDropdowns()
{
    //const auto& carouselData = HQDataObjectManager::getInstance()->getHQDataObjectForKey(HQConsts::kGameHQName)->getHqCarousels();
    const auto& hqData = HQDataObjectManager::getInstance()->getHQDataObjectForKey(HQConsts::kGameHQName);
    if(!hqData)
    {
        return;
    }
    const auto& carouselData = hqData->getHqCarousels();
    for(int i = 1; i < carouselData.size(); i++)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        auto carousel = GameDataManager::getInstance()->createFilteredCarouselForBundledGames(carouselData.at(i));
#else
        auto carousel = carouselData.at(i);
#endif
        DropdownContentHolder* dropdown = DropdownContentHolder::create();
        dropdown->setTilePlaceholder(CONTENT_PLACEHOLDER_GAME_1X1);
        dropdown->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
        dropdown->setOpen(i == 1);
        dropdown->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
        dropdown->setFrameColour(Color3B(carousel->getColour()));
        dropdown->setPatternColour(Color3B(carousel->getColour()));
        dropdown->setContentItemData(carousel);
        dropdown->setContentSelectedCallback([this, i](HQContentItemObjectRef content, int elementIndex){
            if(_contentSelectedCallback)
            {
                _contentSelectedCallback(content, elementIndex, i, HQConsts::kContentLocCategory);
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

int GameHQ::getOpenDropdown() const
{
    for(int i = 0; i < _dropdownLayouts.size(); i++)
    {
        if(_dropdownLayouts.at(i)->isOpen())
        {
            return i;
        }
    }
    return -1;
}

void GameHQ::setDropdownOpen(int dropdownIndex)
{
    for(auto dropdown : _dropdownLayouts)
    {
        dropdown->setOpen(false);
    }
    if(dropdownIndex >= 0 && dropdownIndex < _dropdownLayouts.size())
    {
        _dropdownLayouts.at(dropdownIndex)->setOpen(true);
    }
}

NS_AZ_END
