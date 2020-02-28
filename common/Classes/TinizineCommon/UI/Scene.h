#ifndef TinizineCommon_Scene_h
#define TinizineCommon_Scene_h

#include "../Tinizine.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

// Quite commonly used with Scene so included here for convenience
#include "Colour.h"
#include "LayoutParams.h"


NS_TZ_BEGIN

/**
 * Can be used instead of cocos2d::Scene to ensure the scene content size 
 * uses only he visible size of the screen.
 * Azoomee Scene also acts like a ui::Layout if you call setLayoutType.
 */
class Scene : public cocos2d::Scene
{
private:
    typedef cocos2d::Scene Super;
    
    /// Inner content layer we use to hold all the content
    cocos2d::ui::Layout* _contentLayer = nullptr;
    /// True when triggering layout
    bool _triggeringLayout = false;
    
protected:
    
    /// Called when the content size of this scene has changed
    virtual void onSizeChanged();
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    virtual const cocos2d::Size& getContentSize() const override;
    
    /// Screen size changed
    void screenSizeDidChange();
    /// The screen will change soon, please look forward to it
    void screenSizeWillChange(float duration);
    
    /// Like ui::Layout, set the layout type
    virtual void setLayoutType(cocos2d::ui::Layout::Type type);
    virtual cocos2d::ui::Layout::Type getLayoutType() const;
    
    using cocos2d::Node::addChild;
    virtual void addChild(cocos2d::Node* child, int localZOrder, int tag) override;
    virtual void addChild(cocos2d::Node* child, int localZOrder, const std::string& name) override;
    virtual cocos2d::Node* getChildByTag(int tag) const override;
    virtual cocos2d::Node* getChildByName(const std::string& name) const override;
    virtual cocos2d::Vector<cocos2d::Node*>& getChildren() override;
    virtual const cocos2d::Vector<cocos2d::Node*>& getChildren() const override;
    virtual void removeChild(cocos2d::Node* child, bool cleanup = true) override;
    virtual void removeAllChildrenWithCleanup(bool cleanup) override;
    virtual void reorderChild(cocos2d::Node* child, int localZOrder) override;
    
};

NS_TZ_END

#endif
