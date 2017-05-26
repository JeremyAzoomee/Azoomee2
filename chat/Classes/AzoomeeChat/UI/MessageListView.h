#ifndef AzoomeeChat_MessageListView_h
#define AzoomeeChat_MessageListView_h

#include "../AzoomeeChat.h"
#include "../Data/Message.h"
#include "../Data/Friend.h"
#include "AvatarWidget.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class MessageListView : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    /// Participants in the conversation
    FriendList _participants;
    /// Data being displayed
    MessageList _listData;
    
    /// Foreground layer - holds the Oomees
    cocos2d::ui::Layout* _foreground = nullptr;
    /// Bar sits at the bottom
    cocos2d::ui::ImageView* _avatarBase = nullptr;
    /// Avatar shadows
    cocos2d::ui::ImageView* _avatarShadows[2];
    /// Avatars
    AvatarWidget* _avatars[2];
    
    /// Internal listview to display the messages
    cocos2d::ui::ListView* _listView = nullptr;
    /// Special item we use to display at the bottom of the list view to deal
    /// with the space for avatars
    cocos2d::ui::Layout* _blankListItem = nullptr;
    
    /// Calculate the item size for the listview
    cocos2d::Size calculateItemSize() const;
    
    /// Get the current scroll position
    float getScrollPosition() const;
    /// Set the current scroll position
    void setScrollPosition(float pos);
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the items to display in this list
    void setData(const FriendList& participants, const MessageList& messageList);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    CREATE_FUNC(MessageListView);
};

NS_AZOOMEE_CHAT_END

#endif
