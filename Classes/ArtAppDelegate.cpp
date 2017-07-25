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
    return fileName;
}

void ArtAppDelegate::setFileName(std::string filename)
{
    this->fileName = filename;
}

void ArtAppDelegate::onArtAppNavigationBack()
{
    if(HQHistoryManager::getInstance()->isOffline)
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(OfflineArtsAppHQ));
    else
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
}


NS_AZOOMEE_END
