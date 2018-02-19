#ifndef __AZOOMEE_NAVIGATION_CONTROL_H__
#define __AZOOMEE_NAVIGATION_CONTROL_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "NavigationItem.h"


NS_AZOOMEE_BEGIN

class NavigationControl
{
    typedef std::function<void(cocos2d::Node*)> NavigationEventCallback;
private:
    /// Navigation points
    std::map<cocos2d::Node*, NavigationEventCallback> _navigationPoints;
    cocos2d::Node* _currentPoint = nullptr;
    
    cocos2d::Layer* _visualLayer = nullptr;
    std::map<cocos2d::Node*, NavigationItem*> _visualNavigationItems;
    
    cocos2d::EventListener* _keyboardListener = nullptr;
    
    void setKeypadEnabled(bool enabled);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    NavigationControl();
    
public:
    
    static NavigationControl* getInstance();
    virtual ~NavigationControl();
    
    /// Called once per frame by cocos2d::Scheduler
    void update(float dt);
    
    /// Reset the navigation
    void reset();
    
    /// Add a navigation
    void addNavigation(cocos2d::Node* node, NavigationEventCallback onAction);
    
    /// Remove a navigation point
    void removeNavigation(cocos2d::Node* node);
    
    /// Hightlight a navigation
    void highlightNavigation(cocos2d::Node* node);
    
    /// Get the visual layer
    cocos2d::Node* getVisualLayer() const;
};

NS_AZOOMEE_END

#endif
