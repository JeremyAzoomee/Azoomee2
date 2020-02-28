//
//  OomeeHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 29/08/2019.
//

#include "OomeeHQ.h"
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/Data/Parent/UserAccountManager.h>
#include <TinizineCommon/Data/Child/ChildManager.h>
#include <TinizineCommon/Utils/DirUtil.h>
#include <TinizineCommon/Data/HQDataObject/ContentItemManager.h>
#include <TinizineCommon/Analytics/AnalyticsSingleton.h>
#include <TinizineCommon/Data/AppConfig.h>
#include <TinizineCommon/ContentDataManagers/FavouritesManager.h>
#include "ArtAppDelegate.h"
#include "SceneManagerScene.h"
#include <TinizineCommon/ContentDataManagers/GameDataManager.h>
#include "Style.h"
#include "ShareInChatLayer.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool OomeeHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    createOomeeLayout();
    createScrollViewContent();
    createFavouritesLayout();
    createOfflineDropdown();
    
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

void OomeeHQ::onEnter()
{
    refreshOfflineList();
    refreshFavouritesList();
    refreshArtList();
    float scrollPerc = _contentListView->getScrolledPercentVertical();
    if(isnan(scrollPerc))
    {
        scrollPerc = 0.1f;
    }
    _contentListView->forceDoLayout();
    _contentListView->scrollToPercentVertical(scrollPerc, 0, false);
    
    _oomeeDisplay->setOomeeImgUrl(ChildManager::getInstance()->getLoggedInChild()->getAvatar());
    _oomeeDisplay->setBgPatternColour(Color3B(ChildManager::getInstance()->getLoggedInChild()->getAvatarColour()));
    Super::onEnter();
}

void OomeeHQ::onExit()
{
    Super::onExit();
}

void OomeeHQ::onSizeChanged()
{
    Super::onSizeChanged();
    
    if(_isPortrait)
    {
        _structureUIHolder->setLayoutType(Type::VERTICAL);
        _staticContentLayout->setSizePercent(Vec2(1.0f, 0.6f));
        _contentListView->setSizePercent(Vec2(1.0f, 0.4f));
        _oomeeDisplay->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _oomeeDisplay->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
        _oomeeDisplay->setSizePercent(Vec2(1.0f, 0.975f));
    }
    else
    {
        _structureUIHolder->stopAllActions();
        
        _structureUIHolder->setLayoutType(Type::HORIZONTAL);
        _staticContentLayout->setSizePercent(Vec2(0.5f, 1.0f));
        _contentListView->setSizePercent(Vec2(0.5f, 1.0f));
        
        _oomeeDisplay->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _oomeeDisplay->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
        _oomeeDisplay->setSizePercent(Vec2(0.975f, 0.975f));
    }
    
    const float contentListViewWidth = _contentListView->getSizePercent().x * getContentSize().width;
    
    _artStudioButton->setContentSize(Size(contentListViewWidth - kListViewSidePadding, _isPortrait ? HQConsts::OomeeHQSecondaryButtonHeightPortrait : HQConsts::OomeeHQSecondaryButtonHeightLandscape));
    _shopButton->setContentSize(Size(contentListViewWidth - kListViewSidePadding, _isPortrait ? HQConsts::OomeeHQSecondaryButtonHeightPortrait : HQConsts::OomeeHQSecondaryButtonHeightLandscape));
    _oomeeMakerButton->setContentSize(Size(contentListViewWidth - kListViewSidePadding, _isPortrait ? HQConsts::OomeeHQPrimaryButtonHeightPortrait : HQConsts::OomeeHQPrimaryButtonHeightLandscape));
    _artTileHolder->setContentSize(Size(contentListViewWidth - kListViewSidePadding, 0));
    
    _offlineDropdown->setContentSize(Size(contentListViewWidth - kListViewSidePadding, _offlineDropdown->getContentSize().height));
    _offlineDropdown->setUsingBigBg(_isPortrait);
    _favouritesTitle->setFontSize(_isPortrait ? 89 : 75);
    _favouritesTitle->setTextAreaSize(Size(contentListViewWidth - kListViewSidePadding, _favouritesTitle->getContentSize().height));
    _favouritesLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _favouritesLayout->setContentSize(Size(contentListViewWidth - kListViewSidePadding, 0));
    
    _topScrollGradient->setContentSize(Size(contentListViewWidth, _topScrollGradient->getContentSize().height));
    _topScrollGradient->setNormalizedPosition(Vec2(1.0f, _contentListView->getSizePercent().y));
    
    _contentListView->forceDoLayout();
    
}

