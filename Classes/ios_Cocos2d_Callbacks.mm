#include "ios_Cocos2d_Callbacks.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/WebGameAPI/WebGameAPIDataManager.h>
#include "SceneManagerScene.h"
#include "LoginController.h"
#include "FlowDataSingleton.h"
#include "ErrorCodes.h"
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectManager.h>
#include <AzoomeeCommon/ContentDataManagers/ContentHistoryManager.h>
#include <AzoomeeCommon/ContentDataManagers/FavouritesManager.h>
#include "ChatDelegate.h"
#include <AzoomeeCommon/WebGameAPI/VideoPlaylistManager.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/ContentDataManagers/RecentlyPlayedManager.h>
#include <AzoomeeCommon/Device.h>
#include <AzoomeeCommon/Data/AppConfig.h>
#include "RewardManager.h"

using namespace cocos2d;
USING_NS_TZ

NS_AZ_BEGIN

void navigateToBaseScene()
{
    AnalyticsSingleton::getInstance()->contentItemClosedEvent();
    
    if(HQHistoryManager::getInstance()->isOffline())
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
        return;
    }

    ContentHistoryManager::getInstance()->setReturnedFromContent(true);
	
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
}

void navigateToLoginScene()
{
    AnalyticsSingleton::getInstance()->contentItemClosedEvent();
    FlowDataSingleton::getInstance()->setErrorCode(ERROR_CODE_SOMETHING_WENT_WRONG);
    LoginController::getInstance()->doLoginLogic();
}

void sendMixPanelData(const char* host, const char* query)
{
    cocos2d::log("host: %s, query: %s", host, query);
    std::string strHost = cocos2d::StringUtils::format("%s", host);
    std::string strQuery = cocos2d::StringUtils::format("%s", query);
    
    if(strHost == "video.play")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerVideoPlayEvent();
    }
    
    if(strHost == "video.pause")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerPausedEvent();
    }
    
    if(strHost == "video.quality")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerQualityEvent(strQuery);
    }
    
    if(strHost == "video.time")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerProgressEvent(std::atoi(strQuery.c_str()));
    }
    
    if(strHost == "video.complete")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerVideoCompletedEvent();
    }
    if(strHost == "video.firstFrame")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerFirstFrameEvent(strQuery.c_str());
    }
    
    if(strHost == "video.playlistComplete")
    {
        AnalyticsSingleton::getInstance()->mediaPlayerPlaylistCompletedEvent();
    }
    
    if(strHost == "video.playlistItem")
    {
        HQContentItemObjectRef contentItem = VideoPlaylistManager::getInstance()->getContentItemDataForPlaylistElement(std::atoi(strQuery.c_str()));
        AnalyticsSingleton::getInstance()->updateContentItemDetails(contentItem);
        AnalyticsSingleton::getInstance()->mediaPlayerNewPlaylistItemSetEvent(std::atoi(strQuery.c_str()));
    }
}

const char* sendGameApiRequest(const char* method, const char* responseid, const char* score)
{
    char* returnString = WebGameAPIDataManager::getInstance()->handleAPIRequest(method, responseid, score);
    cocos2d::log("returnString in callback listener: %s", returnString);
    
    return returnString;
}

const char* getLocalStorageForGame()
{
    char* returnString = WebGameAPIDataManager::getInstance()->getLocalStorageData();
    return returnString;
}

void saveLocalStorageData(NSString* stringToSave)
{
    std::string stringToSaveStd = [stringToSave UTF8String];
    WebGameAPIDataManager::getInstance()->saveLocalStorageData(stringToSaveStd);
}

NSString* getVideoPlaylist()
{
    std::string returnString = VideoPlaylistManager::getInstance()->getPlaylist();
    return [NSString stringWithUTF8String:returnString.c_str()];
}

NSString* getRemoteWebGameAPIPath()
{
    return [NSString stringWithUTF8String:AppConfig::getInstance()->getRemoteWebGameAPIPath().c_str()];
}

bool isDeviceIphoneX()
{
    return TZ::Device::getInstance()->isDeviceIphoneX();
}

void favContent()
{
    FavouritesManager::getInstance()->addToFavourites(ContentHistoryManager::getInstance()->getLastOpenedContent());
}

void unFavContent()
{
    FavouritesManager::getInstance()->removeFromFavourites(ContentHistoryManager::getInstance()->getLastOpenedContent());
}

bool isFavContent()
{
    return FavouritesManager::getInstance()->isFavouriteContent(ContentHistoryManager::getInstance()->getLastOpenedContent()->getContentItemId());
}

void shareContentInChat()
{
	if(!HQHistoryManager::getInstance()->isOffline())
	{
    	ChatDelegate::getInstance()->_sharedContentId = ContentHistoryManager::getInstance()->getLastOpenedContent()->getContentItemId();
    	ChatDelegate::getInstance()->shareContentInChat();
	}
}

bool isChatEntitled()
{
    return !HQHistoryManager::getInstance()->isOffline() && HQDataObjectManager::getInstance()->getHQDataObjectForKey(HQConsts::kChatHQName)->getHqEntitlement();
}

bool isAnonUser()
{
    return UserAccountManager::getInstance()->isLoggedInParentAnonymous();
}

void releaseCachedHQMemory()
{
    HQHistoryManager::getInstance()->releaseCachedHQScene();
}

void sendProgressMetaDataVideo(int videoProgressSeconds, int videoDuration)
{
	ContentHistoryManager::getInstance()->onVideoContentClosed(videoProgressSeconds, videoDuration);
}

void sendProgressMetaDataGame()
{
	ContentHistoryManager::getInstance()->onGameContentClosed();
    // Notify RewardManager to calculate reward
    RewardManager::getInstance()->calculateRewardForContent(ContentHistoryManager::getInstance()->getLastOpenedContent(), ContentHistoryManager::getInstance()->getTimeInContentSec());
}

void newVideoOpened(int playlistIndex)
{
	const auto& contentItem = VideoPlaylistManager::getInstance()->getContentItemDataForPlaylistElement(playlistIndex);
	RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), HQConsts::kVideoHQName);
	RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), HQConsts::kOomeeHQName);
	ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
	ContentHistoryManager::getInstance()->onContentOpened();
	
}

NSString* getPlaylistString()
{
    return [NSString stringWithUTF8String:VideoPlaylistManager::getInstance()->getPlaylistForIosNativePlayer().c_str()];
}

NSString* getNSStringForKey(const char* key)
{
    return [NSString stringWithUTF8String:LocaleManager::getInstance()->getStringForKey(key).c_str()];
}

NS_AZ_END
