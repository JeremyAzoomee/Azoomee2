#include "HQDataProvider.h"
#include "HQDataParser.h"

#include <external/json/document.h>
#include <external/json/writer.h>
#include <external/json/stringbuffer.h>
#include <external/json/prettywriter.h>

#include "HQScene.h"
#include "HQScene2.h"
#include "MainHubScene.h"
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

void HQDataProvider::startBuildingHQ(const std::string &category)
{
    hideLoadingScreen();
    
    Scene *runningScene = Director::getInstance()->getRunningScene();
    Node *baseLayer = runningScene->getChildByName("baseLayer");
    Node *contentLayer = baseLayer->getChildByName("contentLayer");
    
    if(category == "GAME HQ" || category == "VIDEO HQ" || category == "AUDIO HQ")
    {
        HQScene2 *hqLayer = (HQScene2 *)contentLayer->getChildByName(category.c_str());
        hqLayer->startBuildingScrollView();
    }
    else if(category != "HOME")
    {
        HQScene *hqLayer = (HQScene *)contentLayer->getChildByName(category.c_str());
        hqLayer->startBuildingScrollViewBasedOnName();
    }
    else
    {
        MainHubScene *homeLayer = (MainHubScene *)contentLayer->getChildByName(category.c_str());
        if(homeLayer->getChildrenCount() == 0)
        {
            homeLayer->buildMainHubScene();
        }
    }
}

void HQDataProvider::getDataForHQ(const std::string &category)
{
    displayLoadingScreen();
    
#ifdef forcereload
        HQDataStorage::getInstance()->HQData.erase(category.c_str());
#endif
    
    const HQDataObjectRef &objectToBeLoaded = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category);
        
    if(objectToBeLoaded->getHqType() != "")
    {
        startBuildingHQ(category);
    }
    else
    {
        if(objectToBeLoaded->getHqUrl() != "")
        {
            BackEndCaller::getInstance()->getHQContent(objectToBeLoaded->getHqUrl(), category);
        }
    }
}

void HQDataProvider::getDataForGroupHQ(const std::string &uri)
{
    displayLoadingScreen();
    HQDataObjectRef groupHQObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey("GROUP HQ");
    
    groupHQObject->clearData();
    
    HQDataObjectStorage::getInstance()->getHQDataObjectForKey("GROUP HQ")->setHqEntitlement(true); //group hq entitlement is not in the initial login feed, so we have to make it enabled manually.
    BackEndCaller::getInstance()->getHQContent(uri, "GROUP HQ");
}

int HQDataProvider::getNumberOfRowsForHQ(const std::string &category)
{
    return (int)HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().size();
}

int HQDataProvider::getNumberOfElementsForRow(const std::string &category, int index) const
{
    return (int)HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(index)->getContentItems().size();
}

std::vector<HQContentItemObjectRef> HQDataProvider::getElementsForRow(const std::string &category, int index)
{
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(index)->getContentItems();
}

std::string HQDataProvider::getTitleForRow(const std::string &category, int index) const
{
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(index)->getTitle();
}

HQContentItemObjectRef HQDataProvider::getItemDataForSpecificItem(const std::string &category,  const std::string &itemid)
{
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getContentItemForId(itemid);
}

HQContentItemObjectRef HQDataProvider::getItemDataForSpecificItem(const std::string &itemid)
{
    const std::vector<std::string>& hqNames = {"GAME HQ", "VIDEO HQ", "AUDIO_HQ"};
    for(const std::string& cat : hqNames)
    {
        HQContentItemObjectRef item = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(cat)->getContentItemForId(itemid);
        if(item)
        {
            return item;
        }
    }
    
    return nullptr;
}

Vec2 HQDataProvider::getHighlightDataForSpecificItem(const std::string &category, int rowNumber, int itemNumber) const
{
    if(rowNumber >= HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().size() || itemNumber >= HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()[rowNumber]->getContentItemHighlights().size())
    {
        cocos2d::log("HIGHLIGHT OUT OF INDEX ERROR: %d, %d", rowNumber, itemNumber);
        return cocos2d::Vec2(1,1);
    }
    
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(rowNumber)->getContentItemHighlights().at(itemNumber);
}

