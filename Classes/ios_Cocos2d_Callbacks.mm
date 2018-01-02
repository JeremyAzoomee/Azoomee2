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
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "ContentHistoryManager.h"

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
    ContentHistoryManager::getInstance()->setReturnedFromContent(true);
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
    CCLOG("host: %s, query: %s", host, query);
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
    CCLOG("returnString in callback listener: %s", returnString);
    
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

bool isDeviceIphoneX()
{
    return ConfigStorage::getInstance()->isDeviceIphoneX();
}

NS_AZOOMEE_END
