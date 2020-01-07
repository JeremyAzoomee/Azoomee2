#ifndef AzoomeeChat_FriendListViewItem_h
#define AzoomeeChat_FriendListViewItem_h

#include "../AzoomeeChat.h"
#include "../Data/Friend.h"
#include "AvatarWidget.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class FriendListViewItem : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    /// The frienddata this item is displayed
    FriendRef _friendData;
    
    /// Where we place the content inside the menu item
    cocos2d::ui::Layout* _contentLayout = nullptr;
    
    /// Avatar container
    cocos2d::ui::Layout* _avatarLayout = nullptr;
    /// Image displayed if Avatar doesn't exist
    cocos2d::ui::ImageView* _avatarPlaceholder = nullptr;
    /// User's avatar
    AvatarWidget* _avatarWidget = nullptr;
    
    /// Unread message indicator
    cocos2d::ui::ImageView* _unreadIndicator = nullptr;
    
    /// inModeration indicator
    cocos2d::ui::ImageView* _inModerationIndicator = nullptr;
    
    /// Friend's name
    cocos2d::ui::Text* _nameLabel = nullptr;
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    virtual void onPressStateChangedToNormal() override;
    virtual void onPressStateChangedToPressed() override;
    virtual void onPressStateChangedToDisabled() override;
    
public:
    
    /// Set this item to display data for friend
    void setData(const FriendRef& friendData);
    /// Get the data this item is displaying
    FriendRef getData() const;
    
    void hideUnreadIndicator();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(FriendListViewItem);
};

NS_AZOOMEE_CHAT_END

#endif
