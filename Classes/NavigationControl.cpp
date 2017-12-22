#include "NavigationControl.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<NavigationControl> sNavigationControlSharedInstance;

NavigationControl* NavigationControl::getInstance()
{
    if(!sNavigationControlSharedInstance.get())
    {
        sNavigationControlSharedInstance.reset(new NavigationControl());
    }
    return sNavigationControlSharedInstance.get();
}

NavigationControl::NavigationControl()
{
    setKeypadEnabled(true);
}

NavigationControl::~NavigationControl()
{
    setKeypadEnabled(false);
}

void NavigationControl::reset()
{
    // Remove all navigations
    _navigationPoints.clear();
    _currentPoint = nullptr;
}

std::string RectToString(const cocos2d::Rect& frame)
{
    std::stringstream str;
    str << frame.origin.x << "," << frame.origin.y << "," << frame.size.width << "," << frame.size.height;
    return str.str();
}

cocos2d::Rect NodeScreenSpaceBoundingBox(cocos2d::Node* node)
{
    cocos2d::Rect bounds;
    bounds.size = node->getContentSize();
    bounds.origin = node->convertToWorldSpace(Vec2::ZERO);// - cocos2d::Director::getInstance()->getVisibleOrigin();
    return bounds;
}

void NavigationControl::addNavigation(cocos2d::Node* node, NavigationEventCallback onAction)
{
    // TODO: Change NavigationEventCallback to use a safety Delegate (not implemented yet, but it's time to do it)
    _navigationPoints[node] = onAction;
    
    if(_currentPoint == nullptr)
    {
        _currentPoint = node;
    }
}

void NavigationControl::highlightNavigation(cocos2d::Node* node)
{
    if(_navigationPoints.find(node) != _navigationPoints.end())
    {
        _currentPoint = node;
        CCLOG("NavigationControl::highlightNavigation: %s", RectToString(NodeScreenSpaceBoundingBox(node)).c_str());
    }
}

#pragma mark - Keypad

const std::map<cocos2d::EventKeyboard::KeyCode, std::string> kKeyCodeNames = {
    { cocos2d::EventKeyboard::KeyCode::KEY_BACK, "KEY_BACK" },
    { cocos2d::EventKeyboard::KeyCode::KEY_DPAD_LEFT, "KEY_DPAD_LEFT" },
    { cocos2d::EventKeyboard::KeyCode::KEY_DPAD_RIGHT, "KEY_DPAD_RIGHT" },
    { cocos2d::EventKeyboard::KeyCode::KEY_DPAD_UP, "KEY_DPAD_UP" },
    { cocos2d::EventKeyboard::KeyCode::KEY_DPAD_DOWN, "KEY_DPAD_DOWN" },
    { cocos2d::EventKeyboard::KeyCode::KEY_DPAD_CENTER, "KEY_DPAD_CENTER" },
    { cocos2d::EventKeyboard::KeyCode::KEY_MENU, "KEY_MENU" },
};

void NavigationControl::setKeypadEnabled(bool enabled)
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    
    if(_keyboardListener != nullptr)
    {
        dispatcher->removeEventListener(_keyboardListener);
    }
    
    if(enabled)
    {
        auto listener = EventListenerKeyboard::create();
        listener->onKeyPressed = CC_CALLBACK_2(NavigationControl::onKeyPressed, this);
        listener->onKeyReleased = CC_CALLBACK_2(NavigationControl::onKeyReleased, this);
        
        dispatcher->addEventListenerWithFixedPriority(listener, -1);
        _keyboardListener = listener;
    }
}

void NavigationControl::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    auto it = kKeyCodeNames.find(keyCode);
    if(it != kKeyCodeNames.end())
    {
        CCLOG("NavigationControl::onKeyPressed: %s", it->second.c_str());
    }
    else
    {
        CCLOG("NavigationControl::onKeyPressed: %d", (int)keyCode);
    }
}

void NavigationControl::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    auto it = kKeyCodeNames.find(keyCode);
    if(it != kKeyCodeNames.end())
    {
        CCLOG("NavigationControl::onKeyReleased: %s", it->second.c_str());
    }
    else
    {
        CCLOG("NavigationControl::onKeyReleased: %d", (int)keyCode);
    }
    
    if(_navigationPoints.size() > 0)
    {
        switch(keyCode)
        {
            case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_LEFT:
            case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_RIGHT:
            case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_UP:
            case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_DOWN:
            {
                if(_currentPoint == nullptr)
                {
                    highlightNavigation(_navigationPoints.begin()->first);
                }
                else
                {
                    //const cocos2d::Vec2& position = NodeScreenSpaceBoundingBox(_currentPoint).origin;
                    // TODO: Find the next item based on the direction we want to go
                    auto it = _navigationPoints.find(_currentPoint);
                    ++it;
                    if(it == _navigationPoints.end())
                    {
                        it = _navigationPoints.begin();
                    }
                    
                    _currentPoint = it->first;
                }
                break;
            }
            case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_CENTER:
            {
                break;
            }
            case cocos2d::EventKeyboard::KeyCode::KEY_MENU:
            {
                cocos2d::Rect visibleArea;
                visibleArea.origin = cocos2d::Director::getInstance()->getVisibleOrigin();
                visibleArea.size = cocos2d::Director::getInstance()->getVisibleSize();
                CCLOG("NavigationControl::visibleArea: %s", RectToString(visibleArea).c_str());
                
                for(auto it : _navigationPoints)
                {
                    const cocos2d::Rect& navPoint = NodeScreenSpaceBoundingBox(it.first);
                    //if(visibleArea.intersectsRect(navPoint))
                    {
                        CCLOG("NavigationControl::navigation: %s", RectToString(navPoint).c_str());
                    }
                }
                break;
            }
        }
    }
}

NS_AZOOMEE_END