std::string HQDataProvider::getThumbnailUrlForItem(const std::string &category, int rowNumber, int itemNumber) const
{
    if(rowNumber >= HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().size() || itemNumber >= HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()[rowNumber]->getContentItems().size())
    {
        cocos2d::log("THUMBNAIL OUT OF INDEX ERROR: %d, %d", rowNumber, itemNumber);
        return "";
    }
    
    const cocos2d::Vec2 &shape = getHighlightDataForSpecificItem(category, rowNumber, itemNumber);
    HQContentItemObjectRef element = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()[rowNumber]->getContentItems()[itemNumber];
    
    return getThumbnailUrlForItem(element, shape);
}

std::string HQDataProvider::getThumbnailUrlForItem(const std::string &itemId) const
{
    const std::vector<std::string>& hqNames = {"GAME HQ", "VIDEO HQ", "AUDIO_HQ", "GROUP HQ"};
    
    for(const std::string& cat : hqNames)
    {
        HQContentItemObjectRef element = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(cat)->getContentItemForId(itemId);
        if(element)
        {
            return getThumbnailUrlForItem(element, Vec2(1,1));
        }
    }
    
    return nullptr;
}

std::string HQDataProvider::getThumbnailUrlForItem(const std::string &category, const std::string &itemId) const
{
    return getThumbnailUrlForItem(HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getContentItemForId(itemId), Vec2(1,1));
}

std::string HQDataProvider::getThumbnailUrlForItem(HQContentItemObjectRef element, const cocos2d::Vec2 shape) const
{
    const std::string &key = convertShapeToThumbnailKey(shape);
    
    if(element->getImages().find(key) != element->getImages().end())
    {
        return element->getImages().at(key);
    }
    else if(element->getImages().find(convertShapeToThumbnailKey(Vec2(1,1))) != element->getImages().end()) //if the queried key does not exist in images map, we try to fall back to ONE_ONE first
    {
        return element->getImages().at(convertShapeToThumbnailKey(Vec2(1,1)));
    }
    else //if ONE_ONE even does not exist, we return an empty string
    {
        return "";
    }
}

std::string HQDataProvider::getHumanReadableHighlightDataForSpecificItem(const std::string &category, int rowNumber, int itemNumber) const
{
    const Vec2 &highlightData = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(rowNumber)->getContentItemHighlights().at(itemNumber);
    return StringUtils::format("%d,%d", int(highlightData.x), int(highlightData.y));
}

std::string HQDataProvider::convertShapeToThumbnailKey(const cocos2d::Vec2 &shape) const
{
    const std::vector<std::string> &numbersByWords = {"ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE"};
    return numbersByWords.at(shape.x) + "_" + numbersByWords.at(shape.y);
}

std::string HQDataProvider::getTypeForSpecificItem(const std::string &category, const std::string &itemId) const
{
    HQContentItemObjectRef targetObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getContentItemForId(itemId);
    return targetObject->getType();
}

std::vector<HQContentItemObjectRef> HQDataProvider::getAllContentItemsInRow(const std::string &category, int rowNumber)                               //this method is being used for creating playlist data when playing video
{
    HQCarouselObjectRef requiredObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(rowNumber);
    std::vector<HQContentItemObjectRef> contentItemObjects = requiredObject->getContentItems();
    
    std::vector<HQContentItemObjectRef> returnArray;
    
    for(int elementIndex = 0; elementIndex < contentItemObjects.size(); elementIndex++)
    {
        HQContentItemObjectRef extendedObj = contentItemObjects.at(elementIndex);
        
        //TODO objectTypes should be pre-configured.
        
        if(((extendedObj->getType() == "VIDEO" || extendedObj->getType() == "AUDIO")) && extendedObj->isEntitled())
        {
            extendedObj->setElementNumber(elementIndex);
            extendedObj->setElementShape(getHighlightDataForSpecificItem(category, rowNumber, elementIndex));
            extendedObj->setImagePath(getThumbnailUrlForItem(category, rowNumber, elementIndex));
        }
        
        returnArray.push_back(extendedObj);
    }
    
    return returnArray;
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
