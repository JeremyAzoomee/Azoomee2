#ifndef AzoomeeChat_FriendListViewItem_h
#define AzoomeeChat_FriendListViewItem_h

#include "../AzoomeeChat.h"
#include "../Data/Friend.h"
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
    
    /// BG for oomee if no oomee is displayed
    cocos2d::ui::ImageView* _oomeeBackground = nullptr;
    /// Friend's name
    cocos2d::ui::Text* _nameLabel = nullptr;
    
protected:
    
    virtual void doLayout() override;
    virtual void onPressStateChangedToNormal() override;
    virtual void onPressStateChangedToPressed() override;
    virtual void onPressStateChangedToDisabled() override;
    
public:
    
    /// Set this item to display data for friend
    void setData(const FriendRef& friendData);
    /// Get the data this item is displaying
    FriendRef getData() const;
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(FriendListViewItem);
};

NS_AZOOMEE_CHAT_END

#endif