void OomeeHQ::createOomeeLayout()
{
    _oomeeLayout = ui::Layout::create();
    _oomeeLayout->setSizeType(SizeType::PERCENT);
    _oomeeLayout->setSizePercent(Vec2(1.0f, 1.0f));
    _oomeeLayout->setPositionType(PositionType::PERCENT);
    _oomeeLayout->setPositionPercent(Vec2(0.5f, 0.5f));
    _oomeeLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _staticContentLayout->addChild(_oomeeLayout, 1);
    
    _oomeeDisplay = OomeeDisplay::create();
    _oomeeDisplay->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _oomeeDisplay->setSizePercent(Vec2(1.0f, 0.975f));
    _oomeeDisplay->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _oomeeDisplay->setKidCode(ChildManager::getInstance()->getLoggedInChild()->getInviteCode());
    _oomeeLayout->addChild(_oomeeDisplay);
}
void OomeeHQ::createScrollViewContent()
{
    _contentListView->setSwallowTouches(false);
    
    _oomeeMakerButton = OomeeMakerButton::create();
    _oomeeMakerButton->setSwallowTouches(false);
    _oomeeMakerButton->setContentSize(Size(_contentListView->getContentSize().width, HQConsts::OomeeHQPrimaryButtonHeightPortrait));
    _oomeeMakerButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _oomeeMakerButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            AnalyticsSingleton::getInstance()->contentItemSelectedEvent(HQConsts::kOomeeMakerURI);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OomeeMakerEntryPointScene));
        }
    });
    _contentListView->pushBackCustomItem(_oomeeMakerButton);
    
    _shopButton = OomeeStoreButton::create();
    _shopButton->setSwallowTouches(false);
    _shopButton->setContentSize(Size(_contentListView->getContentSize().width, HQConsts::OomeeHQSecondaryButtonHeightPortrait));
    _shopButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _shopButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            AnalyticsSingleton::getInstance()->openShopEvent();
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Shop));
        }
    });
    _contentListView->pushBackCustomItem(_shopButton);
    
    _artStudioButton = ArtStudioButton::create();
    _artStudioButton->setSwallowTouches(false);
    _artStudioButton->setContentSize(Size(_contentListView->getContentSize().width, HQConsts::OomeeHQSecondaryButtonHeightPortrait));
    _artStudioButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _artStudioButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            ArtAppDelegate::getInstance()->setFileName("");
            AnalyticsSingleton::getInstance()->contentItemSelectedEvent(HQConsts::OpenArtAnalyticsContentName);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ArtAppEntryPointScene));
        }
    });
    _contentListView->pushBackCustomItem(_artStudioButton);
    
    _artTileHolder = ArtTileHolder::create();
    _artTileHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _artTileHolder->setDeleteCallback([this](const std::string& filename){
        AnalyticsSingleton::getInstance()->genericButtonPressEvent(HQConsts::DeleteArtButtonAnalyticsName);
        FileUtils::getInstance()->removeFile(filename);
        refreshArtList();
        float scrollPerc = _contentListView->getScrolledPercentVertical();
        _contentListView->forceDoLayout();
        _contentListView->scrollToPercentVertical(scrollPerc, 0, false);
    });
    _artTileHolder->setEditCallback([this](const std::string& filename){
        ArtAppDelegate::getInstance()->setFileName(filename);
        _reloadArtFilename = filename;
        AnalyticsSingleton::getInstance()->contentItemSelectedEvent(HQConsts::OpenArtAnalyticsContentName);
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ArtAppEntryPointScene));
    });
    _artTileHolder->setOnResizeCallback([this](){
        this->listviewDropdownResizeCallback();
    });
    _artTileHolder->setToggleSelectedCallback([this](){
        _focusedDropdown = _artTileHolder;
        // Stop the scrollview
        _contentListView->stopOverallScroll();
        
        // Get the position of the focused item in viewport
        const Vec2& scrollPosition = _contentListView->getInnerContainerPosition();
        const Size& itemSize = _focusedDropdown->getContentSize();
        const Vec2 itemOrigin(_focusedDropdown->getLeftBoundary(), _focusedDropdown->getBottomBoundary());
        _resizingPositionInView = itemOrigin + scrollPosition + Vec2(itemSize.width * kFocusDropDownAnchor.x, itemSize.height * kFocusDropDownAnchor.y);
    });
    _contentListView->pushBackCustomItem(_artTileHolder);
    
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [this](Touch *touch, Event *event){
        if(_isPortrait && _contentListView->getBoundingBox().containsPoint(_contentListView->convertTouchToNodeSpace(touch)))
        {
            _contentListView->stopAllActions();
            _contentListView->stopOverallScroll();
            return true;
        }
        return false;
    };
    _touchListener->onTouchMoved = [this](Touch *touch, Event *event){
        float distY = touch->getDelta().y;
        if(distY > 0)
        {
            if(_contentListView->getSizePercent().y < 0.65f)
            {
                float movePercent = distY / this->getContentSize().height;
                float targetPos = MIN(0.65f,_contentListView->getSizePercent().y + movePercent);
                
                _contentListView->setSizePercent(Vec2(1.0f,targetPos));
                _staticContentLayout->setSizePercent(Vec2(1.0f, 1.0f - targetPos));
            }
        }
        else if(_contentListView->getScrolledPercentVertical() < 1.0f)
        {
            if(_contentListView->getSizePercent().y > 0.4f)
            {
                float movePercent = distY / this->getContentSize().height;
                float targetPos = MAX(0.4f,_contentListView->getSizePercent().y + movePercent);
                
                _contentListView->setSizePercent(Vec2(1.0f,targetPos));
                _staticContentLayout->setSizePercent(Vec2(1.0f, 1.0f - targetPos));
            }
        }
        _topScrollGradient->setNormalizedPosition(Vec2(1.0f, _contentListView->getSizePercent().y));
        _structureUIHolder->forceDoLayout();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, _contentListView);
}

