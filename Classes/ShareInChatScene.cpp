//
//  ShareInChatScene.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 19/11/2019.
//

#include "ShareInChatScene.h"
#include "ChatDelegate.h"
#include "SceneManagerScene.h"
#include "ArtAppDelegate.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool ShareInChatScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _shareLayer = ShareInChatLayer::create();
    _shareLayer->setContentImage(ChatDelegate::getInstance()->_imageFileName);
    _shareLayer->setContentId(ChatDelegate::getInstance()->_sharedContentId);
    _shareLayer->setOnCompleteCallback([this](){
        switch (_shareType) {
            case ShareInChatLayer::ART:
            {
                ArtAppDelegate::getInstance()->setFileName(ChatDelegate::getInstance()->_imageFileName);
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ArtAppEntryPointScene));
                break;
            }
            case ShareInChatLayer::CONTENT:
            {
                ChatDelegate::getInstance()->onChatNavigateToContent(ChatDelegate::getInstance()->_sharedContentId);
                break;
            }
            case ShareInChatLayer::OOMEE:
            {
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OomeeMakerEntryPointScene));
                break;
            }
        }
    });
    addChild(_shareLayer);
    
    return true;
}

void ShareInChatScene::onEnter()
{
    Super::onEnter();
}

void ShareInChatScene::onExit()
{
    ChatDelegate::getInstance()->_sharedContentId = "";
    ChatDelegate::getInstance()->_imageFileName = "";
    ChatDelegate::getInstance()->_sharingOomee = false;
    Super::onExit();
}

void ShareInChatScene::onSizeChanged()
{
    Super::onSizeChanged();
}

void ShareInChatScene::setShareType(const ShareInChatLayer::ShareType& type)
{
    _shareType = type;
    _shareLayer->setShareType(type);
}

NS_AZ_END
