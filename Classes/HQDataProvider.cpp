#include "HQDataProvider.h"
#include "HQDataParser.h"
#include "HQDataStorage.h"

#include <external/json/document.h>
#include <external/json/writer.h>
#include <external/json/stringbuffer.h>
#include <external/json/prettywriter.h>

#include "HQScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static HQDataProvider *_sharedHQDataProvider = NULL;

HQDataProvider* HQDataProvider::getInstance()
{
    if (! _sharedHQDataProvider)
    {
        _sharedHQDataProvider = new HQDataProvider();
        _sharedHQDataProvider->init();
    }
    
    return _sharedHQDataProvider;
}

HQDataProvider::~HQDataProvider(void)
{
}

bool HQDataProvider::init(void)
{
    return true;
}

std::string HQDataProvider::getImageUrlForItem(std::string itemId, Vec2 shape)
{
    std::string returnString = StringUtils::format("%s/%s/thumb_%d_%d.jpg", ConfigStorage::getInstance()->getImagesUrl().c_str(), itemId.c_str(), (int)shape.x, (int)shape.y);
    CCLOG("image for item: %s", returnString.c_str());
    return returnString;
}

std::string HQDataProvider::getImageUrlForGroupLogo(std::string itemId)
{
    std::string returnString = StringUtils::format("%s/%s/logo.png", ConfigStorage::getInstance()->getImagesUrl().c_str(), itemId.c_str());
    CCLOG("image for grouplogo: %s", returnString.c_str());
    return returnString;
}

void HQDataProvider::startBuildingHQ(std::string category)
{
    hideLoadingScreen();
    
    if(category != "HOME")
    {
        Scene *runningScene = Director::getInstance()->getRunningScene();
        Node *baseLayer = runningScene->getChildByName("baseLayer");
        Node *contentLayer = baseLayer->getChildByName("contentLayer");
        HQScene *hqLayer = (HQScene *)contentLayer->getChildByName(category.c_str());
        
        //hqLayer->removeAllChildren();
        hqLayer->startBuildingScrollViewBasedOnName();
    }
}

void HQDataProvider::getDataForHQ(std::string category)
{
    displayLoadingScreen();
    
#ifdef forcereload
        HQDataStorage::getInstance()->HQData.erase(category.c_str());
#endif
    
    if(HQDataStorage::getInstance()->HQData.find(category.c_str()) != HQDataStorage::getInstance()->HQData.end())
    {
        startBuildingHQ(category);
    }
    else
    {
        if(HQDataStorage::getInstance()->HQGetContentUrls.find(category.c_str()) != HQDataStorage::getInstance()->HQGetContentUrls.end()) //We don't have the data locally. Now we have to check if the data has to be downloaded (we have an uri for it)
        {
            BackEndCaller::getInstance()->getHQContent(HQDataStorage::getInstance()->HQGetContentUrls[category.c_str()], category.c_str());
        }
    }
}

void HQDataProvider::getDataForGroupHQ(std::string uri)
{
    displayLoadingScreen();
    
    HQDataStorage::getInstance()->HQData["GROUP HQ"].clear();
    BackEndCaller::getInstance()->getHQContent(uri, "GROUP HQ");
}

int HQDataProvider::getNumberOfRowsForHQ(std::string category)
{
    return (int)HQDataStorage::getInstance()->HQListTitles[category].size();
}

int HQDataProvider::getNumberOfElementsForRow(std::string category, int index)
{
    return (int)HQDataStorage::getInstance()->HQListElements[category][index].size();
}

std::vector<std::string> HQDataProvider::getElementsForRow(std::string category, int index)
{
    return HQDataStorage::getInstance()->HQListElements[category][index];
}

std::string HQDataProvider::getTitleForRow(std::string category, int index)
{
    return HQDataStorage::getInstance()->HQListTitles[category].at(index);
}

std::map<std::string, std::string> HQDataProvider::getItemDataForSpecificItem(std::string category, std::string itemid)
{
    std::map<std::string, std::string> result;
    
    std::vector<std::map<std::string, std::string>> toBeChecked = HQDataStorage::getInstance()->HQData[category];
    for(int i = 0; i < toBeChecked.size(); i++)
    {
        if(toBeChecked.at(i)["id"] == itemid) result = toBeChecked.at(i);
    }
    
    return result;
}

Vec2 HQDataProvider::getHighlightDataForSpecificItem(std::string category, int rowNumber, int itemNumber)
{
    return HQDataStorage::getInstance()->HQElementHighlights[category].at(rowNumber).at(itemNumber);
}

std::string HQDataProvider::getHumanReadableHighlightDataForSpecificItem(std::string category, int rowNumber, int itemNumber)
{
    Vec2 highlightData = HQDataStorage::getInstance()->HQElementHighlights[category].at(rowNumber).at(itemNumber);
    return StringUtils::format("%d,%d", int(highlightData.x), int(highlightData.y));
}

std::string HQDataProvider::getTypeForSpecificItem(std::string category, std::string itemId)
{
    std::vector<std::map<std::string, std::string>> allItemsInCategory = HQDataStorage::getInstance()->HQData[category];
    
    for(int i = 0; i < allItemsInCategory.size(); i++)
    {
        std::map<std::string, std::string> currentItem = allItemsInCategory.at(i);
        if(currentItem["id"] == itemId)
        {
            return currentItem["type"];
        }
    }
    
    return "NILTYPE";
}

std::vector<std::map<std::string, std::string>> HQDataProvider::getAllElementDataInRow(std::string category, int rowNumber)
{
    std::vector<std::string> elementids = getElementsForRow(category, rowNumber);
    std::vector<std::map<std::string, std::string>> playlistElements;
    
    for(int i = 0; i < elementids.size(); i++)
    {
        if((getTypeForSpecificItem(category, elementids.at(i)) == "VIDEO" || getTypeForSpecificItem(category, elementids.at(i)) == "AUDIO") && getItemDataForSpecificItem(category, elementids.at(i))["entitled"] == "true")
        {
            std::map<std::string, std::string> elementToBeAdded = getItemDataForSpecificItem(category, elementids.at(i));
            elementToBeAdded["image"] = ConfigStorage::getInstance()->getImagesUrl() + "/" + elementids.at(i) + "/thumb_1_1.jpg";
            elementToBeAdded["elementNumber"] = cocos2d::StringUtils::format("%d", i);
            elementToBeAdded["elementShape"] = getHumanReadableHighlightDataForSpecificItem(category, rowNumber, i);
            
            playlistElements.push_back(elementToBeAdded);
        }
    }
    
    return playlistElements;
}

//---------------------LOADING SCREEN----------------------------------
void HQDataProvider::displayLoadingScreen()
{
    ModalMessages::getInstance()->startLoading();
}

void HQDataProvider::hideLoadingScreen()
{
    ModalMessages::getInstance()->stopLoading();
}

const std::map<std::string, std::string> HQDataProvider::kLockFiles = {
    { "VIDEO", "res/hqscene/locked_video.png" },
    { "AUDIO", "res/hqscene/locked_audio_books.png" },
    { "GAME", "res/hqscene/locked_games.png" },
    { "", "res/hqscene/locked.png" }
};

NS_AZOOMEE_END
