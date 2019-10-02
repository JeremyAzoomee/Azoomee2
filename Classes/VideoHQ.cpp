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
    
    if(recentlyPlayedData->getContentItems().size() != 0 && !_recentlyPlayedLayout->getParent())
    {
        _contentListView->insertCustomItem(_recentlyPlayedTitle, 1);
        _contentListView->insertCustomItem(_recentlyPlayedLayout, 2);
        _recentlyPlayedLayout->release();
        _recentlyPlayedTitle->release();
    }
    
    Super::onEnter();
}

void VideoHQ::onExit()
{
    Super::onExit();
}

void VideoHQ::onSizeChanged()
{
    Super::onSizeChanged();

    const Size& contentSize = getContentSize();
    
    _episodeSelector->stopAllActions();
    _episodeSelector->toggleBottomGradient(!_isPortrait);
    _episodeSelector->enableCloseButton(_isPortrait);
    if(_isPortrait)
    {
        const float contentWidthMidpoint =  contentSize.width * 0.5f;
        _contentListView->setSizePercent(Vec2(1.0f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
        
        _episodeSelector->setSizePercent(Vec2(0.975,1.0f));
        if(_episodePlayerMoving)
        {
            if(_episodePlayerOpen)
            {
                _episodeSelector->setPosition(Vec2(contentWidthMidpoint, -_episodeSelector->getContentSize().height));
                _episodePlayerOpen = false;
                _episodePlayerMoving = false;
            }
            else
            {
                _episodeSelector->setPosition(Vec2(contentWidthMidpoint,0));
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            }
        }
        else
        {
            if(_episodePlayerOpen)
            {
                _episodeSelector->setPosition(Vec2(contentWidthMidpoint,0));
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            }
            else
            {
                _episodeSelector->setPosition(Vec2(contentWidthMidpoint, -_episodeSelector->getContentSize().height));
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
        
        _episodeSelector->setPosition(Vec2(contentSize.width / 4.0f,contentSize.height * 0.05f));
        _episodeSelector->setSizePercent(Vec2(0.475,0.95f));
        _episodeSelector->setVisible(true);
        _episodePlayerMoving = false;
    }
    
    const float contentListViewWidth = _contentListView->getSizePercent().x * contentSize.width;
    
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

void VideoHQ::setEpisodeSelectorContentSelectedCallback(const ContentSelectedCallback& callback)
{
    _episodeSelectorContentSelectedCallback = callback;
}

void VideoHQ::createFeaturedTiles()
{
    _featuredLayout = FeaturedVideosHolder::create();
    _featuredLayout->setContentItemData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kVideoHQName)->getHqCarousels().at(0));
    _featuredLayout->setSizeType(SizeType::ABSOLUTE);
    //_featuredLayout->setContentSize(Size(_contentListView->getContentSize().width, kFeaturedContentHeightPortrait));
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
 
    _recentlyPlayedTitle = DynamicText::create(_("Recently watched"), Style::Font::PoppinsBold(), 75);
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
    _recentlyPlayedLayout->setPlaceholder("res/hqscene/video_icon.png");
    _recentlyPlayedLayout->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
    _recentlyPlayedLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            _contentSelectedCallback(content, elementIndex, -1);
        }
    });
    _recentlyPlayedLayout->retain();
}

void VideoHQ::createDropdowns()
{
    const auto& carouselData = HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kVideoHQName)->getHqCarousels();
    for(int i = 1; i < carouselData.size(); i++)
    {
        auto carousel = carouselData.at(i);
        DropdownContentHolder* dropdown = DropdownContentHolder::create();
        dropdown->setTilePlaceholder("res/contentPlaceholders/Video1X1.png");
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

void VideoHQ::createEpisodePlayer()
{
    HQDataProvider::getInstance()->getDataForGroupHQ(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kVideoHQName)->getHqCarousels().at(0)->getContentItems().at(0)->getUri());
    _episodeSelector = EpisodeSelector::create();
    _episodeSelector->setSizeType(SizeType::PERCENT);
    _episodeSelector->setSizePercent(Vec2(0.975f,1.0f));
    _episodeSelector->setHqData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName));
    _episodeSelector->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _episodeSelector->setPosition(Vec2(getContentSize().width / 2.0f,0));
    _episodeSelector->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_episodeSelectorContentSelectedCallback)
        {
            _episodeSelectorContentSelectedCallback(content, elementIndex, 0);
        }
    });
    _episodeSelector->setTouchEnabled(true);
    _episodeSelector->setCloseButtonCallback([this](){
        if(_episodePlayerOpen)
        {
            if(!_episodePlayerMoving)
            {
                _episodePlayerMoving = true;
                _episodeSelector->runAction(Sequence::createWithTwoActions(MoveTo::create(0.5f, Vec2(getContentSize().width / 2.0f,-_episodeSelector->getContentSize().height)), CallFunc::create([this](){
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
                _episodeSelector->runAction(Sequence::createWithTwoActions(MoveTo::create(0.5f, Vec2(getContentSize().width / 2.0f,0)), CallFunc::create([this](){
                    _episodePlayerOpen = true;
                    _episodePlayerMoving = false;
                })));
            }
        }
    });
    addChild(_episodeSelector, 1);
    
    EventListenerCustom* eventListener = EventListenerCustom::create(HQDataProvider::kGroupRefreshEvent, [this](EventCustom* event){
        _episodeSelector->setHqData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName));
        if(_isPortrait && !_episodePlayerOpen && !_episodePlayerMoving)
        {
            _episodePlayerMoving = true;
            _episodeSelector->setVisible(true);
            _episodeSelector->runAction(Sequence::createWithTwoActions(MoveTo::create(0.5f, Vec2(getContentSize().width / 2.0f,0)), CallFunc::create([this](){
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
