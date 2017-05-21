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
    
    /// Callback for an item being selected
    ItemSelectedCallback _selectedEventCallback = nullptr;
    
    
    /// Calculate the item size for the listview
    cocos2d::Size calculateItemSize() const;
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the items to display in this list
    void setItems(const FriendList& friendList);
    
    /// Register for backbutton click events
    void addItemSelectedEventListener(const ItemSelectedCallback& callback);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    CREATE_FUNC(FriendListView);
};

NS_AZOOMEE_CHAT_END

#endif