void OomeeHQ::createFavouritesLayout()
{
    const float contentWidth = getContentSize().width;
    
    _favouritesTitle = DynamicText::create(_("Favourites"), Style::Font::PoppinsBold(), 75);
    _favouritesTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _favouritesTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _favouritesTitle->setOverflow(Label::Overflow::SHRINK);
    _favouritesTitle->setTextAreaSize(Size((_contentListView->getSizePercent().x * contentWidth) - kListViewSidePadding, _favouritesTitle->getContentSize().height));
    _favouritesTitle->setTextColor(Color4B::WHITE);
    _favouritesTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentListView->pushBackCustomItem(_favouritesTitle);
    
    _favouritesLayout = CircleContentHolder::create();
    _favouritesLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _favouritesLayout->setPlaceholder("res/OomeeHQ/favourite_icon.png");
    _favouritesLayout->setContentSize(Size(_contentListView->getSizePercent().x * contentWidth, 0));
    _favouritesLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _favouritesLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            // Open the actual content by checking _recentPlayedContent
            // This is because content might be a group if it's a video episode
            HQContentItemObjectRef contentToOpen = elementIndex < _favouritesContent.size() ? _favouritesContent[elementIndex] : content;
            _contentSelectedCallback(contentToOpen, elementIndex, -1, HQConsts::kContentLocFavourite);
        }
    });
    _contentListView->pushBackCustomItem(_favouritesLayout);
    
}

