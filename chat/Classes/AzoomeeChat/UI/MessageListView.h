#ifndef AzoomeeChat_MessageListView_h
#define AzoomeeChat_MessageListView_h

#include "../AzoomeeChat.h"
#include "../Data/Message.h"
#include "../Data/Friend.h"
#include "AvatarWidget.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


// Enable this to show avatars at the bottom of the list
//#define AVATARS_IN_LISTVIEW


NS_AZOOMEE_CHAT_BEGIN
    
class MessageListView : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    /// Participants in the conversation
    FriendList _participants;
    /// Data being displayed
    MessageList _listData;
    
#ifdef AVATARS_IN_LISTVIEW
    /// Foreground layer - holds the Oomees
    cocos2d::ui::Layout* _foreground = nullptr;
    /// Bar sits at the bottom
    cocos2d::ui::ImageView* _avatarBase = nullptr;
    /// Avatar shadows
    cocos2d::ui::ImageView* _avatarShadows[2];
    /// Avatars
    AvatarWidget* _avatars[2];
#endif
    
    /// Internal listview to display the messages
    cocos2d::ui::ListView* _listView = nullptr;
#ifdef AVATARS_IN_LISTVIEW
    /// Special item we use to display at the bottom of the list view to deal
    /// with the space for avatars
    cocos2d::ui::Layout* _blankListItem = nullptr;
#endif
    
    /// Calculate the item size for the listview
    cocos2d::Size calculateItemSize() const;
    
    /// Get the current scroll position
    float getScrollPosition() const;
    /// Set the current scroll position
    void setScrollPosition(float pos);
    
    /// List was scrolled
    void onScrollEvent(cocos2d::Ref* sender, cocos2d::ui::ScrollView::EventType event);
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the items to display in this list
    void setData(const FriendList& participants, const MessageList& messageList);
    /// Add a new message to the list
    void addMessage(const MessageRef& message);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    static const char* const kEventListenerFlag;
    static const int kMessagesOnPage = 20;

    CREATE_FUNC(MessageListView);
};

NS_AZOOMEE_CHAT_END

#endif
