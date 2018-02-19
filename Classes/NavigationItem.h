#ifndef __AZOOMEE_NAVIGATION_ITEM_H__
#define __AZOOMEE_NAVIGATION_ITEM_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Azoomee.h>


NS_AZOOMEE_BEGIN

class NavigationItem : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    /// Is the item highlighted?
    bool _isHighlighted = false;
    /// Image to show when highlighted
    cocos2d::ui::ImageView* _highlightIcon = nullptr;
    
    NavigationItem();
    
public:
    
    static NavigationItem* create();
    virtual ~NavigationItem();
    
    /// Returns true if the item is highlighted
    bool isHighlighted() const;
    /// Change highlighted state
    void setHighlighted(bool highlight);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
};

NS_AZOOMEE_END

#endif
