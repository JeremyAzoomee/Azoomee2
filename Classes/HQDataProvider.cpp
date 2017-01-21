#include "HQDataProvider.h"
#include "HQDataParser.h"
#include "HQDataStorage.h"

USING_NS_CC;

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "external/json/prettywriter.h"

#include "HQScene.h"
#include "BackEndCaller.h"
#include "HttpRequestCreator.h"
#include "ConfigStorage.h"

using namespace cocos2d;

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
    std::string returnString = StringUtils::format("https://media.azoomee.ninja/static/images/%s/thumb_%d_%d.jpg", itemId.c_str(), (int)shape.x, (int)shape.y);
    CCLOG("image for item: %s", returnString.c_str());
    return returnString;
}

void HQDataProvider::startBuildingHQ(std::string category)
{
    if(category != "HOME")
    {
        Scene *runningScene = Director::getInstance()->getRunningScene();
        Node *baseLayer = runningScene->getChildByName("baseLayer");
        Node *contentLayer = baseLayer->getChildByName("contentLayer");
        HQScene *hqLayer = (HQScene *)contentLayer->getChildByName(category.c_str());
        
        hqLayer->startBuildingScrollViewBasedOnName();
    }
}

void HQDataProvider::getDataForHQ(std::string category)
{
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
            HQDataParser::getInstance()->getContent(HQDataStorage::getInstance()->HQGetContentUrls[category.c_str()], category.c_str());
        }
    }
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
