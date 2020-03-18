//
//  VideoHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 23/08/2019.
//

#include "VideoHQ.h"
#include <TinizineCommon/Data/HQDataObject/HQDataObjectManager.h>
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/Data/HQDataObject/ContentItemManager.h>
#include <TinizineCommon/ContentDataManagers/RecentlyPlayedManager.h>
#include "HQConstants.h"
#include "Style.h"
#include "ContentOpener.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool VideoHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
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
    
    createFeaturedTiles();
    createRecentlyPlayedTiles();
    createDropdowns();
    createEpisodePlayer();
    
    return true;
}

void VideoHQ::onEnter()
{
    const auto& allRecentPlayed = RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(HQConsts::kVideoHQName);
    // Filter the recently played by group so videos can display as a series, but open the episode
    const auto& recentPlayedData = ContentItemManager::getInstance()->filterContentItemsByUniqueGroup(allRecentPlayed);
    
    _recentPlayedContent = recentPlayedData.first;
    
    MutableHQCarouselObjectRef recentlyPlayedData = MutableHQCarouselObject::create();
    recentlyPlayedData->addContentItemsToCarousel(recentPlayedData.second);
    recentlyPlayedData->setTitle(_("Recently Watched"));
    
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

    const Size& contentSize = getContentSize();
    
    _episodeSelector->stopAllActions();
    _episodeSelector->toggleBottomGradient(!_isPortrait);
    _episodeSelector->enableCloseButton(_isPortrait);
    if(_isPortrait)
    {
        _contentListView->setSizePercent(Vec2(1.0f, 1.0f));
        _staticContentLayout->setSizePercent(Vec2(0.0f, 1.0f));
        
        _episodeSelector->setSizePercent(Vec2(1.0f,1.0f));
        if(_episodePlayerMoving)
        {
            if(_episodePlayerOpen)
            {
                _episodeSelector->setPosition(Vec2(0, -_episodeSelector->getContentSize().height));
                _episodePlayerOpen = false;
                _episodePlayerMoving = false;
            }
            else
            {
                _episodeSelector->setPosition(Vec2(0,0));
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            }
        }
        else
        {
            if(_episodePlayerOpen)
            {
                _episodeSelector->setPosition(Vec2(0,0));
                _episodePlayerOpen = true;
                _episodePlayerMoving = false;
            }
            else
            {
                _episodeSelector->setPosition(Vec2(0, -_episodeSelector->getContentSize().height));
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
        
        _episodeSelector->setPosition(Vec2(0,contentSize.height * 0.05f));
        _episodeSelector->setSizePercent(Vec2(0.475,0.95f));
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
    const auto& hqData = HQDataObjectManager::getInstance()->getHQDataObjectForKey(HQConsts::kVideoHQName);
    HQCarouselObjectRef carouselData = nullptr;
    if(hqData)
    {
        if(hqData->getHqCarousels().size() > 0)
        {
            carouselData = hqData->getHqCarousels().at(0);
        }
    }
    _featuredLayout = FeaturedVideosHolder::create();
    _featuredLayout->setContentItemData(carouselData);
    _featuredLayout->setSizeType(SizeType::ABSOLUTE);
    _featuredLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _featuredLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            const std::string& location = StringUtils::format("%s%d", HQConsts::kContentLocFeature, elementIndex);
            _contentSelectedCallback(content, elementIndex, 0, location);
        }
    });
    _contentListView->pushBackCustomItem(_featuredLayout);
    
}

void VideoHQ::createRecentlyPlayedTiles()
{
    
    _recentlyPlayedTitle = DynamicText::create(_("Recently Watched"), Style::Font::PoppinsBold(), 75);
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
    _recentlyPlayedLayout->setPlaceholder("res/hqscene/video_icon.png");
    _recentlyPlayedLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _recentlyPlayedLayout->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
    _recentlyPlayedLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            // Open the actual content by checking _recentPlayedContent
            // This is because content might be a group if it's a video episode
            HQContentItemObjectRef contentToOpen = elementIndex < _recentPlayedContent.size() ? _recentPlayedContent[elementIndex] : content;
            _contentSelectedCallback(contentToOpen, elementIndex, -1, HQConsts::kContentLocRecentPlayed);
        }
    });
    _contentListView->pushBackCustomItem(_recentlyPlayedLayout);
}

