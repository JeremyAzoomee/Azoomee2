#include "NavigationControl.h"

// Quick testing using deeplinks/injection
#include "DeepLinkingSingleton.h"
#include "VideoPlaylistManager.h"
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "WebViewSelector.h"
#include "SceneManagerScene.h"

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
    
    _visualLayer = cocos2d::Layer::create();
    _visualLayer->retain();
    
    // Schedule updates
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->scheduleUpdate(this, 0, false);
}

NavigationControl::~NavigationControl()
{
    setKeypadEnabled(false);
    _visualLayer->release();
    
    // Unschedule updates
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->unscheduleUpdate(this);
}

void NavigationControl::reset()
{
    // Remove all navigations
    _navigationPoints.clear();
    _visualNavigationItems.clear();
    _currentPoint = nullptr;
    _visualLayer->removeAllChildren();
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
    bounds.origin = node->convertToWorldSpace(Vec2::ZERO);
    // bounds.origin.x += bounds.size.width * 0.5f;
    // bounds.origin.y += bounds.size.height * 0.5f;
    return bounds;
}

void NavigationControl::addNavigation(cocos2d::Node* node, NavigationEventCallback onAction)
{
    _navigationPoints[node] = onAction;
    NavigationItem* navItem = NavigationItem::create();
    navItem->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _visualNavigationItems[node] = navItem;
    _visualLayer->addChild(navItem);
    
    if(_currentPoint == nullptr)
    {
        _currentPoint = node;
        navItem->setHighlighted(true);
    }
}

void NavigationControl::removeNavigation(cocos2d::Node* node)
{
    auto pointIt = _navigationPoints.find(node);
    if(pointIt != _navigationPoints.end())
    {
        _navigationPoints.erase(pointIt);
    }
    
    auto visualIt = _visualNavigationItems.find(node);
    if(visualIt != _visualNavigationItems.end())
    {
        visualIt->second->removeFromParent();
        _visualNavigationItems.erase(visualIt);
    }
    
    if(node == _currentPoint)
    {
        _currentPoint = nullptr;
    }
}

void NavigationControl::highlightNavigation(cocos2d::Node* node)
{
    if(_currentPoint != nullptr)
    {
        _visualNavigationItems[_currentPoint]->setHighlighted(false);
    }
    if(node != nullptr && _navigationPoints.find(node) != _navigationPoints.end())
    {
        _currentPoint = node;
        _visualNavigationItems[_currentPoint]->setHighlighted(true);
        
        // TODO: If node is inside a scrollview, scroll it automatically into view
    }
}

cocos2d::Node* NavigationControl::getVisualLayer() const
{
    // TODO: Return nullptr if the current device doesn't require visual navigation
    return _visualLayer;
}

