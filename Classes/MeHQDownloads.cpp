//
//  MeHQDownloads.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/05/2018.
//

#include "MeHQDownloads.h"
#include "HQSceneElement.h"
#include "HQScene2ElementPositioner.h"
#include "NavigationLayer.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemPool.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool MeHQDownloads::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    const float spaceAboveCarousel = HQDataProvider::getInstance()->getSpaceAboveCarousel();
    const float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreen();
    const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    this->setContentSize(Size(visibleSize.width, 0));
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    std::vector<HQContentItemObjectRef> gameList;
    const std::vector<std::string>& jsonList = getJsonFileListFromDir();
    
    for(const auto& json : jsonList)
    {
        if(json.length() > 3)
        {
            if(isStarterFileExists(json))
            {
                auto item = ContentItemPool::getInstance()->getContentItemForId(json);
                if(item && item->isEntitled())
                {
                    gameList.push_back(item);
                }
            }
        }
    }
    
    ui::Text* heading = ui::Text::create(StringMgr::getInstance()->getStringForKey((gameList.size() > 0) ? MEHQ_HEADING_DOWNLOADS : MEHQ_HEADING_DOWNLOADS_EMPTY), Style::Font::Regular, 75);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
    heading->setContentSize(Size(visibleSize.width, spaceAboveCarousel));
    this->addChild(heading);
    
    Sprite* icon = Sprite::create("res/meHQ/title_icon_my_downloads.png");
    icon->setAnchorPoint(Vec2(1.5f,0.25f));
    icon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    heading->addChild(icon);
    
    if(gameList.size() > 0)
    {
        Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
        float unitWidth = (visibleSize.width - 2 * sideMargin) / unitsOnScreen;
        float unitMultiplier = unitWidth / contentItemSize.width;
        
        cocos2d::ui::Layout* carouselLayer = ui::Layout::create();
        carouselLayer->setContentSize(Size(visibleSize.width - 2 * sideMargin, 0));
        carouselLayer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
        
        float lowestElementYPosition = 0;
        
        for(int elementIndex = 0; elementIndex < gameList.size(); elementIndex++)
        {
            auto hqSceneElement = HQSceneElement::create();
            hqSceneElement->setCategory(ConfigStorage::kMeHQName);
            hqSceneElement->setItemData(gameList[elementIndex]);
            hqSceneElement->setElementRow(-3);
            hqSceneElement->setElementIndex(elementIndex);
            hqSceneElement->setMargin(contentItemMargin);
            hqSceneElement->setManualSizeMultiplier(unitMultiplier); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
            hqSceneElement->deleteButtonVisible(false);
            
            hqSceneElement->addHQSceneElement();
            
            Vec2 elementShape = Vec2(1,1);
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(hqSceneElement);
            hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
            hqScene2ElementPositioner.setHighlightData(elementShape);
            hqScene2ElementPositioner.setBaseUnitSize(hqSceneElement->getContentSize());
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            
            hqSceneElement->setPosition(elementPosition);
            carouselLayer->addChild(hqSceneElement);
            
            if(elementPosition.y < lowestElementYPosition)
            {
                lowestElementYPosition = elementPosition.y;
            }
        }
        
        int numPlaceholders = (unitsOnScreen * ceil((double)(gameList.size()) / (double)unitsOnScreen)) - gameList.size();
        for(int i = 0; i < numPlaceholders; i++)
        {
            Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
            placeholder->setContentSize(placeholder->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / placeholder->getContentSize().width));
            placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(placeholder);
            hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
            hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
            hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            float offset = contentItemMargin/2;
            placeholder->setPosition(elementPosition + Vec2(offset, offset));
            carouselLayer->addChild(placeholder);
        }
        
        carouselLayer->setPosition(Vec2(sideMargin, -lowestElementYPosition));
        
        this->addChild(carouselLayer);
        
        this->setContentSize(Size(visibleSize.width, -lowestElementYPosition + spaceAboveCarousel));
    }
    else
    {
        buildEmptyCarousel();
    }
    return true;
}

