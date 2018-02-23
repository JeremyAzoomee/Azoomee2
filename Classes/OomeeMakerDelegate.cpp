//
//  OomeeMakerDelegate.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/02/2018.
//

#include "OomeeMakerDelegate.h"
#include "SceneManagerScene.h"

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

NS_AZOOMEE_END
