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

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float MeHQDownloads::kSideMarginSize = 20.0f;
const float MeHQDownloads::kSpaceAboveCarousel = 200.0f;
const int MeHQDownloads::kUnitsOnScreen = 4;
const float MeHQDownloads::kContentItemMargin = 20.0f;

bool MeHQDownloads::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setContentSize(Size(Director::getInstance()->getVisibleSize().width, 0));
    setBackGroundColor(Color3B::BLUE);
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setLayoutType(ui::Layout::Type::VERTICAL);
    
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
    float unitWidth = (this->getContentSize().width - 2 * kSideMarginSize) / kUnitsOnScreen;
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    cocos2d::LayerColor* carouselLayer = LayerColor::create(cocos2d::Color4B(255, 0, 0, 0), this->getContentSize().width - 2 * kSideMarginSize, 0);
    
    float lowestElementYPosition = 0;
    
    for(int elementIndex = 0; elementIndex < gameList.size(); elementIndex++)
    {
        auto hqSceneElement = HQSceneElement::create();
        hqSceneElement->setCategory("ME HQ");
        hqSceneElement->setItemData(gameList[elementIndex]);
        hqSceneElement->setElementRow(-1);
        hqSceneElement->setElementIndex(elementIndex);
        hqSceneElement->setMargin(kContentItemMargin);
        hqSceneElement->setManualSizeMultiplier(unitMultiplier); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
        
        hqSceneElement->addHQSceneElement();
        
        Vec2 elementShape = Vec2(1,1);
        
        HQScene2ElementPositioner hqScene2ElementPositioner;
        hqScene2ElementPositioner.setElement(hqSceneElement);
        hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
        hqScene2ElementPositioner.setHighlightData(elementShape);
        hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
        
        const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
        
        hqSceneElement->setPosition(elementPosition);
        carouselLayer->addChild(hqSceneElement);
        
        if(elementPosition.y < lowestElementYPosition)
        {
            lowestElementYPosition = elementPosition.y;
        }
    }
    
    carouselLayer->setPosition(Vec2(kSideMarginSize, -lowestElementYPosition));
    
    this->addChild(carouselLayer);
    
    this->setContentSize(Size(this->getContentSize().width, -lowestElementYPosition));
    
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
