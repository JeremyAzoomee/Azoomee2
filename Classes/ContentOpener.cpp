//
//  ContentOpener.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/12/2017.
//

#include "ContentOpener.h"
#include "HQHistoryManager.h"
#include <TinizineCommon/ContentDataManagers/ContentHistoryManager.h>
#include <TinizineCommon/ContentDataManagers/GameDataManager.h>
#include <TinizineCommon/WebGameAPI/VideoPlaylistManager.h>
#include "WebViewSelector.h"
#include <TinizineCommon/ContentDataManagers/RecentlyPlayedManager.h>
#include "ArtAppDelegate.h"
#include "ManualGameInputLayer.h"
#include <TinizineCommon/Analytics/AnalyticsSingleton.h>
#include "ModalMessages.h"
#include <TinizineCommon/Utils/StringFunctions.h>
#include <TinizineCommon/Crashlytics/CrashlyticsConfig.h>
#include <TinizineCommon/Data/Parent/UserAccountManager.h>
#include <TinizineCommon/Data/Child/ChildManager.h>
#include <TinizineCommon/Data/Cookie/CookieManager.h>
#include <TinizineCommon/Data/HQDataObject/ContentItemManager.h>
#include <TinizineCommon/Data/HQDataObject/HQStructureDownloadHandler.h>
#include <TinizineCommon/Data/HQDataObject/HQDataObjectManager.h>
#include "ErrorCodes.h"
#include "SceneManagerScene.h"
#include "PopupMessageBox.h"
#include <TinizineCommon/Utils/LocaleManager.h>

#include "AgeGate.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

const std::string ContentOpener::kGroupRefreshEvent = "groupRefresh";

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
    HQContentItemObjectRef contentItem = ContentItemManager::getInstance()->getContentItemForId(contentId);
    
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
        
        const auto& onSuccessCallback = [](Orientation orientation, const std::string& path, const cocos2d::Vec2& closeButtonAnchor){
            ModalMessages::getInstance()->stopLoading();
            Director::getInstance()->replaceScene(SceneManagerScene::createWebview(orientation, path, closeButtonAnchor));
        };
        
        const auto& onFailedCallback = [](const std::string& errorType, long errorCode){
            ModalMessages::getInstance()->stopLoading();
            long messageCode = ERROR_CODE_SOMETHING_WENT_WRONG;
            if(errorType == GameDataManager::kZipDownloadError || errorType == GameDataManager::kJsonDownloadError)
            {
                messageCode = errorCode;
            }
            else if (errorType == GameDataManager::kVersionIncompatibleError)
            {
                messageCode = ERROR_CODE_GAME_INCOMPATIBLE;
            }
            const auto& errorMessageText = LocaleManager::getInstance()->getErrorMessageWithCode(messageCode);
            
            PopupMessageBox* messageBox = PopupMessageBox::create();
            messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
            messageBox->setBody(errorMessageText.at(ERROR_BODY));
            messageBox->setButtonText(_("Back"));
            messageBox->setButtonColour(Colours::Color_3B::darkIndigo);
            messageBox->setPatternColour(Colours::Color_3B::azure);
            messageBox->setButtonPressedCallback([](MessagePopupBase* pSender){
                pSender->removeFromParent();
            });
            Director::getInstance()->getRunningScene()->addChild(messageBox, 1000);

        };
        ModalMessages::getInstance()->startLoading();
        GameDataManager::getInstance()->startProcessingGame(contentItem, onSuccessCallback, onFailedCallback, HQHistoryManager::getInstance()->isOffline());
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
		getDataForGroupHQ(contentItem->getUri(), contentItem->getCarouselColour());
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
		AnalyticsSingleton::getInstance()->contentItemSelectedEvent(contentItem, rowIndex, elementIndex, location);
		
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
	
	AnalyticsSingleton::getInstance()->contentItemSelectedEvent(contentItem, rowIndex, elementIndex, location);
	
	if(contentItem->getType() == HQContentItemObject::kContentTypeVideo || contentItem->getType() == HQContentItemObject::kContentTypeAudio)
	{
		if(hqCategory == HQDataObject::kGroupHQName)
		{
			VideoPlaylistManager::getInstance()->setPlaylist(HQDataObjectManager::getInstance()->getHQDataObjectForKey(hqCategory)->getHqCarousels().at(rowIndex));
		}
		else
		{
			MutableHQCarouselObjectRef carousel = MutableHQCarouselObject::create();
            
            const HQContentItemObjectRef& groupForContent = ContentItemManager::getInstance()->getParentOfContentItemForId(contentItem->getContentItemId());
            if(groupForContent)
            {
                const auto& items = ContentItemManager::getInstance()->getContentItemsFromIDs(groupForContent->getItems());
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

void ContentOpener::getDataForGroupHQ(const std::string &uri, const Color4B& carouselColour)
{
    HQStructureDownloadHandler::getInstance()->loadGroupHQData(uri);
    Color4B colourCopy = carouselColour; // event is sent immediatly so we send address of colour object stored on the stack so we dont get mem leak
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kGroupRefreshEvent, &colourCopy);
}

// delegate functions

void ContentOpener::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagChildCookieRefresh)
	{
		ChildManager::getInstance()->parseChildSessionUpdate(body);
		HttpRequestCreator* request = API::GetSessionCookiesRequest(ChildManager::getInstance()->getLoggedInChild()->getId(), ChildManager::getInstance()->getLoggedInChild()->getCDNSessionId(), false, this);
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

NS_AZ_END