void MeHQDownloads::buildEmptyCarousel()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    const float spaceAboveCarousel = HQDataProvider::getInstance()->getSpaceAboveCarousel();
    const float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreen();
    const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
    float unitWidth = (visibleSize.width - 2 * sideMargin) / unitsOnScreen;
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    cocos2d::ui::Layout* carouselLayer = ui::Layout::create();
    carouselLayer->setContentSize(Size(visibleSize.width - 2 * sideMargin, 0));
    carouselLayer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
    float lowestElementYPosition = 0;
    
    for(int elementIndex = 0; elementIndex < unitsOnScreen - 1; elementIndex++)
    {
        Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
        placeholder->setContentSize(placeholder->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / placeholder->getContentSize().width));
        placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        HQScene2ElementPositioner hqScene2ElementPositioner;
        hqScene2ElementPositioner.setElement(placeholder);
        hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
        hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
        hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
        
        const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
        float offset = contentItemMargin/2;
        placeholder->setPosition(elementPosition + Vec2(offset, offset));
        carouselLayer->addChild(placeholder);
        
        if(elementPosition.y < lowestElementYPosition)
        {
            lowestElementYPosition = elementPosition.y;
        }
    }
    
    ui::Button* playGamesButton = ui::Button::create("res/meHQ/my_downloads_button.png");
    playGamesButton->setContentSize(playGamesButton->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / playGamesButton->getContentSize().width));
    playGamesButton->ignoreContentAdaptWithSize(false);
    playGamesButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    playGamesButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            auto baseLayer = Director::getInstance()->getRunningScene();
            if(baseLayer)
            {
                NavigationLayer *navigationLayer = dynamic_cast<NavigationLayer*>(baseLayer->getChildByName("NavigationLayer"));
                
                if(navigationLayer)
                {
                    navigationLayer->changeToScene(ConfigStorage::kGameHQName, 0.5);
                }
            }
        }
    });
    
    HQScene2ElementPositioner hqScene2ElementPositioner;
    hqScene2ElementPositioner.setElement(playGamesButton);
    hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
    hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
    hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
    
    const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
    float offset = contentItemMargin/2;
    playGamesButton->setPosition(elementPosition + Vec2(offset, offset));
    carouselLayer->addChild(playGamesButton);
    
    for(auto item : carouselLayer->getChildren())
    {
        item->setPosition(item->getPosition() - Vec2(0,lowestElementYPosition));
    }
    
    carouselLayer->setContentSize(Size(carouselLayer->getContentSize().width, -lowestElementYPosition));
    
    this->addChild(carouselLayer);
    
    Label* heading = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(MEHQ_SUB_HEADING_DOWNLOADS), Style::Font::Regular, 80);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setHorizontalAlignment(TextHAlignment::CENTER);
    heading->setMaxLineWidth(visibleSize.width * 0.8f);
    
    ui::Layout* labelHolder = ui::Layout::create();
    labelHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    labelHolder->setContentSize(heading->getContentSize());
    labelHolder->addChild(heading);
    this->addChild(labelHolder);
    
    this->setContentSize(Size(visibleSize.width, labelHolder->getContentSize().height -lowestElementYPosition + spaceAboveCarousel + contentItemMargin + 100));
    
}

void MeHQDownloads::onEnter()
{
    Super::onEnter();
}

void MeHQDownloads::onExit()
{
    Super::onExit();
}

void MeHQDownloads::onSizeChanged()
{
    Super::onSizeChanged();
}

std::vector<std::string> MeHQDownloads::getJsonFileListFromDir() const
{
    return DirectorySearcher::getInstance()->getFoldersInDirectory(ConfigStorage::getInstance()->getGameCachePath());
}

bool MeHQDownloads::isStarterFileExists(const std::string &gameId) const
{
    if(getStartFileFromJson(gameId) == ConfigStorage::kGameDownloadError) return false;
    
    std::string path = ConfigStorage::getInstance()->getGameCachePath() + gameId + "/" + getStartFileFromJson(gameId);
    return FileUtils::getInstance()->isFileExist(path);
}

std::string MeHQDownloads::getStartFileFromJson(const std::string &gameId) const
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


NS_AZOOMEE_END
