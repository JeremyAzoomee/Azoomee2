#include "ios_Cocos2d_Callbacks.h"
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "OfflineHubScene.h"
#include "WebGameAPIDataManager.h"
#include "VideoPlaylistManager.h"
#include "SceneManagerScene.h"
#include "LoginLogicHandler.h"
#include "FlowDataSingleton.h"
#include <AzoomeeCommon/ErrorCodes.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include "ContentHistoryManager.h"
#include "FavouritesManager.h"
#include "ChatDelegate.h"
#include "VideoPlaylistManager.h"
#include "BackEndCaller.h"
#include "RecentlyPlayedManager.h"

using namespace cocos2d;
NS_AZOOMEE_BEGIN

void navigateToBaseScene()
{
    AnalyticsSingleton::getInstance()->contentItemClosedEvent();
    
    if(HQHistoryManager::getInstance()->isOffline)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(OfflineHub));
        return;
    }
    if(HQHistoryManager::getInstance()->getCurrentHQ() != ConfigStorage::kHomeHQName && !(HQHistoryManager::getInstance()->getCurrentHQ() == ConfigStorage::kGroupHQName && HQHistoryManager::getInstance()->getPreviousHQ() == ConfigStorage::kHomeHQName))
    {
        ContentHistoryManager::getInstance()->setReturnedFromContent(true);
    }
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
}

void navigateToLoginScene()
{
    AnalyticsSingleton::getInstance()->contentItemClosedEvent();
    FlowDataSingleton::getInstance()->setErrorCode(ERROR_CODE_SOMETHING_WENT_WRONG);
    LoginLogicHandler::getInstance()->doLoginLogic();
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
    return [NSString stringWithUTF8String:ConfigStorage::getInstance()->getRemoteWebGameAPIPath().c_str()];
}

bool isDeviceIphoneX()
{
    return ConfigStorage::getInstance()->isDeviceIphoneX();
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
	if(!HQHistoryManager::getInstance()->isOffline)
	{
    	ChatDelegate::getInstance()->_sharedContentId = ContentHistoryManager::getInstance()->getLastOpenedContent()->getContentItemId();
    	ChatDelegate::getInstance()->shareContentInChat();
	}
}

bool isChatEntitled()
{
    return HQDataObjectStorage::getInstance()->getHQDataObjectForKey(ConfigStorage::kChatHQName)->getHqEntitlement()  && !HQHistoryManager::getInstance()->isOffline;
}

bool isAnonUser()
{
    return ParentDataProvider::getInstance()->isLoggedInParentAnonymous();
}

void sendVideoProgress(int playlistIndex , int videoProgressSeconds)
{
	BackEndCaller::getInstance()->updateVideoProgress(VideoPlaylistManager::getInstance()->getContentItemDataForPlaylistElement(playlistIndex)->getContentItemId(), videoProgressSeconds);
}

void newVideoOpened(int playlistIndex)
{
	const auto& contentItem = VideoPlaylistManager::getInstance()->getContentItemDataForPlaylistElement(playlistIndex);
	RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), ConfigStorage::kVideoHQName);
	RecentlyPlayedManager::getInstance()->addContentIdToRecentlyPlayedFileForHQ(contentItem->getContentItemId(), ConfigStorage::kMeHQName);
	ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
}

NSString* getPlaylistString()
{
    return [NSString stringWithUTF8String:VideoPlaylistManager::getInstance()->getPlaylistForIosNativePlayer().c_str()];
}

NSString* getNSStringForKey(const char* key)
{
    return [NSString stringWithUTF8String:StringMgr::getInstance()->getStringForKey(key).c_str()];
}

NS_AZOOMEE_END