void NavigationControl::update(float dt)
{
    // Update element positions
    for(auto const& entry : _visualNavigationItems)
    {
        cocos2d::Node* node = entry.first;
        NavigationItem* nav = entry.second;
        const cocos2d::Rect& navPoint = NodeScreenSpaceBoundingBox(node);
        const cocos2d::Vec2& centerPos = cocos2d::Vec2(navPoint.getMidX(), navPoint.getMidY());
        nav->setPosition(centerPos);
        nav->setContentSize(navPoint.size);
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

// Return true if a < b
bool NavigationPointSortX( const std::pair<cocos2d::Node*, cocos2d::Vec2>& a,
                           const std::pair<cocos2d::Node*, cocos2d::Vec2>& b )
{
    const cocos2d::Vec2& posA = a.second;
    const cocos2d::Vec2& posB = b.second;
    return posA.x < posB.x;
}

// Return true if a < b
bool NavigationPointSortY( const std::pair<cocos2d::Node*, cocos2d::Vec2>& a,
                           const std::pair<cocos2d::Node*, cocos2d::Vec2>& b )
{
    const cocos2d::Vec2& posA = a.second;
    const cocos2d::Vec2& posB = b.second;
    return posA.y < posB.y;
}

// Return true if a < b
bool NavigationPointSortXY( const std::pair<cocos2d::Node*, cocos2d::Vec2>& a,
                            const std::pair<cocos2d::Node*, cocos2d::Vec2>& b )
{
    const cocos2d::Vec2& posA = a.second;
    const cocos2d::Vec2& posB = b.second;
    
    float diffX = posA.x < posB.x;
    if( diffX < 0.0f )
    {
        return true;
    }
    else if( diffX == 0.0f )
    {
        float diffY = posA.y < posB.y;
        return diffY < 0.0f;
    }
    else
    {
        return false;
    }
}

// Return true if a < b
bool NavigationPointSortYX( const std::pair<cocos2d::Node*, cocos2d::Vec2>& a,
                            const std::pair<cocos2d::Node*, cocos2d::Vec2>& b )
{
    const cocos2d::Vec2& posA = a.second;
    const cocos2d::Vec2& posB = b.second;
    
    float diffY = posA.y < posB.y;
    if( diffY < 0.0f )
    {
        return true;
    }
    else if( diffY == 0.0f )
    {
        float diffX = posA.x < posB.x;
        return diffX < 0.0f;
    }
    else
    {
        return false;
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
//            case cocos2d::EventKeyboard::KeyCode::KEY_BACK:
//            {
//                if(_navigationPoints.size() > 0)
//                {
//                    highlightNavigation(nullptr);
//                    _navigationPoints.clear();
//                    cocos2d::Director::getInstance()->replaceScene(SceneManagerScene::createScene(BaseWithNoHistory));
//                }
//                break;
//            }
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
                    // Find the next item based on the direction we want to go
                    cocos2d::Rect visibleBounds;
                    visibleBounds.origin = cocos2d::Director::getInstance()->getVisibleOrigin();
                    visibleBounds.size = cocos2d::Director::getInstance()->getVisibleSize();

                    const cocos2d::Rect& currentBounds = NodeScreenSpaceBoundingBox(_currentPoint);
                    const cocos2d::Vec2& currentPosition = cocos2d::Vec2(currentBounds.getMidX(), currentBounds.getMidY());
                    const float currentPositionV[2] = { currentPosition.x, currentPosition.y };
                    cocos2d::Node* closestNode = nullptr;
                    cocos2d::Vec2 closestPosition;
                    const int axis = ( keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DPAD_LEFT || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DPAD_RIGHT) ? 0 : 1;
                    const float dir = ( keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DPAD_LEFT || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DPAD_DOWN ) ? -1 : 1;
                    
                    cocos2d::log("NavigationControl: axis=%d, dir=%f", axis, dir);
                    
                    // Get potential items on x axis based on x < currentX
                    // Sort the items by x axis
                    // Sort the items by y axis?
                    
                    std::vector<std::pair<cocos2d::Node*, cocos2d::Vec2>> potentialPoints;
                    
                    // Find points we could potentially navigate to
                    for(auto it = _navigationPoints.begin(); it != _navigationPoints.end(); ++it)
                    {
                        if(it->first == _currentPoint)
                        {
                            continue;
                        }
                        
                        const cocos2d::Rect& bounds = NodeScreenSpaceBoundingBox(it->first);
                        const cocos2d::Vec2& maxPos = Vec2(bounds.getMaxX(), bounds.getMaxY());

                        // Ignore points outside of the visible screen
                        // TODO: In future we can remove this and auto scroll ScrollViews to move the content into view
                        if(!visibleBounds.containsPoint(bounds.origin) || !visibleBounds.containsPoint(maxPos))
                        {
                            continue;
                        }

                        const cocos2d::Vec2& pos = cocos2d::Vec2(bounds.getMidX(), bounds.getMidY());
                        const float posV[2] = { pos.x, pos.y };
                        
                        // Dir left/down (negative)
                        // 5 - 10 = -5, * -1 = 5
                        // 10 - 5 = 5, * -1 = -5
                        // Dir right/up (positive)
                        // 5 - 10 = -5, * 1 = -5
                        // 10 - 5 = 5, * 1 = 5
                        if((posV[axis] - currentPositionV[axis]) * dir >= 0)
                        {
                            cocos2d::Vec2 distance;
                            distance.x = fabs(pos.x - currentPosition.x);
                            distance.y = fabs(pos.y - currentPosition.y);
                            potentialPoints.push_back(std::pair<cocos2d::Node*, cocos2d::Vec2>(it->first, distance));
                        }
                    }
                    
                    if(potentialPoints.size() > 0)
                    {
                        // Sort on the other axis
                        if(axis == 0)
                        {
//                            std::stable_sort(potentialPoints.begin(), potentialPoints.end(), NavigationPointSortYX);
                            std::stable_sort(potentialPoints.begin(), potentialPoints.end(), NavigationPointSortX);
                            std::stable_sort(potentialPoints.begin(), potentialPoints.end(), NavigationPointSortY);
                        }
                        else
                        {
//                            std::stable_sort(potentialPoints.begin(), potentialPoints.end(), NavigationPointSortXY);
                            std::stable_sort(potentialPoints.begin(), potentialPoints.end(), NavigationPointSortY);
                            std::stable_sort(potentialPoints.begin(), potentialPoints.end(), NavigationPointSortX);
                        }
                        
                        for(int i = 0; i < potentialPoints.size(); ++i)
                        {
                            const cocos2d::Vec2& distance = potentialPoints[i].second;
                            cocos2d::log("NavigationControl: %f, %f", distance.x, distance.y);
                        }
                        
                        // Grab the first element
                        closestNode = potentialPoints[0].first;
                    }
                    
                    if(closestNode != nullptr)
                    {
                        highlightNavigation(closestNode);
                    }
                }
                break;
            }
            case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_CENTER:
            {
                if(_currentPoint != nullptr)
                {
                    auto item = _navigationPoints.find(_currentPoint);
                    item->second(_currentPoint);
                }
                break;
            }
            case cocos2d::EventKeyboard::KeyCode::KEY_MENU:
            {
                highlightNavigation(nullptr);
                _navigationPoints.clear();
                cocos2d::Director::getInstance()->replaceScene(SceneManagerScene::createScene(BaseWithNoHistory));
                break;
            }
        }
    }
}

NS_AZOOMEE_END
