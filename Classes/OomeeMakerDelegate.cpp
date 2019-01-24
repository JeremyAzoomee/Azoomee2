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
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeOomeeMaker/UI/OomeeMakerScene.h>
#include <AzoomeeOomeeMaker/UI/OomeeSelectScene.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>

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
    HQHistoryManager::getInstance()->_returnedFromForcedOrientation = true;
    AnalyticsSingleton::getInstance()->contentItemClosedEvent();
    if(!HQHistoryManager::getInstance()->isOffline)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
    }
    else
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(OfflineHub));
    }
}

void OomeeMakerDelegate::onOomeeMakerShareOomee(const std::string& filename)
{
    ChatDelegate::getInstance()->_imageFileName = filename;
    if(filename != "")
    {
        if(!HQHistoryManager::getInstance()->isOffline && ChildDataProvider::getInstance()->isChildLoggedIn())
        {
            HQHistoryManager::getInstance()->_returnedFromForcedOrientation = true;
            Director::getInstance()->getTextureCache()->reloadTexture(filename);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
            AnalyticsSingleton::getInstance()->shareOomee();
        }
    }
}

void OomeeMakerDelegate::onOomeeMakerUpdateAvatar(const std::string &filename)
{
	if(ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
	{
		const std::string& localChildPath = FileUtils::getInstance()->getWritablePath() + "anonLocalChild/";
		
		if(!FileUtils::getInstance()->isDirectoryExist(localChildPath))
		{
			FileUtils::getInstance()->createDirectory(localChildPath);
		}
		
		const std::string& avatarImgPath = localChildPath + "avatar.png";
		const std::string& avatarDataPath = localChildPath + "avatar.oomee";
		
		const std::string& imageData = FileUtils::getInstance()->getStringFromFile(filename);
		FileUtils::getInstance()->writeStringToFile(imageData, avatarImgPath);
		const std::string& oomeeData = FileUtils::getInstance()->getStringFromFile(filename.substr(0,filename.length() - 3) + "oomee");
		FileUtils::getInstance()->writeStringToFile(oomeeData, avatarDataPath);
		
		if(TutorialController::getInstance()->isTutorialActive())
		{
			if(TutorialController::getInstance()->getCurrentState() == TutorialController::kConfirmOomee)
			{
				TutorialController::getInstance()->nextStep();
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(AddChildAnon));
			}
		}
	}
	else
	{
		ModalMessages::getInstance()->startLoading();
		const std::string& imageData = FileUtils::getInstance()->getStringFromFile(filename);
		char* str = nullptr;
		base64Encode((unsigned char*)imageData.c_str(), (unsigned int)imageData.length(), &str);
		
		HttpRequestCreator* request = API::UpdateChildAvatar(ChildDataProvider::getInstance()->getParentOrChildId(), str, this);
		request->execute();
	}
}

void OomeeMakerDelegate::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagUpdateChildAvatar)
    {
        rapidjson::Document json;
        json.Parse(body.c_str());
		ChildRef child = ChildDataProvider::getInstance()->getLoggedInChild();
        child->setAvatar(getStringFromJson("avatar", json));
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
