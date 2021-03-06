#include "ChatDelegate.h"

#include <cocos/cocos2d.h>
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
#include "FlowDataSingleton.h"
#include "LoginLogicHandler.h"
#include "HQDataProvider.h"
#include "ContentOpener.h"
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/ErrorCodes.h>
#include <AzoomeeCommon/Utils/StringMgr.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemManager.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<ChatDelegate> sChatDelegateSharedInstance;

ChatDelegate* ChatDelegate::getInstance()
{
    if(!sChatDelegateSharedInstance.get())
    {
        sChatDelegateSharedInstance.reset(new ChatDelegate());
    }
    return sChatDelegateSharedInstance.get();
}

void ChatDelegate::shareContentInChat()
{
    if(_sharedContentId != "")
    {
        AnalyticsSingleton::getInstance()->shareContentItemButtonPressed(_sharedContentId);
        const std::string& fileurl = HQDataProvider::getInstance()->getThumbnailUrlForItem(_sharedContentId);
        ImageDownloaderRef imgDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
        imgDownloader->downloadImage(this, fileurl);
    }
}

#pragma mark - Azoomee::Chat::Delegate

void ChatDelegate::onChatNavigationBack()
{
    // Go back to the hub
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        HQHistoryManager::getInstance()->addDefaultHQIfHistoryEmpty();
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
    }
    else
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::SettingsFromChat));
    }
    
}

void ChatDelegate::onChatAddFriend()
{
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::SettingsFromChat));
}

void ChatDelegate::onChatAuthorizationError(const std::string& requestTag, long errorCode)
{
    FlowDataSingleton::getInstance()->setErrorCode(errorCode);
    LoginLogicHandler::getInstance()->doLoginLogic();
}

void ChatDelegate::onChatNavigateToContent(const std::string &contentId)
{
    const auto& contentItem = ContentItemManager::getInstance()->getContentItemForId(contentId);
    if(contentItem)
    {
        AnalyticsSingleton::getInstance()->chatOpenSharedContentEvent(contentId);
        AnalyticsSingleton::getInstance()->contentItemSelectedEvent(contentItem, 0, 0, "", ConfigStorage::kChatHQName);
        ContentOpener::getInstance()->openContentById(contentId);
    }
}

void ChatDelegate::onChatOfflineError(const std::string &requestTag)
{
	if(!HQHistoryManager::getInstance()->isOffline())
	{
		MessageBox::createWith(ERROR_CODE_OFFLINE, this);
	}
}

// delegate functions
void ChatDelegate::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _imageFileName = downloader->getLocalImagePath();
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ShareInChatScene));
}

void ChatDelegate::onImageDownloadFailed()
{
    if(_sharedContentId != "")
    {
        const auto& item = HQDataProvider::getInstance()->getContentItemFromID(_sharedContentId);
        if(item)
        {
            std::string filename = "res/contentPlaceholders/Games1X1.png";
            if(item->getType() == ConfigStorage::kContentTypeVideo)
            {
                filename = "res/contentPlaceholders/Video1X1.png";
            }
            _imageFileName = filename;
        }
    }
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ShareInChatScene));
}

void ChatDelegate::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
	if(messageBoxTitle == StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_OFFLINE).at(ERROR_TITLE))
	{
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
	}
}

NS_AZOOMEE_END
