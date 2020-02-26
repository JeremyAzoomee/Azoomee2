#ifndef AzoomeeChat_MessageComposer_h
#define AzoomeeChat_MessageComposer_h

#include "../AzoomeeChat.h"
#include <AzoomeeCommon/UI/SplitLayout.h>
#include <AzoomeeCommon/UI/ObservableLayout.h>
#include "StickerSelector.h"
#include "ChatTextField.h"
#include "../Data/Message.h"
#include "../Data/Sticker.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "ArtListView.h"


// Enable this define to display gallery art tab on composer
#define AZOOMEE_CHAT_GALLERY_SHARE


USING_NS_TZ

NS_AZ_CHAT_BEGIN
    
class MessageComposer : public ObservableLayout, public cocos2d::IMEDelegate, public cocos2d::TextFieldDelegate
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
#ifdef AZOOMEE_CHAT_GALLERY_SHARE
        /// Art gallery is open
        ArtGalleryEntry
#endif
    };
    
    struct Delegate
    {
        virtual void onMessageComposerSendMessage(const MessageRef& message) {};
    };
    
private:
    
    static const float kMessageInputPadding;
    
    /// The current mode
    MessageComposer::Mode _currentMode = MessageComposer::Mode::Idle;
    /// Current fixed height of the composer
    float _currentHeight = 0.0f;
    /// True if the IME keyboard is currently open
    bool _imeOpen = false;
    /// Last used keyboard height
    float _lastKeyboardHeight = 0.0f;
    
    /// Delegate for the composer
    MessageComposer::Delegate* _delegate = nullptr;
    
    /// Top layout for the message entry & tabs
    SplitLayout* _topLayout = nullptr;
    /// Cancel button
    cocos2d::ui::Button* _cancelButton = nullptr;
    /// Message entry layout
    SplitLayout* _messageEntryLayout = nullptr;
    /// Message entry
    ChatTextField* _messageEntryField = nullptr;
    /// Send button
    cocos2d::ui::Button* _sendButton = nullptr;
    
    /// Layout to hold various selectors, i.e chat & art
    cocos2d::ui::Layout* _selectorLayout = nullptr;
    
    /// Stickers tab button
    cocos2d::ui::Button* _stickersTab = nullptr;
    /// The stickers selector widget
    StickerSelector* _stickerSelector = nullptr;
    
#ifdef AZOOMEE_CHAT_GALLERY_SHARE
    /// Gallery tab button
    cocos2d::ui::Button* _galleryTab = nullptr;
    /// Art gallery selector
    ArtListView* _artListView = nullptr;
#endif
    
    
    /// Touch event listener
    cocos2d::EventListenerTouchOneByOne* _touchListener = nullptr;
    /// Dismiss composer on touch end?
    bool _dismissOnTouchEnd = false;
    
    
    /// Create the UI for the top
    void createTopUIContent(SplitLayout* parent);
    /// Create the cancel button
    void createCancelButton(cocos2d::ui::Layout* parent);
    /// Create the message entry UI
    void createMessageEntryUI(cocos2d::ui::Layout* parent);
    /// Create the tab buttons
    void createTabButtonsUI(cocos2d::ui::Layout* parent);
    
    /// Send a message (text type) via the delegate
    void sendMessage(const std::string& message);
    /// Send a message (sticker type) via the delegate
    void sendMessage(const StickerRef& sticker);
    /// Send a message (Art type) via the delegate
    void sendArtMessage(const std::string& artFile);
    /// Send a message (Content type) via the delegate
    void sendContentMessage(const std::string& contentId);
    
    /// Called when we should resize UI elements to fit a keyboard
    /// If a duration > 0 is provided, it will be animated
    void resizeUIForKeyboard(float keyboardHeight, float duration);
    /// Keyboard resize ended
    void onKeyboardResizeEnded(float keyboardHeight);
    
    /// Get the estimated keyboard height for the IME
    float getEstimatedKeyboardHeight() const;
    
    /// Called on TextField events
    void onTextFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
    /// Update the height of the textfield based on input changes
    void updateTextEntryHeight();
    
    // - IMEDelegate
    void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardDidShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
    void keyboardDidHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
    
    // - TextFieldDelegate
    bool onTextFieldAttachWithIME(cocos2d::TextFieldTTF* sender) override;
    bool onTextFieldDetachWithIME(cocos2d::TextFieldTTF* sender) override;
    bool onTextFieldInsertText(cocos2d::TextFieldTTF* sender, const char* text, size_t nLen) override;
    bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF* sender, const char* delText, size_t nLen) override;
    bool onVisit(cocos2d::TextFieldTTF* sender, cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    
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
    
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;

    CREATE_FUNC(MessageComposer);
};

NS_AZ_CHAT_END

#endif
