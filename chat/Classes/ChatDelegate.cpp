#include "ChatDelegate.h"

#include <cocos/cocos2d.h>
#include "ChildSelectorScene.h"
#include "Auth/AuthAPI.h"

using namespace cocos2d;

NS_AZOOMEE_CHAT_BEGIN

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
    // Logout child
    AuthAPI::getInstance()->logoutChild();
    // Back to profile select
    auto childSelectScene = ChildSelectorScene::create();
    Director::getInstance()->replaceScene(childSelectScene);
}

NS_AZOOMEE_CHAT_END
