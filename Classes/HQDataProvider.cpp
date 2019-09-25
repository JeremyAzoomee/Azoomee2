#include "HQDataProvider.h"

#include <external/json/document.h>
#include <external/json/writer.h>
#include <external/json/stringbuffer.h>
#include <external/json/prettywriter.h>

#include "BackEndCaller.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemManager.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string HQDataProvider::kGroupRefreshEvent = "groupRefresh";

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
	Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
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
    HQStructureDownloadHandler::getInstance()->loadGroupHQData(uri);
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kGroupRefreshEvent);
}

int HQDataProvider::getNumberOfRowsForHQ(const std::string &hqName) const
{
    return (int)HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().size();
}

int HQDataProvider::getNumberOfElementsForRow(const std::string &hqName, int index) const
{
    if(getNumberOfRowsForHQ(hqName) <= index)
    {
        return 0;
    }
    return (int)HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(index)->getContentItems().size();
}

std::vector<HQContentItemObjectRef> HQDataProvider::getElementsForRow(const std::string &hqName, int index)
{
    if(getNumberOfRowsForHQ(hqName) <= index)
    {
        return std::vector<HQContentItemObjectRef>();
    }
    return HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(index)->getContentItems();
}

std::string HQDataProvider::getTitleForRow(const std::string &hqName, int index) const
{
    if(getNumberOfRowsForHQ(hqName) <= index)
    {
        return "";
    }
    return HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(index)->getTitle();
}

HQContentItemObjectRef HQDataProvider::getItemDataForSpecificItem(const std::string &itemid)
{
    return ContentItemManager::getInstance()->getContentItemForId(itemid);
}

Vec2 HQDataProvider::getHighlightDataForSpecificItem(const std::string &hqName, int rowNumber, int itemNumber) const
{
    try
    {
        return HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(rowNumber)->getContentItemHighlights().at(itemNumber);
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
        HQContentItemObjectRef element = HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels()[rowNumber]->getContentItems()[itemNumber];
        
        return getThumbnailUrlForItem(element, shape);
    } catch (std::out_of_range) {
        return "";
    }
}

std::string HQDataProvider::getThumbnailUrlForItem(const std::string &itemId) const
{
    HQContentItemObjectRef element = ContentItemManager::getInstance()->getContentItemForId(itemId);
    if(element)
    {
        return getThumbnailUrlForItem(element, Vec2(1,1));
    }
    
    return "";
}

std::string HQDataProvider::getThumbnailUrlForItem(const std::string &hqName, const std::string &itemId) const
{
    return getThumbnailUrlForItem(ContentItemManager::getInstance()->getContentItemForId(itemId), Vec2(1,1));
}

std::string HQDataProvider::getThumbnailUrlForItem(HQContentItemObjectRef element, const cocos2d::Vec2 &shape) const
{
    if(element == nullptr)
    {
        return "";
    }
    
    const std::string &key = convertShapeToThumbnailKey(shape);
    const auto& images = element->getImages();
    if(images.find(key) != images.end())
    {
        return images.at(key);
    }
    else if(images.find(convertShapeToThumbnailKey(Vec2(1,1))) != images.end()) //if the queried key does not exist in images map, we try to fall back to ONE_ONE first
    {
        return images.at(convertShapeToThumbnailKey(Vec2(1,1)));
    }
    else //if ONE_ONE even does not exist, we return an empty string
    {
        return "";
    }
}

std::string HQDataProvider::getHumanReadableHighlightDataForSpecificItem(const std::string &hqName, int rowNumber, int itemNumber) const
{
    try{
        const Vec2 &highlightData = HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(rowNumber)->getContentItemHighlights().at(itemNumber);
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
    HQContentItemObjectRef targetObject = ContentItemManager::getInstance()->getContentItemForId(itemId);
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
    
    HQCarouselObjectRef requiredObject = HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqName)->getHqCarousels().at(rowNumber);
	return requiredObject->getContentItems();
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

// hq size/layout params
// Format:
//          [18:9/16:9] [Landscape,Portrait],
//          [16:10] [Landscape, Portrait],
//          [4:3] [Landscape, Portrait]]
//
const float HQDataProvider::kSideMarginSize[3][2] = {{20,10},{20,10},{20,10}};
const float HQDataProvider::kSpaceAboveCarousel[3][2] = {{200,200},{200,200},{200,200}};
const int HQDataProvider::kUnitsOnScreen[3][2] = {{4,2},{4,2},{4,4}};
const int HQDataProvider::kUnitsOnScreenMeHQ[3][2] = {{4,3},{4,3},{4,4}};
const float HQDataProvider::kContentItemMargin[3][2] = {{20,20},{20,20},{20,20}};

float HQDataProvider::getSideMargin() const
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    int isPortrait = visibleSize.width < visibleSize.height;
    float screenRatio =  isPortrait ? (visibleSize.height / visibleSize.width) : (visibleSize.width / visibleSize.height);
        
    int screenIndex = 0;
    if(screenRatio < k16x10UpperBound)
    {
        screenIndex++;
    }
    if(screenRatio < k16x10LowerBound)
    {
        screenIndex++;
    }
    return kSideMarginSize[screenIndex][isPortrait];
    
}

float HQDataProvider::getSpaceAboveCarousel() const
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    int isPortrait = visibleSize.width < visibleSize.height;
    float screenRatio =  isPortrait ? (visibleSize.height / visibleSize.width) : (visibleSize.width / visibleSize.height);
    
    int screenIndex = 0;
    if(screenRatio < (16.0/9.5))
    {
        screenIndex++;
    }
    if(screenRatio < (16.0/10.5))
    {
        screenIndex++;
    }
    return kSpaceAboveCarousel[screenIndex][isPortrait];
}

int HQDataProvider::getUnitsOnScreen() const
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    int isPortrait = visibleSize.width < visibleSize.height;
    float screenRatio =  isPortrait ? (visibleSize.height / visibleSize.width) : (visibleSize.width / visibleSize.height);
    
    int screenIndex = 0;
    if(screenRatio < (16.0/9.5))
    {
        screenIndex++;
    }
    if(screenRatio < (16.0/10.5))
    {
        screenIndex++;
    }
    return kUnitsOnScreen[screenIndex][isPortrait];
    
}

int HQDataProvider::getUnitsOnScreenMeHQ() const
{
	const Size& visibleSize = Director::getInstance()->getVisibleSize();
	int isPortrait = visibleSize.width < visibleSize.height;
	float screenRatio =  isPortrait ? (visibleSize.height / visibleSize.width) : (visibleSize.width / visibleSize.height);
	
	int screenIndex = 0;
	if(screenRatio < (16.0/9.5))
	{
		screenIndex++;
	}
	if(screenRatio < (16.0/10.5))
	{
		screenIndex++;
	}
	return kUnitsOnScreenMeHQ[screenIndex][isPortrait];
	
}

float HQDataProvider::getContentItemMargin() const
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    int isPortrait = visibleSize.width < visibleSize.height;
    float screenRatio =  isPortrait ? (visibleSize.height / visibleSize.width) : (visibleSize.width / visibleSize.height);
    
    int screenIndex = 0;
    if(screenRatio < (16.0/9.5))
    {
        screenIndex++;
    }
    if(screenRatio < (16.0/10.5))
    {
        screenIndex++;
    }
    return kContentItemMargin[screenIndex][isPortrait];
    
}
NS_AZOOMEE_END
