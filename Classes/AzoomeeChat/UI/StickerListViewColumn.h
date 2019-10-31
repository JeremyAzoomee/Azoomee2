#ifndef AzoomeeChat_StickerListViewColumn_h
#define AzoomeeChat_StickerListViewColumn_h

#include "../AzoomeeChat.h"
#include "../Data/StickerCategory.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class StickerListViewColumn : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(const StickerRef&)> ItemSelectedCallback;
private:
    
    /// The stickers being displayed in this column
    StickerRef _stickers[2];
    
    /// Callback for an item being selected
    ItemSelectedCallback _selectedEventCallback = nullptr;
    
    /// Where we place the content inside the item
    cocos2d::ui::Layout* _contentLayout = nullptr;
    
    /// Top row
    cocos2d::ui::Layout* _topRowLayout = nullptr;
    /// Top sticker button
    cocos2d::ui::Button* _topRowButton = nullptr;
    
    /// Bottom row
    cocos2d::ui::Layout* _bottomRowLayout = nullptr;
    /// Bottom sticker button
    cocos2d::ui::Button* _bottomRowButton = nullptr;
    
    
    /// Resize the button images
    void resizeButtons();
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    
public:
    
    /// Set this item to two stickers in the list, starting with index
    void setData(const StickerList& stickers, int startIndex);
    
    /// Register for item selection events
    void addItemSelectedEventListener(const ItemSelectedCallback& callback);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    CREATE_FUNC(StickerListViewColumn);
};

NS_AZOOMEE_CHAT_END

#endif
