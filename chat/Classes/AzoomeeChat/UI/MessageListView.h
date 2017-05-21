#ifndef AzoomeeChat_MessageListView_h
#define AzoomeeChat_MessageListView_h

#include "../AzoomeeChat.h"
#include "../Data/Message.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class MessageListView : public cocos2d::ui::ListView
{
    typedef cocos2d::ui::ListView Super;
private:
    
    /// Data being displayed
    MessageList _listData;
    
    
    /// Calculate the item size for the listview
    cocos2d::Size calculateItemSize() const;
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the items to display in this list
    void setItems(const MessageList& messageList);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    CREATE_FUNC(MessageListView);
};

NS_AZOOMEE_CHAT_END

#endif
