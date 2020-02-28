#ifndef TinizineCommon_SplitLayout_h
#define TinizineCommon_SplitLayout_h

#include "../Tinizine.h"
#include "ObservableLayout.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "UIConsts.h"

NS_TZ_BEGIN

/**
 * A custom Layout which displays two columns or rows.
 * Use the provided firstLayout() and secondLayout() to add
 * widgets to the correct section.
 * Note that this Layout sets up it's own Layout Type, you should
 * not change it, or for the first or second Layouts either.
 * You can however freely change LayoutParams as you would normally expect.
 */
class SplitLayout : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    
private:
    
    /// The first layout (left or top)
    ObservableLayout* _firstLayout;
    /// The second layout (right or bottom)
    ObservableLayout* _secondLayout;
    
    /// Behaviour of split
    float _layoutBehaviour[2] = { 0.5f, 0.5f };
    /// Current layout mode
    Orientation _orientation = Orientation::Landscape;
    
    /// True if we're currently resizing the layouts
    bool _resizingLayouts = false;
    
    
    /// Resize the first and second layouts
    void resizeLayouts();
    
    /// Called when the size of first or second layout changes
    void onChildLayoutSizeChanged(cocos2d::ui::Layout* layout);
    
protected:
    
    virtual void onSizeChanged() override;
    
public:
    
    const static float FixedSize;
    const static float FillSize;
    
    /// Set the mode to either Vertical or Horizontal (default)
    void setOrientation(Orientation orientation);
    /// Get the current mode
    Orientation orientation() const;
    
    /// Set the split behaviour, value is between 0 and 1, and represents the % of size to use.
    /// 0 indicates the layout will size to fill the remaining space.
    /// 0,0 is the same as 0.5,0.5
    /// Use a value of SplitLayout::FixedSize to indicate the layout has a custom size.
    /// When using SplitLayout::FixedSize, you must set the size of the layout yourself
    void setSplitBehaviour(float firstLayoutSize, float secondLayoutSize);
    
    /// Get the first layout. This will be the first column or row, depending on Mode
    cocos2d::ui::Layout* firstLayout() const;
    /// Get the second layout. This will be the second column or row, depending on Mode
    cocos2d::ui::Layout* secondLayout() const;
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    CREATE_FUNC(SplitLayout);
};

NS_TZ_END

#endif
