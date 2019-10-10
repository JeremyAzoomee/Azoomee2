//
//  OomeeHQ.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 29/08/2019.
//

#include "OomeeHQ.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Utils/DirUtil.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemManager.h>
#include "HQDataProvider.h"
#include "FavouritesManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    
    return true;
}

void OomeeHQ::onEnter()
{
    refreshOfflineList();
    refreshFavouritesList();
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
        _staticContentLayout->setSizePercent(Vec2(1.0f, 0.5f));
        _contentListView->setSizePercent(Vec2(1.0f, 0.5f));
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
    
    _artStudioLayout->setContentSize(Size(_contentListView->getContentSize().width, 1412));
    _shopButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _oomeeMakerButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    
    _offlineDropdown->setContentSize(Size(contentListViewWidth - kListViewSidePadding, _offlineDropdown->getContentSize().height));
    _favouritesTitle->setTextAreaSize(Size(contentListViewWidth - kListViewSidePadding, _favouritesTitle->getContentSize().height));
    _favouritesLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _favouritesLayout->setContentSize(Size(contentListViewWidth, 0));
    
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
    
    _oomeeMakerButton = ui::Layout::create();
    _oomeeMakerButton->setBackGroundColorType(BackGroundColorType::SOLID);
    _oomeeMakerButton->setBackGroundColor(Color3B::BLUE);
    _oomeeMakerButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _contentListView->pushBackCustomItem(_oomeeMakerButton);
    
    _shopButton = ui::Layout::create();
    _shopButton->setBackGroundColorType(BackGroundColorType::SOLID);
    _shopButton->setBackGroundColor(Color3B::YELLOW);
    _shopButton->setContentSize(Size(_contentListView->getContentSize().width, 574));
    _contentListView->pushBackCustomItem(_shopButton);
    
    _artStudioLayout = ui::Layout::create();
    _artStudioLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _artStudioLayout->setBackGroundColor(Color3B::GREEN);
    _artStudioLayout->setContentSize(Size(_contentListView->getContentSize().width, 1412));
    _contentListView->pushBackCustomItem(_artStudioLayout);
 
    
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
            if(_contentListView->getSizePercent().y < 0.75f)
            {
                float movePercent = distY / this->getContentSize().height;
                float targetPos = MIN(0.75f,_contentListView->getSizePercent().y + movePercent);
                
                _contentListView->setSizePercent(Vec2(1.0f,targetPos));
                _staticContentLayout->setSizePercent(Vec2(1.0f, 1.0f - targetPos));
            }
        }
        else if(_contentListView->getScrolledPercentVertical() < 1.0f)
        {
            if(_contentListView->getSizePercent().y > 0.5f)
            {
                float movePercent = distY / this->getContentSize().height;
                float targetPos = MAX(0.5f,_contentListView->getSizePercent().y + movePercent);
                
                _contentListView->setSizePercent(Vec2(1.0f,targetPos));
                _staticContentLayout->setSizePercent(Vec2(1.0f, 1.0f - targetPos));
            }
        }
        _structureUIHolder->forceDoLayout();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, _contentListView);
}

void OomeeHQ::createFavouritesLayout()
{
    _favouritesTitle = DynamicText::create(_("Favourites"), Style::Font::PoppinsBold(), 75);
    _favouritesTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _favouritesTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _favouritesTitle->setOverflow(Label::Overflow::SHRINK);
    _favouritesTitle->setTextAreaSize(Size((_contentListView->getSizePercent().x * getContentSize().width) - kListViewSidePadding, _favouritesTitle->getContentSize().height));
    _favouritesTitle->setTextColor(Color4B::WHITE);
    _favouritesTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentListView->pushBackCustomItem(_favouritesTitle);
    
    _favouritesLayout = CircleContentHolder::create();
    _favouritesLayout->setTileSize(_isPortrait ? kCircleTileSizePortrait : kCircleTileSizeLandscape);
    _favouritesLayout->setPlaceholder("res/hqscene/game_icon.png");
    _favouritesLayout->setContentSize(Size(_contentListView->getSizePercent().x * getContentSize().width, 0));
    _favouritesLayout->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            _contentSelectedCallback(content, elementIndex, -1);
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
    _offlineDropdown->setFrameColour(Style::Color::azure);
    _offlineDropdown->setPatternColour(Style::Color::azure);
    _offlineDropdown->setContentSelectedCallback([this](HQContentItemObjectRef content, int elementIndex){
        if(_contentSelectedCallback)
        {
            _contentSelectedCallback(content, elementIndex, -3);
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

std::vector<std::string> OomeeHQ::getJsonFileListFromDir() const
{
    return DirUtil::getFoldersInDirectory(ConfigStorage::getInstance()->getGameCachePath());
}

bool OomeeHQ::isStarterFileExists(const std::string &gameId) const
{
    if(getStartFileFromJson(gameId) == ConfigStorage::kGameDownloadError) return false;
    
    std::string path = ConfigStorage::getInstance()->getGameCachePath() + gameId + "/" + getStartFileFromJson(gameId);
    return FileUtils::getInstance()->isFileExist(path);
}

std::string OomeeHQ::getStartFileFromJson(const std::string &gameId) const
{
    std::string jsonFileName = ConfigStorage::getInstance()->getGameCachePath() + gameId + "/package.json";
    
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    if(gameData.HasParseError())
    {
        return ConfigStorage::kGameDownloadError;
    }
    
    if(gameData.HasMember("pathToStartPage"))
    {
        return getStringFromJson("pathToStartPage", gameData);
    }
    else
    {
        return ConfigStorage::kGameDownloadError;
    }
}

void OomeeHQ::refreshOfflineList()
{
    std::vector<HQContentItemObjectRef> gameList;
    const std::vector<std::string>& jsonList = getJsonFileListFromDir();
    
    for(const auto& json : jsonList)
    {
        if(json.length() > 3)
        {
            if(isStarterFileExists(json))
            {
                auto item = ContentItemManager::getInstance()->getContentItemForId(json);
                if(item && item->isEntitled())
                {
                    gameList.push_back(item);
                }
            }
        }
    }
    
    MutableHQCarouselObjectRef carousel = MutableHQCarouselObject::create();
    carousel->setColour(Color4B(Style::Color::azure));
    carousel->addContentItemsToCarousel(gameList);
    carousel->setTitle(_("Offline"));
    
    _offlineDropdown->setContentItemData(carousel);
}

void OomeeHQ::refreshFavouritesList()
{
    const auto& favList = FavouritesManager::getInstance()->getFavouriteContent();
    MutableHQCarouselObjectRef carousel = MutableHQCarouselObject::create();
    carousel->setColour(Color4B(Style::Color::azure));
    carousel->addContentItemsToCarousel(favList);
    carousel->setTitle(_("Favourites"));
    _favouritesLayout->setMaxRows(favList.size() > 0 ? -1 : 1);
    _favouritesLayout->setContentItemData(carousel);
}

NS_AZOOMEE_END
