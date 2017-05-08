#ifndef AzoomeeChat_ChatTestScene_h
#define AzoomeeChat_ChatTestScene_h

#include "AzoomeeChat/AzoomeeChat.h"
#include "AzoomeeChat/ChatAPI.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class ChatTestScene : public cocos2d::Scene, public ChatAPIObserver, public cocos2d::IMEDelegate
{
    typedef cocos2d::Scene Super;
private:
    
    /// Listener for window changes
    cocos2d::EventListenerCustom* _windowChangedEvent = nullptr;
    
    /// Root layout for all elements
    cocos2d::ui::Layout* _rootLayout = nullptr;
    /// Layout for the content (sits below title)
    cocos2d::ui::Layout* _contentLayout = nullptr;
    /// Layout for the left side
    cocos2d::ui::Layout* _leftLayout = nullptr;
    /// Layout for the right side
    cocos2d::ui::Layout* _rightLayout = nullptr;
    
    // Titlebar layout
    cocos2d::ui::Layout* _titleLayout = nullptr;
    /// Back button
    cocos2d::ui::Button* _backButton = nullptr;
    /// Title label
    cocos2d::ui::Text* _titleLabel = nullptr;
    
    /// Contact list
    cocos2d::ui::ListView* _contactListView = nullptr;
    /// Contact list item template
    cocos2d::ui::Button* _contactListViewItem = nullptr;
    
    /// Message list
    cocos2d::ui::ListView* _messageListView = nullptr;
    /// Message list item template
    cocos2d::ui::Button* _messageListViewItem = nullptr;
    
    /// Message entry
    cocos2d::ui::TextField* _messageEntryField = nullptr;
    /// Send button
    cocos2d::ui::Button* _sendButton = nullptr;
    
    
    /// Currently selected friend
    FriendRef _selectedFriend;
    
    
    /// Create the title UI
    void createTitleUI(cocos2d::ui::Layout* parent);
    /// Create the left UI
    void createLeftSideUI(cocos2d::ui::Layout* parent);
    /// Create the right side UI
    void createRightSideUI(cocos2d::ui::Layout* parent);
    
    /// Called when projection/size changes
    void onWindowChanged(cocos2d::EventCustom* event);
    
    /// Called on TextField events
    void onTextFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
    
    /// Select a friend from the friend list
    void selectFriend(int index);
    
    // - ChatAPIObserver
    void onChatAPIGetFriendList(const FriendList& friendList) override;
    void onChatAPIGetChatMessages(const MessageList& messageList) override;
    
    // - IMEDelegate
    void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardDidShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardDidHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
    
public:
    
    virtual ~ChatTestScene();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(ChatTestScene);
};

NS_AZOOMEE_CHAT_END

#endif
