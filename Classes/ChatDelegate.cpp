#include "ChatDelegate.h"

#include <cocos/cocos2d.h>
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
#include "FlowDataSingleton.h"
#include "LoginController.h"
#include "ContentOpener.h"
#include <TinizineCommon/Data/Child/ChildManager.h>
#include <TinizineCommon/Analytics/AnalyticsSingleton.h>
#include "ErrorCodes.h"
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/Data/HQDataObject/ContentItemManager.h>
#include "PopupMessageBox.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

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
        const std::string& fileurl = ContentItemManager::getInstance()->getThumbnailUrlForItem(_sharedContentId);
        ImageDownloaderRef imgDownloader = ImageDownloader::create(ImageDownloader::kImageCachePath, ImageDownloader::CacheMode::File);
        imgDownloader->downloadImage(this, fileurl);
    }
}

#pragma mark - AZ::Chat::Delegate

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
    LoginController::getInstance()->doLoginLogic();
}

void ChatDelegate::onChatNavigateToContent(const std::string &contentId)
{
    const auto& contentItem = ContentItemManager::getInstance()->getContentItemForId(contentId);
    if(contentItem)
    {
        AnalyticsSingleton::getInstance()->chatOpenSharedContentEvent(contentId);
        AnalyticsSingleton::getInstance()->contentItemSelectedEvent(contentItem, 0, 0, HQConsts::kChatHQName);
        ContentOpener::getInstance()->openContentById(contentId);
    }
}

void ChatDelegate::onChatOfflineError(const std::string &requestTag)
{
	if(!HQHistoryManager::getInstance()->isOffline())
	{
        const auto& errorMessageText = LocaleManager::getInstance()->getErrorMessageWithCode(ERROR_CODE_OFFLINE);
               
        PopupMessageBox* messageBox = PopupMessageBox::create();
        messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
        messageBox->setBody(errorMessageText.at(ERROR_BODY));
        messageBox->setButtonText(_("Back"));
        messageBox->setButtonColour(Colours::Color_3B::darkIndigo);
        messageBox->setPatternColour(Colours::Color_3B::azure);
        messageBox->setButtonPressedCallback([this](MessagePopupBase* pSender){
            pSender->removeFromParent();
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
        });
        Director::getInstance()->getRunningScene()->addChild(messageBox, 1);
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
        const auto& item = ContentItemManager::getInstance()->getContentItemForId(_sharedContentId);
        if(item)
        {
            std::string filename = "res/contentPlaceholders/Games1X1.png";
            if(item->getType() == HQContentItemObject::kContentTypeVideo)
            {
                filename = "res/contentPlaceholders/Video1X1.png";
            }
            _imageFileName = filename;
        }
    }
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ShareInChatScene));
}

NS_AZ_END
