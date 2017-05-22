#ifndef AzoomeeChat_MessageComposer_h
#define AzoomeeChat_MessageComposer_h

#include "../AzoomeeChat.h"
#include <AzoomeeCommon/UI/SplitLayout.h>
#include <AzoomeeCommon/UI/ObservableLayout.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class MessageComposer : public ObservableLayout, public cocos2d::IMEDelegate
{
    typedef ObservableLayout Super;
public:
    
    /// Message composer mode
    enum Mode {
        /// Idle means the bar is small and no current input is available
        Idle = 0,
        /// Text entry mode activates the text entry field and brings up the keyboard
        TextEntry,
        /// Stickers is open
        StickersEntry,
        /// Art gallery is open
        ArtGalleryEntry
    };
    
    struct Delegate
    {
        virtual void onMessageComposerSendMessage(const std::string& message) {};
    };
    
private:
    
    /// The current mode
    MessageComposer::Mode _currentMode = MessageComposer::Mode::Idle;
    /// Current fixed height of the composer
    float _currentHeight = 0.0f;
    /// True if the IME keyboard is currently open
    bool _imeOpen = false;
    
    /// Delegate for the composer
    MessageComposer::Delegate* _delegate = nullptr;
    
    /// Top layout for the message entry & tabs
    SplitLayout* _topLayout = nullptr;
    /// Cancel button
    cocos2d::ui::Button* _cancelButton = nullptr;
    /// Message entry layout
    SplitLayout* _messageEntryLayout = nullptr;
    /// Message entry
    cocos2d::ui::TextField* _messageEntryField = nullptr;
    /// Send button
    cocos2d::ui::Button* _sendButton = nullptr;
    
    /// Stickers tab button
    cocos2d::ui::Button* _stickersTab = nullptr;
    /// Gallery tab button
    cocos2d::ui::Button* _galleryTab = nullptr;
    
    
    /// Create the UI for the top
    void createTopUIContent(SplitLayout* parent);
    /// Create the cancel button
    void createCancelButton(cocos2d::ui::Layout* parent);
    /// Create the message entry UI
    void createMessageEntryUI(cocos2d::ui::Layout* parent);
    /// Create the tab buttons
    void createTabButtonsUI(cocos2d::ui::Layout* parent);
    
    /// Send a message via the delegate
    void sendMessage(const std::string& message);
    
    /// Called when we should resize UI elements to fit a keyboard
    /// If a duration > 0 is provided, it will be animated
    void resizeUIForKeyboard(float keyboardHeight, float duration);
    
    /// Called on TextField events
    void onTextFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
    
    // - IMEDelegate
    void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardDidShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardDidHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
    
protected:
    
    virtual void onSizeChanged() override;
    
public:
    
    /// Get the current mode
    MessageComposer::Mode currentMode() const;
    /// Change the current mode
    void setMode(MessageComposer::Mode mode);
    
    /// Set the delegate
    void setDelegate(MessageComposer::Delegate* delegate);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    CREATE_FUNC(MessageComposer);
};

NS_AZOOMEE_CHAT_END

#endif
