#include "HQDataProvider.h"
#include "HQDataParser.h"

#include <external/json/document.h>
#include <external/json/writer.h>
#include <external/json/stringbuffer.h>
#include <external/json/prettywriter.h>

#include "HQScene2.h"
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

void HQDataProvider::startBuildingHQ(const std::string &hqName)
{
    hideLoadingScreen();
    
    Scene *runningScene = Director::getInstance()->getRunningScene();

    Node *contentLayer = runningScene->getChildByName(ConfigStorage::kContentLayerName);
    if(contentLayer == nullptr)
    {
        return;
    }
    
    HQScene2 *hqLayer = (HQScene2 *)contentLayer->getChildByName(hqName.c_str());
    hqLayer->startBuildingScrollView();
    
}

void HQDataProvider::getDataForHQ(const std::string &hqName)
{
    displayLoadingScreen();
    
#ifdef FORCE_RELOAD
        HQDataStorage::getInstance()->HQData.erase(hqName.c_str());
#endif
    startBuildingHQ(hqName);
}

void HQDataProvider::getDataForGroupHQ(const std::string &uri)
{
    displayLoadingScreen();
    HQDataObjectRef groupHQObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName);
    
    groupHQObject->clearData();
    
    HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kGroupHQName)->setHqEntitlement(true); //group hq entitlement is not in the initial login feed, so we have to make it enabled manually.
    HQStructureHandler::getInstance()->loadGroupHQData(uri);
    startBuildingHQ(ConfigStorage::kGroupHQName);
    //BackEndCaller::getInstance()->getHQContent(uri, ConfigStorage::kGroupHQName);
}

int HQDataProvider::getNumberOfRowsForHQ(const std::string &hqName) const
{
    return (int)HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().size();
}

int HQDataProvider::getNumberOfElementsForRow(const std::string &hqName, int index) const
{
    if(getNumberOfRowsForHQ(hqName) <= index)
    {
        return 0;
    }
    return (int)HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(index)->getContentItems().size();
}

std::vector<HQContentItemObjectRef> HQDataProvider::getElementsForRow(const std::string &hqName, int index)
{
    if(getNumberOfRowsForHQ(hqName) <= index)
    {
        return std::vector<HQContentItemObjectRef>();
    }
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(index)->getContentItems();
}

std::string HQDataProvider::getTitleForRow(const std::string &hqName, int index) const
{
    if(getNumberOfRowsForHQ(hqName) <= index)
    {
        return "";
    }
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(index)->getTitle();
}

HQContentItemObjectRef HQDataProvider::getItemDataForSpecificItem(const std::string &itemid)
{
    return ContentItemPool::getInstance()->getContentItemForId(itemid);
}

Vec2 HQDataProvider::getHighlightDataForSpecificItem(const std::string &hqName, int rowNumber, int itemNumber) const
{
    try
    {
        return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(rowNumber)->getContentItemHighlights().at(itemNumber);
    }
    catch(std::out_of_range)
    {
        return cocos2d::Vec2(1,1);
    }
}

std::string HQDataProvider::getThumbnailUrlForItem(const std::string &hqName, int rowNumber, int itemNumber) const
{
    try
    {
        const cocos2d::Vec2 &shape = getHighlightDataForSpecificItem(hqName, rowNumber, itemNumber);
        HQContentItemObjectRef element = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels()[rowNumber]->getContentItems()[itemNumber];
        
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

std::string HQDataProvider::getThumbnailUrlForItem(const std::string &hqName, const std::string &itemId) const
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

std::string HQDataProvider::getHumanReadableHighlightDataForSpecificItem(const std::string &hqName, int rowNumber, int itemNumber) const
{
    try{
        const Vec2 &highlightData = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(rowNumber)->getContentItemHighlights().at(itemNumber);
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

std::string HQDataProvider::getTypeForSpecificItem(const std::string &hqName, const std::string &itemId) const
{
    HQContentItemObjectRef targetObject = ContentItemPool::getInstance()->getContentItemForId(itemId);
    if(targetObject)
    {
        return targetObject->getType();
    }
    
    return "";
}

std::vector<HQContentItemObjectRef> HQDataProvider::getAllContentItemsInRow(const std::string &hqName, int rowNumber)                               //this method is being used for creating playlist data when playing video
{
    if(getNumberOfRowsForHQ(hqName) <= rowNumber)
    {
        return std::vector<HQContentItemObjectRef>();
    }
    
    HQCarouselObjectRef requiredObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(rowNumber);
    std::vector<HQContentItemObjectRef> contentItemObjects = requiredObject->getContentItems();
    
    std::vector<HQContentItemObjectRef> returnArray;
    
    for(int elementIndex = 0; elementIndex < contentItemObjects.size(); elementIndex++)
    {
        HQContentItemObjectRef extendedObj = contentItemObjects.at(elementIndex);
        
        //TODO objectTypes should be pre-configured.
        
        if(((extendedObj->getType() == ConfigStorage::kContentTypeVideo || extendedObj->getType() == ConfigStorage::kContentTypeAudio)) && extendedObj->isEntitled())
        {
            extendedObj->setElementNumber(elementIndex);
            extendedObj->setElementShape(getHighlightDataForSpecificItem(hqName, rowNumber, elementIndex));
            extendedObj->setImagePath(getThumbnailUrlForItem(hqName, rowNumber, elementIndex));
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
    { ConfigStorage::kContentTypeGroup, "res/hqscene/locked_video.png" },
    { ConfigStorage::kContentTypeAudioGroup, "res/hqscene/locked_audio_books.png" },
    { ConfigStorage::kContentTypeGame, "res/hqscene/locked_games.png" },
    { "", "res/hqscene/locked.png" }
};

NS_AZOOMEE_END
