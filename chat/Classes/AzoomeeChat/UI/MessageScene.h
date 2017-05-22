#ifndef AzoomeeChat_MessageScene_h
#define AzoomeeChat_MessageScene_h

#include "../AzoomeeChat.h"
#include "../ChatAPI.h"
#include "MessageListView.h"
#include "TitleBarWidget.h"
#include "MessageComposer.h"
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class MessageScene : public Azoomee::Scene, public ChatAPIObserver, public MessageComposer::Delegate
{
    typedef Azoomee::Scene Super;
private:
    
    /// The friend we're displaying messages for
    FriendRef _friendData;
    
    /// Root layout for all elements
    cocos2d::ui::Layout* _rootLayout = nullptr;
    
    /// Titlebar at the top of the screen
    TitleBarWidget* _titleBar = nullptr;
    /// Container layout for the content (always sits below the titlebar)
    cocos2d::ui::Layout* _contentLayout = nullptr;
    
    /// Message List
    MessageListView* _messageListView = nullptr;
    /// Message composer
    MessageComposer* _messageComposer = nullptr;
    
    /// Private construction: Use ::create
    MessageScene(const FriendRef& friendData);
    
    /// Create the content UI
    void createContentUI(cocos2d::ui::Layout* parent);
    
    /// Back button was pressed
    void onBackButtonPressed();
    
    // - ChatAPIObserver
    void onChatAPIGetChatMessages(const MessageList& messageList) override;
    void onChatAPISendMessage(const MessageRef& sentMessage) override;
    
    // - MessageComposer::Delegate
    void onMessageComposerSendMessage(const std::string& message) override;

protected:
    
    /// Called when the content size of this scene has changed
    virtual void onSizeChanged() override;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    static MessageScene* create(const FriendRef& friendData);
};

NS_AZOOMEE_CHAT_END

#endif