void VideoHQ::createDropdowns()
{
    const auto& hqData = HQDataObjectManager::getInstance()->getHQDataObjectForKey(HQConsts::kVideoHQName);
    if(!hqData)
    {
        return;
    }
    const auto& carouselData = hqData->getHqCarousels();
    for(int i = 1; i < carouselData.size(); i++)
    {
        auto carousel = carouselData.at(i);
        DropdownContentHolder* dropdown = DropdownContentHolder::create();
        dropdown->setTilePlaceholder(CONTENT_PLACEHOLDER_VIDEO_1X1);
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

void VideoHQ::createEpisodePlayer()
{
    HQContentItemObjectRef firstItem = nullptr;
    const auto& featuredData = _featuredLayout->getContentItemData();
    if(featuredData && featuredData->getContentItems().size() > 0)
    {
        firstItem = featuredData->getContentItems().front();
    }
    
    _episodeSelector = EpisodeSelector::create();
    _episodeSelector->setSizeType(SizeType::PERCENT);
    _episodeSelector->setSizePercent(Vec2(0.975f,1.0f));
    _episodeSelector->setEpisodeBarHeight(_isPortrait ? EpisodeSelector::kEpisodeBarHeightPortrait : EpisodeSelector::kEpisodeBarHeightLandscape);
    _episodeSelector->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _episodeSelector->setPosition(Vec2(0,0));
    if(firstItem)
    {
        ContentOpener::getInstance()->getDataForGroupHQ(firstItem->getUri(), firstItem->getCarouselColour());
        _episodeSelector->setLineAndTextColour(Color3B(firstItem->getCarouselColour()));
        _episodeSelector->setHqData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(HQDataObject::kGroupHQName));
    }
    _episodeSelector->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_episodeSelectorContentSelectedCallback)
        {
            _episodeSelectorContentSelectedCallback(content, elementIndex, 0, HQConsts::kContentLocEpisodeSelector);
        }
    });
    _episodeSelector->setTouchEnabled(true);
    _episodeSelector->setCloseButtonCallback([this](){
        if(_episodePlayerOpen)
        {
            if(!_episodePlayerMoving)
            {
                _episodePlayerMoving = true;
                _episodeSelector->runAction(Sequence::createWithTwoActions(MoveTo::create(0.5f, Vec2(0,-_episodeSelector->getContentSize().height)), CallFunc::create([this](){
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
                _episodeSelector->runAction(Sequence::createWithTwoActions(MoveTo::create(0.5f, Vec2(0,0)), CallFunc::create([this](){
                    _episodePlayerOpen = true;
                    _episodePlayerMoving = false;
                })));
            }
        }
    });
    addChild(_episodeSelector, 1);
    
    EventListenerCustom* eventListener = EventListenerCustom::create(ContentOpener::kGroupRefreshEvent, [this](EventCustom* event){
        _episodeSelector->setHqData(HQDataObjectManager::getInstance()->getHQDataObjectForKey(HQDataObject::kGroupHQName));
        Color4B colour = *static_cast<Color4B*>(event->getUserData());
        _episodeSelector->setLineAndTextColour(Color3B(colour));
        if(_isPortrait && !_episodePlayerOpen && !_episodePlayerMoving)
        {
            _episodePlayerMoving = true;
            _episodeSelector->setVisible(true);
            _episodeSelector->runAction(Sequence::createWithTwoActions(MoveTo::create(0.5f, Vec2(0,0)), CallFunc::create([this](){
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

int VideoHQ::getOpenDropdown() const
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

void VideoHQ::setDropdownOpen(int dropdownIndex)
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

bool VideoHQ::isEpisodePlayerOpen() const
{
    return _episodePlayerOpen;
}

NS_AZ_END
