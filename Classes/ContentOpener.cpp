//
//  ContentOpener.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/12/2017.
//

#include "ContentOpener.h"
#include "HQDataProvider.h"
#include "HQHistoryManager.h"
#include "ContentHistoryManager.h"
#include "GameDataManager.h"
#include "VideoPlaylistManager.h"
#include "WebViewSelector.h"
#include "NavigationLayer.h"
#include "RecentlyPlayedManager.h"
#include "ArtAppDelegate.h"
#include "ManualGameInputLayer.h"
#include "DynamicNodeHandler.h"
#include "IAPFlowController.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>
#include <AzoomeeCommon/Crashlytics/CrashlyticsConfig.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<ContentOpener> sContentOpenerSharedInstance;

ContentOpener* ContentOpener::getInstance()
{
    if(!sContentOpenerSharedInstance.get())
    {
        sContentOpenerSharedInstance.reset(new ContentOpener());
    }
    return sContentOpenerSharedInstance.get();
}

ContentOpener::~ContentOpener(void)
{
    
}

bool ContentOpener::init(void)
{
    return true;
}

void ContentOpener::openContentById(const std::string &contentId)
{
    HQContentItemObjectRef contentItem = HQDataProvider::getInstance()->getItemDataForSpecificItem(contentId);
    
    if(contentItem)
    {
        openContentObject(contentItem);
    }
}

void ContentOpener::openContentObject(const HQContentItemObjectRef &contentItem)
{
    if(contentItem == nullptr || !contentItem->isEntitled())
    {
        return;
    }
	
	setCrashlyticsKeyWithString(CrashlyticsConsts::kContentIdKey, contentItem->getContentItemId());
	
    if(contentItem->getType() == ConfigStorage::kContentTypeGame)
    {
        RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(),ConfigStorage::kGameHQName);
        RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), ConfigStorage::kMeHQName);
        ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
		TutorialController::getInstance()->setTutorialCompleted(TutorialController::kFTUPlayGameID);
        GameDataManager::getInstance()->startProcessingGame(contentItem);
    }
    else if(contentItem->getType()  == ConfigStorage::kContentTypeVideo || contentItem->getType()  == ConfigStorage::kContentTypeAudio)
    {
        RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), ConfigStorage::kVideoHQName);
        RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), ConfigStorage::kMeHQName);
        ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
		TutorialController::getInstance()->setTutorialCompleted(TutorialController::kFTUWatchVideoID);
        Director::getInstance()->replaceScene(SceneManagerScene::createWebview(Orientation::Landscape, contentItem->getUri(),Vec2(0,0)));
    }
    else if(contentItem->getType()  == ConfigStorage::kContentTypeAudioGroup || contentItem->getType()  == ConfigStorage::kContentTypeGroup)
    {
        ModalMessages::getInstance()->stopLoading();
        
        HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kGroupHQName);
                
		HQHistoryManager::getInstance()->setGroupHQSourceId(contentItem->getContentItemId());
		
		HQDataProvider::getInstance()->getDataForGroupHQ(contentItem->getUri());
    }
    else if(contentItem->getType() == ConfigStorage::kContentTypeInternal)
    {
        if(contentItem->getUri() == ConfigStorage::kOomeeMakerURI)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OomeeMakerEntryPointScene));
        }
        else if(contentItem->getUri() == ConfigStorage::kArtAppURI)
        {
            ArtAppDelegate::getInstance()->setFileName("");
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ArtAppEntryPointScene));
        }
    }
}

void ContentOpener::doCarouselContentOpenLogic(const HQContentItemObjectRef& contentItem, int rowIndex, int elementIndex, const std::string& hqCategory)
{
	if(contentItem->getType() == ConfigStorage::kContentTypeManual)
	{
		ManualGameInputLayer::create();
		return;
	}
	
	if(TutorialController::getInstance()->isTutorialActive() && (TutorialController::getInstance()->getCurrentState() == TutorialController::kFTUVideoHQContent || TutorialController::getInstance()->getCurrentState() == TutorialController::kFTUGameHQContent || TutorialController::getInstance()->getCurrentState() == TutorialController::kFTUGroupHQContent))
	{
		TutorialController::getInstance()->nextStep();
	}
	
	if(!contentItem->isEntitled())
	{
		AnalyticsSingleton::getInstance()->contentItemSelectedEvent(contentItem, rowIndex, elementIndex, HQDataProvider::getInstance()->getHumanReadableHighlightDataForSpecificItem(hqCategory, rowIndex, elementIndex));
		AnalyticsSingleton::getInstance()->registerCTASource("lockedContent",contentItem->getContentItemId(),contentItem->getType());
		IAPEntryContext context = IAPEntryContext::DEFAULT;
		if(contentItem->getType() == ConfigStorage::kContentTypeGame)
		{
			context = IAPEntryContext::LOCKED_GAME;
		}
		else if(contentItem->getType() == ConfigStorage::kContentTypeVideo || contentItem->getType() == ConfigStorage::kContentTypeGroup)
		{
			context = IAPEntryContext::LOCKED_VIDEO;
		}
		DynamicNodeHandler::getInstance()->startIAPFlow(context);
	}
	
	AnalyticsSingleton::getInstance()->contentItemSelectedEvent(contentItem, rowIndex, elementIndex, HQDataProvider::getInstance()->getHumanReadableHighlightDataForSpecificItem(hqCategory, rowIndex, elementIndex));
	
	if(contentItem->getType() == ConfigStorage::kContentTypeVideo || contentItem->getType() == ConfigStorage::kContentTypeAudio)
	{
		if(hqCategory == ConfigStorage::kGroupHQName)
		{
			VideoPlaylistManager::getInstance()->setPlaylist(HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqCategory)->getHqCarousels().at(rowIndex));
		}
		else
		{
			HQCarouselObjectRef carousel = HQCarouselObject::create();
			carousel->addContentItemToCarousel(contentItem);
			VideoPlaylistManager::getInstance()->setPlaylist(carousel);
		}
	}
	openContentObject(contentItem);
}


NS_AZOOMEE_END
