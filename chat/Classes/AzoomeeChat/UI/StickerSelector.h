#ifndef AzoomeeChat_StickerSelector_h
#define AzoomeeChat_StickerSelector_h

#include "../AzoomeeChat.h"
#include "StickerCategoryListView.h"
#include <AzoomeeCommon/UI/SplitLayout.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class StickerSelector : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    /// Split layout, holds tabs at the bottom and list uses remaining space
    SplitLayout* _splitLayout = nullptr;
    
    /// Category tabs
    StickerCategoryListView* _categoryListView = nullptr;
    
protected:
    
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the height of the tab bar
    void setTabBarHeight(float height);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(StickerSelector);
};

NS_AZOOMEE_CHAT_END

#endif
