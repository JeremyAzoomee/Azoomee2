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

std::string ArtAppDelegate::getFileName()
{
    return filename;
}

void ArtAppDelegate::setFileName(std::string filename)
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
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
    }
    
    
}

void ArtAppDelegate::onArtAppShareImage()
{
    ChatDelegate::getInstance()->_imageFileName = filename;
    if(filename != "")
    {
        if(!HQHistoryManager::getInstance()->isOffline && ChildDataProvider::getInstance()->getIsChildLoggedIn())
        {
            ArtAppRunning = false;
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
        }
    }
}


NS_AZOOMEE_END
