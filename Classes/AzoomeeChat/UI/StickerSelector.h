#ifndef AzoomeeChat_StickerSelector_h
#define AzoomeeChat_StickerSelector_h

#include "../AzoomeeChat.h"
#include "StickerCategoryListView.h"
#include "StickerListView.h"
#include <TinizineCommon/UI/SplitLayout.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


USING_NS_TZ

NS_AZ_CHAT_BEGIN
    
class StickerSelector : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(const StickerRef&)> StickerSelectedCallback;
private:
    
    /// Currently selected category
    StickerCategoryRef _selectedCategory;
    /// Category list
    StickerCategoryList _categories;
    
    /// Callback for a sticker being selected
    StickerSelectedCallback _selectedEventCallback = nullptr;
    
    /// Split layout, holds tabs at the bottom and list uses remaining space
    SplitLayout* _splitLayout = nullptr;
    
    /// Category selection
    StickerCategoryListView* _categoryListView = nullptr;
    /// Sticker selection (content changes based on category selection)
    StickerListView* _stickerListView = nullptr;
    
protected:
    
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the height of the tab bar
    void setTabBarHeight(float height);
    /// Select a category
    void selectCategory(const StickerCategoryRef& category);
    
    /// Register for sticker selected events
    void addStickerSelectedEventListener(const StickerSelectedCallback& callback);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(StickerSelector);
};

NS_AZ_CHAT_END

#endif
