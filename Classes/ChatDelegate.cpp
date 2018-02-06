#include "ChatDelegate.h"

#include <cocos/cocos2d.h>
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
#include "FlowDataSingleton.h"
#include "LoginLogicHandler.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<ChatDelegate> sChatDelegateSharedInstance;

ChatDelegate* ChatDelegate::getInstance()
{
    if(!sChatDelegateSharedInstance.get())
    {
        sChatDelegateSharedInstance.reset(new ChatDelegate());
    }
    return sChatDelegateSharedInstance.get();
}

#pragma mark - Azoomee::Chat::Delegate

void ChatDelegate::onChatNavigationBack()
{
    // Go back to the hub
    if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        HQHistoryManager::getInstance()->addDefaultHQIfHistoryEmpty();
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
    }
    else
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
    }
    
}

void ChatDelegate::onChatAddFriend()
{
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SettingsFromChat));
}

void ChatDelegate::onChatAuthorizationError(const std::string& requestTag, long errorCode)
{
    FlowDataSingleton::getInstance()->setErrorCode(errorCode);
    LoginLogicHandler::getInstance()->doLoginLogic();
}

NS_AZOOMEE_END
