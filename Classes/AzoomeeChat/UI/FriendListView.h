#ifndef AzoomeeChat_FriendListView_h
#define AzoomeeChat_FriendListView_h

#include "../AzoomeeChat.h"
#include "../Data/Friend.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class FriendListView : public cocos2d::ui::ListView
{
    typedef cocos2d::ui::ListView Super;
    typedef std::function<void(const FriendRef&)> ItemSelectedCallback;
private:
    
    /// Data being displayed
    FriendList _listData;
    /// Number of columns to display the data
    int _columns = 1;
    
    bool _includeAddFriendButton = true;
    
    /// Callback for an item being selected
    ItemSelectedCallback _selectedEventCallback = nullptr;
    
    
    /// Calculate the item size for the listview
    cocos2d::Size calculateItemSize() const;
    
    cocos2d::ui::Layout* createAddFriendButton();
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    virtual void interceptTouchEvent(cocos2d::ui::Widget::TouchEventType event, cocos2d::ui::Widget *sender, cocos2d::Touch* touch) override;
    
public:
    
    /// Set the number of columns
    void setColumns(int columns);
    
    /// Set the items to display in this list
    void setItems(const FriendList& friendList);
    
    void setIncludeAddFriendButton(bool includeButton);
    
    /// Register for backbutton click events
    void addItemSelectedEventListener(const ItemSelectedCallback& callback);
    
    void hideUnreadIndicators();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    CREATE_FUNC(FriendListView);
};

NS_AZOOMEE_CHAT_END

#endif
