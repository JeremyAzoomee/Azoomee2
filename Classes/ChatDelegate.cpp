#include "ChatDelegate.h"

#include <cocos/cocos2d.h>
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
#include "FlowDataSingleton.h"
#include "LoginLogicHandler.h"
#include "ContentHistoryManager.h"
#include "HQDataProvider.h"
#include "DeepLinkingSingleton.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

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
        const std::string& fileurl = HQDataProvider::getInstance()->getThumbnailUrlForItem(_sharedContentId);
        ImageDownloaderRef imgDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
        imgDownloader->downloadImage(this, fileurl);
    }
}

#pragma mark - Azoomee::Chat::Delegate

void ChatDelegate::onChatNavigationBack()
{
    // Go back to the hub
    if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        HQHistoryManager::getInstance()->addDefaultHQIfHistoryEmpty();
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
    }
    else
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
    }
    
}

void ChatDelegate::onChatAddFriend()
{
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SettingsFromChat));
}

void ChatDelegate::onChatAuthorizationError(const std::string& requestTag, long errorCode)
{
    FlowDataSingleton::getInstance()->setErrorCode(errorCode);
    LoginLogicHandler::getInstance()->doLoginLogic();
}

void ChatDelegate::onChatNavigateToContent(const std::string &contentId)
{
    DeepLinkingSingleton::getInstance()->setDeepLink(DeepLinkingSingleton::kPostContentDeeplinkStr + contentId);
}

// delegate functions
void ChatDelegate::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    ChatDelegate::getInstance()->_imageFileName = downloader->getLocalImagePath();
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
    
}

void ChatDelegate::onImageDownloadFailed()
{
    if(_sharedContentId != "")
    {
        const auto& item = HQDataProvider::getInstance()->getItemDataForSpecificItem(_sharedContentId);
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
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
}


NS_AZOOMEE_END
