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
    
    HQDataObject *objectToBeLoaded = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category);
        
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

void HQDataProvider::getDataForGroupHQ(std::string uri)
{
    displayLoadingScreen();
    BackEndCaller::getInstance()->getHQContent(uri, "GROUP HQ");
}

int HQDataProvider::getNumberOfRowsForHQ(std::string category)
{
    return (int)HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()->size();
}

int HQDataProvider::getNumberOfElementsForRow(std::string category, int index)
{
    return (int)HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()->at(index).getContentItems().size();
}

std::vector<HQContentItemObject *> HQDataProvider::getElementsForRow(std::string category, int index)
{
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()->at(index).getContentItems();
}

std::string HQDataProvider::getTitleForRow(std::string category, int index)
{
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()->at(index).getTitle();
}

HQContentItemObject HQDataProvider::getItemDataForSpecificItem(std::string category, std::string itemid)
{
    return *HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getContentItemForId(itemid);
}

Vec2 HQDataProvider::getHighlightDataForSpecificItem(std::string category, int rowNumber, int itemNumber)
{
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()->at(rowNumber).getContentItemHighlights()[itemNumber];
}

std::string HQDataProvider::getHumanReadableHighlightDataForSpecificItem(std::string category, int rowNumber, int itemNumber)
{
    Vec2 highlightData = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()->at(rowNumber).getContentItemHighlights()[itemNumber];
    return StringUtils::format("%d,%d", int(highlightData.x), int(highlightData.y));
}

std::string HQDataProvider::getTypeForSpecificItem(std::string category, std::string itemId)
{
    HQContentItemObject *targetObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getContentItemForId(itemId);
    return targetObject->getType();
}

std::vector<HQContentItemObject> HQDataProvider::getAllContentItemsInRow(std::string category, int rowNumber)                               //this method is being used for creating playlist data when playing video
{
    HQCarouselObject requiredObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getHqCarousels()->at(rowNumber);
    std::vector<HQContentItemObject *> contentItemObjects = requiredObject.getContentItems();
    
    std::vector<HQContentItemObject> returnArray;
    
    for(int i = 0; i < contentItemObjects.size(); i++)
    {
        HQContentItemObject extendedObj = *contentItemObjects.at(i);
        
        if(((extendedObj.getType() == "VIDEO" || extendedObj.getType() == "AUDIO")) && extendedObj.getEntitled())
        {
            extendedObj.setElementNumber(i);
            extendedObj.setElementShape(getHighlightDataForSpecificItem(category, rowNumber, i));
            extendedObj.setImagePath(getImageUrlForItem(extendedObj.getContentItemId(), Vec2(1,1)));
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

NS_AZOOMEE_END
