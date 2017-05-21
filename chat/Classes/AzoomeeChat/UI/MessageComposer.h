#ifndef AzoomeeChat_MessageComposer_h
#define AzoomeeChat_MessageComposer_h

#include "../AzoomeeChat.h"
#include <AzoomeeCommon/UI/SplitLayout.h>
#include <AzoomeeCommon/UI/ObservableLayout.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class MessageComposer : public ObservableLayout
{
    typedef ObservableLayout Super;
private:
    
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
    
protected:
    
    virtual void onSizeChanged() override;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    CREATE_FUNC(MessageComposer);
};

NS_AZOOMEE_CHAT_END

#endif
