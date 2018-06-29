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
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "ChatDelegate.h"

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
    
    if(HQHistoryManager::getInstance()->isOffline)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(OfflineArtsAppHQ));
    }
    else
    {
        _returnedFromArtApp = true;
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
    }
    
}

void ArtAppDelegate::onArtAppShareImage()
{
    if(filename != "")
    {
        if(!HQHistoryManager::getInstance()->isOffline && ChildDataProvider::getInstance()->getIsChildLoggedIn())
        {
            ChatDelegate::getInstance()->_imageFileName = filename;
            ArtAppRunning = false;
            _returnedFromArtApp = true;
            Director::getInstance()->getTextureCache()->reloadTexture(filename);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
        }
    }
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
