#ifndef TinizineCommon_ObservableLayout_h
#define TinizineCommon_ObservableLayout_h

#include "../Tinizine.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_TZ_BEGIN

/**
 * Like a normal Layout, but you can observe changes to it's size.
 */
class ObservableLayout : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(cocos2d::ui::Layout*)> SizeChangedCallback;
private:
    
    /// Callback when the size of this composer changes
    SizeChangedCallback _sizeChangedCallback = nullptr;
    
protected:
    
    virtual void onSizeChanged() override;
    
public:
    
    /// Register for size change events
    void addSizeChangedEventListener(const SizeChangedCallback& callback);
    
    CREATE_FUNC(ObservableLayout);
    
};

NS_TZ_END

#endif
