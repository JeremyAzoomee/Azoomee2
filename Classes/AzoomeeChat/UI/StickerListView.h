#ifndef AzoomeeChat_StickerListView_h
#define AzoomeeChat_StickerListView_h

#include "../AzoomeeChat.h"
#include "../Data/Sticker.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


USING_NS_TZ

NS_AZ_CHAT_BEGIN
    
class StickerListView : public cocos2d::ui::ListView
{
    typedef cocos2d::ui::ListView Super;
    typedef std::function<void(const StickerRef&)> ItemSelectedCallback;
private:
    
    /// Data being displayed
    StickerList _listData;
    
    /// Callback for an item being selected
    ItemSelectedCallback _selectedEventCallback = nullptr;
    
    
    /// Calculate the item size for the listview
    cocos2d::Size calculateItemSize() const;
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the items to display in this list
    void setItems(const StickerList& data);
    
    /// Register for item selection events
    void addItemSelectedEventListener(const ItemSelectedCallback& callback);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    CREATE_FUNC(StickerListView);
};

NS_AZ_CHAT_END

#endif