void OomeeHQ::createOfflineDropdown()
{
    _offlineDropdown = DropdownContentHolder::create();
    _offlineDropdown->setTilePlaceholder(CONTENT_PLACEHOLDER_GAME_1X1);
    _offlineDropdown->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _offlineDropdown->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
    _offlineDropdown->setFrameColour(Colours::Color_3B::azure);
    _offlineDropdown->setPatternColour(Colours::Color_3B::azure);
    _offlineDropdown->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            _contentSelectedCallback(content, elementIndex, -3, HQConsts::kContentLocOffline);
        }
    });
    _offlineDropdown->setOnResizeCallback([this](){
        this->listviewDropdownResizeCallback();
    });
    _offlineDropdown->setTouchEnabled(true);
    _offlineDropdown->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->dropdownAutoOpenCloseLogic(_offlineDropdown, {_offlineDropdown});
        }
    });
    _contentListView->pushBackCustomItem(_offlineDropdown);
}

void OomeeHQ::refreshOfflineList()
{
    MutableHQCarouselObjectRef carousel = MutableHQCarouselObject::create();
    carousel->setColour(Color4B(Colours::Color_3B::azure));
    carousel->addContentItemsToCarousel(GameDataManager::getInstance()->getOfflineGameList());
    carousel->setTitle(_("Offline"));
    
    _offlineDropdown->setContentItemData(carousel);
}

void OomeeHQ::refreshFavouritesList()
{
    const auto& allFavourites = FavouritesManager::getInstance()->getFavouriteContent();
    // Filter the recently played by group so videos can display as a series, but open the episode
    const auto& filteredFavourites = ContentItemManager::getInstance()->filterContentItemsByUniqueGroup(allFavourites);
    _favouritesContent = filteredFavourites.first;
    
    MutableHQCarouselObjectRef carousel = MutableHQCarouselObject::create();
    carousel->setColour(Color4B(Colours::Color_3B::azure));
    carousel->addContentItemsToCarousel(filteredFavourites.second);
    carousel->setTitle(_("Favourites"));
    _favouritesLayout->setMaxRows(_favouritesContent.size() > 0 ? -1 : 1);
    _favouritesLayout->setContentItemData(carousel);
}

void OomeeHQ::refreshArtList()
{
    const std::string& dirPath = DirUtil::getCachesPath() + AppConfig::kArtCacheFolder + ChildManager::getInstance()->getLoggedInChild()->getId();
    
    if(!FileUtils::getInstance()->isDirectoryExist(dirPath))
    {
        FileUtils::getInstance()->createDirectory(dirPath);
    }
    
    auto artImages = DirUtil::getImagesInDirectory(dirPath);
    std::reverse(artImages.begin(), artImages.end());
    std::vector<std::string> fullFilepaths;
    if(artImages.size() > 0)
    {
        for(auto filename : artImages)
        {
            const std::string& fullFilename = dirPath + "/" + filename;
            fullFilepaths.push_back(fullFilename);
            if(fullFilename == _reloadArtFilename)
            {
                Director::getInstance()->getTextureCache()->reloadTexture(fullFilename);
            }
        }
    }
    else
    {
        fullFilepaths.push_back(ArtTileHolder::kEmptyArtFilename);
    }
    
    if(fullFilepaths.size() % 2)
    {
        fullFilepaths.push_back(ArtTileHolder::kEmptyArtFilename);
    }
    
    _artTileHolder->setArtFilenames(fullFilepaths);
}

int OomeeHQ::getOpenDropdown() const
{
    return _offlineDropdown->isOpen();
}

void OomeeHQ::setDropdownOpen()
{
    _offlineDropdown->setOpen(true);
}

NS_AZ_END
