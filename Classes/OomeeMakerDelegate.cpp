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
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
}

void OomeeMakerDelegate::onOomeeMakerShareOomee(const std::string& filename)
{
    ChatDelegate::getInstance()->_imageFileName = filename;
    if(filename != "")
    {
        if(!HQHistoryManager::getInstance()->isOffline && ChildDataProvider::getInstance()->getIsChildLoggedIn())
        {
            Director::getInstance()->getTextureCache()->reloadTexture(filename);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
        }
    }
}

NS_AZOOMEE_END
