//
//  OomeeMakerDelegate.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/02/2018.
//

#include "OomeeMakerDelegate.h"
#include "ChatDelegate.h"
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeOomeeMaker/UI/OomeeMakerScene.h>
#include <AzoomeeOomeeMaker/UI/OomeeSelectScene.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

USING_NS_CC;

NS_AZOOMEE_BEGIN

static std::auto_ptr<OomeeMakerDelegate> sOomeeMakerDelegateSharedInstance;

OomeeMakerDelegate* OomeeMakerDelegate::getInstance()
{
    if(!sOomeeMakerDelegateSharedInstance.get())
    {
        sOomeeMakerDelegateSharedInstance.reset(new OomeeMakerDelegate());
    }
    return sOomeeMakerDelegateSharedInstance.get();
}

void OomeeMakerDelegate::onOomeeMakerNavigationBack()
{
    HQHistoryManager::getInstance()->setReturnedFromForcedOrientation(true);
    AnalyticsSingleton::getInstance()->contentItemClosedEvent();
    if(!HQHistoryManager::getInstance()->isOffline())
    {
		if(_oomeeMakerFromShop)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Shop));
		}
		else
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
		}
    }
    else
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
    }
}

void OomeeMakerDelegate::onOomeeMakerShareOomee(const std::string& filename)
{
    ChatDelegate::getInstance()->_imageFileName = filename;
    if(filename != "")
    {
		if(!HQHistoryManager::getInstance()->isOffline() && ParentManager::getInstance()->isPaidUser())
        {
            ChatDelegate::getInstance()->_sharingOomee = true;
            HQHistoryManager::getInstance()->setReturnedFromForcedOrientation(true);
            Director::getInstance()->getTextureCache()->reloadTexture(filename);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ShareInChatScene));
            AnalyticsSingleton::getInstance()->shareOomee();
        }
    }
}

void OomeeMakerDelegate::onOomeeMakerUpdateAvatar(const std::string &filename)
{
	ModalMessages::getInstance()->startLoading();
	const std::string& imageData = FileUtils::getInstance()->getStringFromFile(filename);
	char* str = nullptr;
	base64Encode((unsigned char*)imageData.c_str(), (unsigned int)imageData.length(), &str);
		
	HttpRequestCreator* request = API::UpdateChildAvatar(ChildManager::getInstance()->getParentOrChildId(), str, this);
	request->execute();
}

void OomeeMakerDelegate::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagUpdateChildAvatar)
    {
        rapidjson::Document json;
        json.Parse(body.c_str());
		ChildManager::getInstance()->parseAvatarUpdate(body);
        ImageDownloaderRef imageDownloader = ImageDownloader::create("imageCache/", ImageDownloader::CacheMode::File );
        imageDownloader->downloadImage(nullptr, getStringFromJson("avatar", json), true);
        ModalMessages::getInstance()->stopLoading();
        auto scene = Director::getInstance()->getRunningScene();
        OomeeMaker::OomeeSelectScene* selectScene = dynamic_cast<OomeeMaker::OomeeSelectScene*>(scene);
        if(selectScene)
        {
            AnalyticsSingleton::getInstance()->makeAvatarSuccess("OOMEE_SELECT");
            selectScene->toggleMakeAvatarHiglight();
            return;
        }
        OomeeMaker::OomeeMakerScene* makerScene = dynamic_cast<OomeeMaker::OomeeMakerScene*>(scene);
        if(makerScene)
        {
            AnalyticsSingleton::getInstance()->makeAvatarSuccess("OOMEE_MAKER");
            makerScene->displayMadeAvatarNotification();
        }
    }
}
void OomeeMakerDelegate::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
}

NS_AZOOMEE_END
