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
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemPool.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float MeHQDownloads::kSideMarginSize[2] = {20.0f, 10.0f};
const float MeHQDownloads::kSpaceAboveCarousel[2] = {200.0f, 200.0f};
const int MeHQDownloads::kUnitsOnScreen[2] = {4,2};
const float MeHQDownloads::kContentItemMargin[2] = {20.0f, 20.0f};

bool MeHQDownloads::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    int isPortrait = visibleSize.width < visibleSize.height;
    
    this->setContentSize(Size(visibleSize.width, 0));
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    auto labelLayout = ui::Layout::create();
    labelLayout->setContentSize(Size(visibleSize.width, kSpaceAboveCarousel[isPortrait] + kContentItemMargin[isPortrait]));
    labelLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    labelLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    this->addChild(labelLayout);
    
    ui::Text* heading = ui::Text::create("My Downloads", Style::Font::Regular, 100);
    heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setContentSize(Size(visibleSize.width, kSpaceAboveCarousel[isPortrait]));
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    labelLayout->addChild(heading);
    
    std::vector<HQContentItemObjectRef> gameList;
    const std::vector<std::string>& jsonList = getJsonFileListFromDir();
    
    for(const auto& json : jsonList)
    {
        if(json.length() > 3)
        {
            if(isStarterFileExists(json))
            {
                auto item = ContentItemPool::getInstance()->getContentItemForId(json);
                if(item)
                {
                    gameList.push_back(item);
                }
            }
        }
    }
    
    if(gameList.size() > 0)
    {
    
        ui::Text* heading2 = ui::Text::create(StringUtils::format("When you play games they’ll appear%shere, so you’ll be able to play%sthem%swhen you’re offline.", isPortrait ? "\n" : " ", isPortrait ? " " : "\n", isPortrait ? "\n" : " "), Style::Font::Regular, 80);
        heading2->setTextHorizontalAlignment(TextHAlignment::CENTER);
        heading2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        heading2->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,kContentItemMargin[isPortrait],0,0)));
        labelLayout->addChild(heading2);
        
        labelLayout->setContentSize(Size(visibleSize.width, 2 * kSpaceAboveCarousel[isPortrait] + kContentItemMargin[isPortrait]));
        
        Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
        float unitWidth = (visibleSize.width - 2 * kSideMarginSize[isPortrait]) / kUnitsOnScreen[isPortrait];
        float unitMultiplier = unitWidth / contentItemSize.width;
        
        cocos2d::LayerColor* carouselLayer = LayerColor::create(cocos2d::Color4B(255, 0, 0, 0), this->getContentSize().width - 2 * kSideMarginSize[isPortrait], 0);
        
        float lowestElementYPosition = 0;
        
        for(int elementIndex = 0; elementIndex < gameList.size(); elementIndex++)
        {
            auto hqSceneElement = HQSceneElement::create();
            hqSceneElement->setCategory(ConfigStorage::kMeHQName);
            hqSceneElement->setItemData(gameList[elementIndex]);
            hqSceneElement->setElementRow(-1);
            hqSceneElement->setElementIndex(elementIndex);
            hqSceneElement->setMargin(kContentItemMargin[isPortrait]);
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
        
        int numPlaceholders = (kUnitsOnScreen[isPortrait] * ceil((double)(gameList.size()) / (double)kUnitsOnScreen[isPortrait])) - gameList.size();
        for(int i = 0; i < numPlaceholders; i++)
        {
            Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
            placeholder->setScale(((contentItemSize.width - kContentItemMargin[isPortrait]) * unitMultiplier) / placeholder->getContentSize().width);
            placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(placeholder);
            hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
            hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
            hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            float offset = kContentItemMargin[isPortrait]/2;
            placeholder->setPosition(elementPosition + Vec2(offset, offset));
            carouselLayer->addChild(placeholder);
        }
        
        carouselLayer->setPosition(Vec2(kSideMarginSize[isPortrait], -lowestElementYPosition));
        
        this->addChild(carouselLayer);
        
        this->setContentSize(Size(visibleSize.width, -lowestElementYPosition + labelLayout->getContentSize().height));
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
    
    int isPortrait = visibleSize.width < visibleSize.height;
    
    ui::ImageView* dlLogo = ui::ImageView::create("res/meHQ/ball.png");
    dlLogo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    dlLogo->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    this->addChild(dlLogo);
    
    Rect capInsents = Rect(100, 0, 286, 149);
    
    ui::Button* playGamesButton = ui::Button::create("res/buttons/button_dark.png");
    playGamesButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    playGamesButton->setCapInsets(capInsents);
    playGamesButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    playGamesButton->setContentSize(Size(1000,playGamesButton->getContentSize().height));
    playGamesButton->setScale9Enabled(true);
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
    
    Label* playGamesButtonLabel = Label::createWithTTF("Play games", Style::Font::Regular, playGamesButton->getContentSize().height * 0.4f);
    playGamesButtonLabel->setTextColor(Color4B::WHITE);
    playGamesButtonLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    playGamesButtonLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    playGamesButton->addChild(playGamesButtonLabel);
    
    this->addChild(playGamesButton);
    
    ui::Text* heading = ui::Text::create(StringUtils::format("Play some games and they’ll appear%shere, then you can play them\nwhen you’re offline.", isPortrait ? "\n" : " "), Style::Font::Regular, 80);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    heading->setContentSize(Size(visibleSize.width, 200));
    this->addChild(heading);
    
    this->setContentSize(Size(visibleSize.width, 3 * kSpaceAboveCarousel[isPortrait] + 350 + dlLogo->getContentSize().height));
    
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
