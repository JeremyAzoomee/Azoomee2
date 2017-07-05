#ifndef AzoomeeChat_StickerCategoryListView_h
#define AzoomeeChat_StickerCategoryListView_h

#include "../AzoomeeChat.h"
#include "../Data/StickerCategory.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class StickerCategoryListView : public cocos2d::ui::ListView
{
    typedef cocos2d::ui::ListView Super;
    typedef std::function<void(const StickerCategoryRef&)> ItemSelectedCallback;
private:
    
    /// Data being displayed
    StickerCategoryList _listData;
    
    /// Callback for an item being selected
    ItemSelectedCallback _selectedEventCallback = nullptr;
    
    
    /// Calculate the item size for the listview
    cocos2d::Size calculateItemSize() const;
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the items to display in this list
    void setItems(const StickerCategoryList& data);
    /// Select an item
    void selectItem(const StickerCategoryRef& category);
    
    /// Register for item selection events
    void addItemSelectedEventListener(const ItemSelectedCallback& callback);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    CREATE_FUNC(StickerCategoryListView);
};

NS_AZOOMEE_CHAT_END

#endif
