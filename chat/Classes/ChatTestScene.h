#ifndef AzoomeeChat_ChatTestScene_h
#define AzoomeeChat_ChatTestScene_h

#include "AzoomeeChat/AzoomeeChat.h"
#include "AzoomeeChat/ChatAPI.h"
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class ChatTestScene : public Azoomee::Scene, public ChatAPIObserver, public cocos2d::IMEDelegate, public MessageBoxDelegate
{
private:
    typedef Azoomee::Scene Super;
    
    /// Keep track of if the keyboard is visible
    bool _keyboardVisible = false;
    /// Time for next auto get messages call
    float _timeTillGet = -1.0f;
    
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
    
    /// Create a MenuItem entry for Message
    cocos2d::ui::Widget* createMessageMenuItem(const MessageRef& message);
    /// Set the UI content for a Message MenuItem
    void updateMessageMenuItem(cocos2d::ui::Widget* item, const MessageRef& message);
    
    /// Show update notes if needed
    void showUpdateNotesIfNeeded();
    
    /// Called when we should resize UI elements to fit a keyboard
    /// If a duration > 0 is provided, it will be animated
    void resizeUIForKeyboard(float keyboardHeight, float duration);
    
    /// Called on TextField events
    void onTextFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
    
    /// Select a friend from the friend list
    void selectFriend(int index);
    void selectFriend(const FriendRef& friendObj);
    /// Back button pressed
    void onBackButtonPressed();
    /// Refresh button pressed
    void onRefreshButtonPressed();
    /// Send a message to the current contact
    void sendMessage(const std::string& message);
    
    
    /// Called when the scene size changes
    void onContentSizeChanged(const cocos2d::Size& contentSize) override;
    
    // - ChatAPIObserver
    void onChatAPIGetFriendList(const FriendList& friendList) override;
    void onChatAPIGetChatMessages(const MessageList& messageList) override;
    void onChatAPISendMessage(const MessageRef& sentMessage) override;
    
    // - IMEDelegate
    void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardDidShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardDidHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
    
    // - MessageBoxDelegate
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle) override;
    
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void update(float dt) override;

    CREATE_FUNC(ChatTestScene);
};

NS_AZOOMEE_CHAT_END

#endif
