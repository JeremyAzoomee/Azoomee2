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
#include "RecentlyPlayedManager.h"
#include "ArtAppDelegate.h"
#include "ManualGameInputLayer.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Crashlytics/CrashlyticsConfig.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Cookie/CookieManager.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemManager.h>
#include <AzoomeeCommon/ErrorCodes.h>

#include "AgeGate.h"

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

void ContentOpener::openContentById(const std::string &contentId)
{
    HQContentItemObjectRef contentItem = HQDataProvider::getInstance()->getContentItemFromID(contentId);
    
    if(contentItem)
    {
        openContentObject(contentItem);
    }
}

void ContentOpener::openContentObject(const HQContentItemObjectRef &contentItem)
{
	if(ChildManager::getInstance()->getLoggedInChild()->isSessionExpired() && !_contentItemToOpen)
	{
		_contentItemToOpen = contentItem;
		ModalMessages::getInstance()->startLoading();
		HttpRequestCreator* request = API::RefreshChildCookiesRequest(this);
		request->execute();
		return;
	}
	
    if(contentItem == nullptr || !contentItem->isEntitled())
    {
        return;
    }
	
	setCrashlyticsKeyWithString(CrashlyticsConsts::kContentIdKey, contentItem->getContentItemId());
	
    if(contentItem->getType() == HQContentItemObject::kContentTypeGame)
    {
        RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(),HQConsts::kGameHQName);
        RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), HQConsts::kOomeeHQName);
        ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);

        GameDataManager::getInstance()->startProcessingGame(contentItem);
    }
    else if(contentItem->getType()  == HQContentItemObject::kContentTypeVideo || contentItem->getType()  == HQContentItemObject::kContentTypeAudio)
    {
        RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), HQConsts::kVideoHQName);
        RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), HQConsts::kOomeeHQName);
        ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
        Director::getInstance()->replaceScene(SceneManagerScene::createWebview(Orientation::Landscape, contentItem->getUri(),Vec2(0,0)));
    }
    else if(contentItem->getType()  == HQContentItemObject::kContentTypeAudioGroup || contentItem->getType()  == HQContentItemObject::kContentTypeGroup)
    {
		HQDataProvider::getInstance()->getDataForGroupHQ(contentItem->getUri(), contentItem->getCarouselColour());
    }
    else if(contentItem->getType() == HQContentItemObject::kContentTypeInternal)
    {
        if(contentItem->getUri() == HQConsts::kOomeeMakerURI)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OomeeMakerEntryPointScene));
        }
        else if(contentItem->getUri() == HQConsts::kArtAppURI)
        {
            ArtAppDelegate::getInstance()->setFileName("");
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ArtAppEntryPointScene));
        }
    }
	_contentItemToOpen = nullptr; // dereference at the end as will be pointing to same memory as contentItem in case of refeshed session
}

void ContentOpener::doCarouselContentOpenLogic(const HQContentItemObjectRef& contentItem, int rowIndex, int elementIndex, const std::string& hqCategory, const std::string& location)
{
	if(contentItem->getType() == HQContentItemObject::kContentTypeManual)
	{
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ManualGameInput));
		return;
	}
	
	if(!contentItem->isEntitled())
	{
		AnalyticsSingleton::getInstance()->contentItemSelectedEvent(contentItem, rowIndex, elementIndex, HQDataProvider::getInstance()->getHumanReadableHighlightDataForSpecificItem(hqCategory, rowIndex, elementIndex), location);
		
#ifndef AZOOMEE_VODACOM_BUILD
        AgeGate* ageGate = AgeGate::create();
        ageGate->setActionCompletedCallback([ageGate](AgeGateResult result){
            ageGate->removeFromParent();
            if(result == AgeGateResult::SUCCESS)
            {
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::IAP));
            }
        });
        Director::getInstance()->getRunningScene()->addChild(ageGate,AGE_GATE_Z_ORDER);
#else
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::VodacomOnboarding));
#endif
	}
	
	AnalyticsSingleton::getInstance()->contentItemSelectedEvent(contentItem, rowIndex, elementIndex, HQDataProvider::getInstance()->getHumanReadableHighlightDataForSpecificItem(hqCategory, rowIndex, elementIndex), location);
	
	if(contentItem->getType() == HQContentItemObject::kContentTypeVideo || contentItem->getType() == HQContentItemObject::kContentTypeAudio)
	{
		if(hqCategory == ConfigStorage::kGroupHQName)
		{
			VideoPlaylistManager::getInstance()->setPlaylist(HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqCategory)->getHqCarousels().at(rowIndex));
		}
		else
		{
			MutableHQCarouselObjectRef carousel = MutableHQCarouselObject::create();
            
            const HQContentItemObjectRef& groupForContent = ContentItemManager::getInstance()->getParentOfContentItemForId(contentItem->getContentItemId());
            if(groupForContent)
            {
                const auto& items = HQDataProvider::getInstance()->getContentItemsFromIDs(groupForContent->getItems());
                carousel->addContentItemsToCarousel(items);
            }
            else
            {
                carousel->addContentItemToCarousel(contentItem);
            }
			VideoPlaylistManager::getInstance()->setPlaylist(carousel);
		}
	}
	openContentObject(contentItem);
}

// delegate functions

void ContentOpener::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagChildCookieRefresh)
	{
		ChildManager::getInstance()->parseChildSessionUpdate(body);
		HttpRequestCreator* request = API::GetSessionCookiesRequest(ChildManager::getInstance()->getLoggedInChild()->getId(), ChildManager::getInstance()->getLoggedInChild()->getCDNSessionId(), this);
		request->execute();
	}
	else if(requestTag == API::TagGetSessionCookies)
	{
		ModalMessages::getInstance()->stopLoading();
		CookieManager::getInstance()->parseDownloadCookies(headers);
		openContentObject(_contentItemToOpen);
	}
	else
	{
		ModalMessages::getInstance()->stopLoading();
	}
}
void ContentOpener::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	ModalMessages::getInstance()->stopLoading();
    if(errorCode == ERROR_CODE_OFFLINE)
    {
        openContentObject(_contentItemToOpen);
    }
}

NS_AZOOMEE_END
