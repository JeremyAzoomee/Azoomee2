//
//  ArtAppDelegate.cpp
//  azoomee2
//
//  Created by Macauley on 05/07/2017.
//
//

#include "ArtAppDelegate.h"
#include "HQHistoryManager.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include "ChatDelegate.h"
#include "ShareInChatLayer.h"

USING_NS_CC;

NS_AZOOMEE_BEGIN

static std::auto_ptr<ArtAppDelegate> sArtAppDelegateSharedInstance;

ArtAppDelegate* ArtAppDelegate::getInstance()
{
    if(!sArtAppDelegateSharedInstance.get())
    {
        sArtAppDelegateSharedInstance.reset(new ArtAppDelegate());
    }
    return sArtAppDelegateSharedInstance.get();
}

std::string ArtAppDelegate::getFileName() const
{
    return filename;
}

void ArtAppDelegate::setFileName(const std::string& filename)
{
    this->filename = filename;
}

void ArtAppDelegate::onArtAppNavigationBack()
{
    ArtAppRunning = false;
    if(HQHistoryManager::getInstance()->isOffline())
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
    }
    else
    {
        HQHistoryManager::getInstance()->setReturnedFromForcedOrientation(true);
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
    }
    
}

void ArtAppDelegate::onArtAppShareImage()
{
    if(filename != "")
    {
		if(!HQHistoryManager::getInstance()->isOffline() && UserAccountManager::getInstance()->isPaidUser())
        {
            AnalyticsSingleton::getInstance()->contentItemClosedEvent();
            ChatDelegate::getInstance()->_imageFileName = filename;
            ArtAppRunning = false;
            HQHistoryManager::getInstance()->setReturnedFromForcedOrientation(true);
            Director::getInstance()->getTextureCache()->reloadTexture(filename);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ShareInChatScene));
        }
    }
}

bool ArtAppDelegate::isOffline()
{
	return HQHistoryManager::getInstance()->isOffline();
}

void ArtAppDelegate::setSecondsSpentInArtApp(long seconds)
{
    _secondsSpentInArtApp = seconds;
}

long ArtAppDelegate::getSecondsSpentInArtApp() const
{
    return _secondsSpentInArtApp;
}


NS_AZOOMEE_END
