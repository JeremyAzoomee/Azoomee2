#include "ChatDelegate.h"

#include <cocos/cocos2d.h>
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
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
        HQHistoryManager::getInstance()->addHomeIfHistoryEmpty();
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
    }
    else
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
    }
    
}

void ChatDelegate::onChatAddFriend()
{
    // TODO: Show parent pin etc.
}

NS_AZOOMEE_END
