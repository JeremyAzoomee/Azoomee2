//
//  MeHQDownloads.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/05/2018.
//

#include "MeHQDownloads.h"
#include "HQSceneElement.h"
#include "HQScene2ElementPositioner.h"
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
    
    int isPortrait = Director::getInstance()->getVisibleSize().width < Director::getInstance()->getVisibleSize().height;
    
    this->setContentSize(Size(Director::getInstance()->getVisibleSize().width, 0));
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    auto labelLayout = ui::Layout::create();
    labelLayout->setContentSize(Size(Director::getInstance()->getVisibleSize().width, 2 * kSpaceAboveCarousel[isPortrait] + kContentItemMargin[isPortrait]));
    labelLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    labelLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    this->addChild(labelLayout);
    
    ui::Text* heading = ui::Text::create("My Downloads", Style::Font::Regular, 150);
    heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setContentSize(Size(this->getContentSize().width, kSpaceAboveCarousel[isPortrait]));
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    labelLayout->addChild(heading);
    
    ui::Text* heading2 = ui::Text::create(StringUtils::format("When you play games they’ll appear%shere, so you’ll be able to play%sthem%swhen you’re offline.", isPortrait ? "\n" : " ", isPortrait ? " " : "\n", isPortrait ? "\n" : " "), Style::Font::Regular, 80);
    heading2->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading2->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,kContentItemMargin[isPortrait],0,0)));
    labelLayout->addChild(heading2);
    
    std::vector<HQContentItemObjectRef> gameList;
    std::vector<std::string> jsonList = getJsonFileListFromDir();
    
    for(int i = 0; i < jsonList.size(); i++)
    {
        if(jsonList.at(i).length() > 3)
        {
            if(isStarterFileExists(jsonList.at(i)))
            {
                auto item = ContentItemPool::getInstance()->getContentItemForId(jsonList.at(i));
                if(item)
                {
                    gameList.push_back(item);
                }
            }
        }
    }
    
    Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
    float unitWidth = (this->getContentSize().width - 2 * kSideMarginSize[isPortrait]) / kUnitsOnScreen[isPortrait];
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    cocos2d::LayerColor* carouselLayer = LayerColor::create(cocos2d::Color4B(255, 0, 0, 0), this->getContentSize().width - 2 * kSideMarginSize[isPortrait], 0);
    
    float lowestElementYPosition = 0;
    
    for(int elementIndex = 0; elementIndex < gameList.size(); elementIndex++)
    {
        auto hqSceneElement = HQSceneElement::create();
        hqSceneElement->setCategory("ME HQ");
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
    
    carouselLayer->setPosition(Vec2(kSideMarginSize[isPortrait], -lowestElementYPosition));
    
    this->addChild(carouselLayer);
    
    this->setContentSize(Size(this->getContentSize().width, -lowestElementYPosition + labelLayout->getContentSize().height));
    
    return true;
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

std::vector<std::string> MeHQDownloads::getJsonFileListFromDir()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path = path + "/gameCache/";
    
    return DirectorySearcher::getInstance()->getFoldersInDirectory(path);
}

bool MeHQDownloads::isStarterFileExists(const std::string &gameId)
{
    if(getStartFileFromJson(gameId) == "ERROR") return false;
    
    std::string path = FileUtils::getInstance()->getWritablePath() + "gameCache/" + gameId + "/" + getStartFileFromJson(gameId);
    return FileUtils::getInstance()->isFileExist(path);
}

std::string MeHQDownloads::getStartFileFromJson(const std::string &gameId)
{
    std::string jsonFileName = FileUtils::getInstance()->getWritablePath() + "gameCache/" + gameId + "/package.json";
    
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    
    rapidjson::Document gameData;
    gameData.Parse(fileContent.c_str());
    
    if(gameData.HasParseError())
    {
        return "ERROR";
    }
    
    if(gameData.HasMember("pathToStartPage"))
    {
        return getStringFromJson("pathToStartPage", gameData);
    }
    else
    {
        return "ERROR";
    }
}


NS_AZOOMEE_END
