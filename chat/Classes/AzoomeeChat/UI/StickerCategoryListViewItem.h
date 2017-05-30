#ifndef AzoomeeChat_StickerCategoryListViewItem_h
#define AzoomeeChat_StickerCategoryListViewItem_h

#include "../AzoomeeChat.h"
#include "../Data/StickerCategory.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class StickerCategoryListViewItem : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    /// The category this item is displaying
    StickerCategoryRef _stickerCategory;
    
    /// Where we place the content inside the menu item
    cocos2d::ui::Layout* _contentLayout = nullptr;
    /// The category image
    cocos2d::ui::ImageView* _imageView = nullptr;
    /// Border we display "between" items
    cocos2d::ui::Layout* _border = nullptr;
    
protected:
    
    virtual void doLayout() override;
    virtual void onSizeChanged() override;
    virtual void onPressStateChangedToNormal() override;
    virtual void onPressStateChangedToPressed() override;
    virtual void onPressStateChangedToDisabled() override;
    
public:
    
    /// Set this item to display data for category
    void setData(const StickerCategoryRef& category);
    /// Get the data this item is displaying
    StickerCategoryRef getData() const;
    
    /// Release the selection
    void releaseSelectedState();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    CREATE_FUNC(StickerCategoryListViewItem);
};

NS_AZOOMEE_CHAT_END

#endif
