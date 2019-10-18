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

VideoHQ::~VideoHQ()
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
    
    const Color3B& gradColour = Style::Color::darkIndigo;
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
        
        _episodeSelector->setSizePercent(Vec2(1.0f,1.0f));
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
        
        _episodeSelector->setPosition(Vec2(contentSize.width * 0.2375f,contentSize.height * 0.05f));
        _episodeSelector->setSizePercent(Vec2(0.4875,0.95f));
        _episodeSelector->setVisible(true);
        _episodePlayerMoving = false;
    }
    
    _episodeSelector->setEpisodeBarHeight(_isPortrait ? EpisodeSelector::kEpisodeBarHeightPortrait : EpisodeSelector::kEpisodeBarHeightLandscape);
    
    const float contentListViewWidth = _contentListView->getSizePercent().x * contentSize.width;
    
    _recentlyPlayedTitle->setFontSize(_isPortrait ? 89 : 75);
    _recentlyPlayedTitle->setTextAreaSize(Size(contentListViewWidth - kListViewSidePadding, _recentlyPlayedTitle->getContentSize().height));
    _recentlyPlayedLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _recentlyPlayedLayout->setContentSize(Size(contentListViewWidth - kListViewSidePadding, 0));
    _featuredLayout->setContentSize(Size(contentListViewWidth, _isPortrait ? kFeaturedContentHeightPortrait : kFeaturedContentHeightLandscape));
    
    _topScrollGradient->setContentSize(Size(contentListViewWidth, _topScrollGradient->getContentSize().height));
    
    for(auto dropdown : _dropdownLayouts)
    {
        dropdown->setContentSize(Size(contentListViewWidth - kListViewSidePadding, dropdown->getContentSize().height));
        dropdown->setUsingBigBg(_isPortrait);
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
    if(_recentlyPlayedTitle)
    {
        _recentlyPlayedTitle->release();
    }
    if(_recentlyPlayedLayout)
    {
        _recentlyPlayedLayout->release();
    }
    
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
    _recentlyPlayedLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
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
        dropdown->setTilePlaceholder(CONTENT_PLACEHOLDER_VIDEO_1X1);
        dropdown->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
        dropdown->setOpen(i == 1);
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
    HQContentItemObjectRef firstItem = _featuredLayout->getContentItemData()->getContentItems().front();
    HQDataProvider::getInstance()->getDataForGroupHQ(firstItem->getUri(), firstItem->getCarouselColour());
    _episodeSelector = EpisodeSelector::create();
    _episodeSelector->setSizeType(SizeType::PERCENT);
    _episodeSelector->setSizePercent(Vec2(0.975f,1.0f));
    _episodeSelector->setEpisodeBarHeight(_isPortrait ? EpisodeSelector::kEpisodeBarHeightPortrait : EpisodeSelector::kEpisodeBarHeightLandscape);
    _episodeSelector->setHqData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName));
    _episodeSelector->setLineAndTextColour(Color3B(firstItem->getCarouselColour()));
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
        Color4B colour = *static_cast<Color4B*>(event->getUserData());
        _episodeSelector->setLineAndTextColour(Color3B(colour));
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
