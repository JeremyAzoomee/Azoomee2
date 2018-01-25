#include "HQDataProvider.h"
#include "HQDataParser.h"

#include <external/json/document.h>
#include <external/json/writer.h>
#include <external/json/stringbuffer.h>
#include <external/json/prettywriter.h>

#include "HQScene2.h"
#include "MainHubScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemPool.h>

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
    if(baseLayer == nullptr)
    {
        return;
    }
    Node *contentLayer = baseLayer->getChildByName("contentLayer");
    if(contentLayer == nullptr)
    {
        return;
    }
    
    if(category != ConfigStorage::kHomeHQName)
    {
        HQScene2 *hqLayer = (HQScene2 *)contentLayer->getChildByName(category.c_str());
        hqLayer->startBuildingScrollView();
        
        //HQScene *hqLayer = (HQScene *)contentLayer->getChildByName(category.c_str());
        //hqLayer->startBuildingScrollViewBasedOnName();
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
    HQDataObjectRef groupHQObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName);
    
    groupHQObject->clearData();
    
    HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName)->setHqEntitlement(true); //group hq entitlement is not in the initial login feed, so we have to make it enabled manually.
    BackEndCaller::getInstance()->getHQContent(uri, ConfigStorage::kGroupHQName);
}

int HQDataProvider::getNumberOfRowsForHQ(const std::string &category) const
{
    return (int)HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().size();
}

int HQDataProvider::getNumberOfElementsForRow(const std::string &category, int index) const
{
    if(getNumberOfRowsForHQ(category) <= index)
    {
        return 0;
    }
    return (int)HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(index)->getContentItems().size();
}

std::vector<HQContentItemObjectRef> HQDataProvider::getElementsForRow(const std::string &category, int index)
{
    if(getNumberOfRowsForHQ(category) <= index)
    {
        return std::vector<HQContentItemObjectRef>();
    }
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(index)->getContentItems();
}

std::string HQDataProvider::getTitleForRow(const std::string &category, int index) const
{
    if(getNumberOfRowsForHQ(category) <= index)
    {
        return "";
    }
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(index)->getTitle();
}

HQContentItemObjectRef HQDataProvider::getItemDataForSpecificItem(const std::string &itemid)
{
    return ContentItemPool::getInstance()->getContentItemForId(itemid);
}

Vec2 HQDataProvider::getHighlightDataForSpecificItem(const std::string &category, int rowNumber, int itemNumber) const
{
    try
    {
        return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(rowNumber)->getContentItemHighlights().at(itemNumber);
    }
    catch(std::out_of_range)
    {
        return cocos2d::Vec2(1,1);
    }
}

std::string HQDataProvider::getThumbnailUrlForItem(const std::string &category, int rowNumber, int itemNumber) const
{
    try
    {
        const cocos2d::Vec2 &shape = getHighlightDataForSpecificItem(category, rowNumber, itemNumber);
        HQContentItemObjectRef element = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()[rowNumber]->getContentItems()[itemNumber];
        
        return getThumbnailUrlForItem(element, shape);
    } catch (std::out_of_range) {
        return "";
    }
}

std::string HQDataProvider::getThumbnailUrlForItem(const std::string &itemId) const
{
    HQContentItemObjectRef element = ContentItemPool::getInstance()->getContentItemForId(itemId);
    if(element)
    {
        return getThumbnailUrlForItem(element, Vec2(1,1));
    }
    
    return "";
}

std::string HQDataProvider::getThumbnailUrlForItem(const std::string &category, const std::string &itemId) const
{
    return getThumbnailUrlForItem(ContentItemPool::getInstance()->getContentItemForId(itemId), Vec2(1,1));
}

std::string HQDataProvider::getThumbnailUrlForItem(HQContentItemObjectRef element, const cocos2d::Vec2 &shape) const
{
    if(element == nullptr)
    {
        return "";
    }
    
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
    try{
        const Vec2 &highlightData = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(rowNumber)->getContentItemHighlights().at(itemNumber);
        return StringUtils::format("%d,%d", int(highlightData.x), int(highlightData.y));
    }
    catch(std::out_of_range)
    {
        return "1,1";
    }
}

std::string HQDataProvider::convertShapeToThumbnailKey(const cocos2d::Vec2 &shape) const
{
    static const std::vector<std::string> &numbersByWords = {"ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE"};
    return numbersByWords.at(shape.x) + "_" + numbersByWords.at(shape.y);
}

std::string HQDataProvider::getTypeForSpecificItem(const std::string &category, const std::string &itemId) const
{
    HQContentItemObjectRef targetObject = ContentItemPool::getInstance()->getContentItemForId(itemId);
    if(targetObject)
    {
        return targetObject->getType();
    }
    
    return "";
}

std::vector<HQContentItemObjectRef> HQDataProvider::getAllContentItemsInRow(const std::string &category, int rowNumber)                               //this method is being used for creating playlist data when playing video
{
    if(getNumberOfRowsForHQ(category) <= rowNumber)
    {
        return std::vector<HQContentItemObjectRef>();
    }
    
    HQCarouselObjectRef requiredObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels().at(rowNumber);
    std::vector<HQContentItemObjectRef> contentItemObjects = requiredObject->getContentItems();
    
    std::vector<HQContentItemObjectRef> returnArray;
    
    for(int elementIndex = 0; elementIndex < contentItemObjects.size(); elementIndex++)
    {
        HQContentItemObjectRef extendedObj = contentItemObjects.at(elementIndex);
        
        //TODO objectTypes should be pre-configured.
        
        if(((extendedObj->getType() == ConfigStorage::kContentTypeVideo || extendedObj->getType() == ConfigStorage::kContentTypeAudio)) && extendedObj->isEntitled())
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
    { ConfigStorage::kContentTypeVideo, "res/hqscene/locked_video.png" },
    { ConfigStorage::kContentTypeAudio, "res/hqscene/locked_audio_books.png" },
    { ConfigStorage::kContentTypeGame, "res/hqscene/locked_games.png" },
    { "", "res/hqscene/locked.png" }
};

NS_AZOOMEE_END
