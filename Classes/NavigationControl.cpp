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
}

NavigationControl::~NavigationControl()
{
    setKeypadEnabled(false);
    _visualLayer->release();
}

void NavigationControl::reset()
{
    // Remove all navigations
    _navigationPoints.clear();
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
        const cocos2d::Rect& navPoint = NodeScreenSpaceBoundingBox(node);
        CCLOG("NavigationControl::highlightNavigation: %s", RectToString(navPoint).c_str());
        
        _visualLayer->removeAllChildren();
        cocos2d::LayerColor* nav = cocos2d::LayerColor::create(cocos2d::Color4B::RED, navPoint.size.width, navPoint.size.height);
        nav->setPosition(navPoint.origin);
        _visualLayer->addChild(nav);
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
            case cocos2d::EventKeyboard::KeyCode::KEY_BACK:
            {
                if(_navigationPoints.size() > 0)
                {
                    _currentPoint = nullptr;
                    _navigationPoints.clear();
                    cocos2d::Director::getInstance()->replaceScene(SceneManagerScene::createScene(BaseWithNoHistory));
                }
                break;
            }
            case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_LEFT:
            case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_RIGHT:
            //case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_UP:
            //case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_DOWN:
            {
                if(_currentPoint == nullptr)
                {
                    highlightNavigation(_navigationPoints.begin()->first);
                }
                else
                {
                    // Find the next item based on the direction we want to go
                    cocos2d::Node* newPoint = nullptr;
                    cocos2d::Vec2 targetPosition = NodeScreenSpaceBoundingBox(_currentPoint).origin;
                    cocos2d::Vec2 closestPosition;
                    for(auto it = _navigationPoints.begin(); it != _navigationPoints.end(); ++it)
                    {
                        const cocos2d::Vec2& newPos = NodeScreenSpaceBoundingBox(it->first).origin;
                        
                        if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DPAD_LEFT)
                        {
                            if(newPos.x < targetPosition.x)
                            {
                                if(newPoint == nullptr || newPos.x > closestPosition.x)
                                {
                                    newPoint = it->first;
                                    closestPosition = newPos;
                                }
                            }
                        }
                        else if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DPAD_RIGHT)
                        {
                            if(newPos.x > targetPosition.x)
                            {
                                if(newPoint == nullptr || newPos.x < closestPosition.x)
                                {
                                    newPoint = it->first;
                                    closestPosition = newPos;
                                }
                            }
                        }
                    }
                    
                    if(newPoint != nullptr)
                    {
                        _currentPoint = newPoint;
                        highlightNavigation(_currentPoint);
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
            case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_UP:
            {
                const std::string& contentID = "1156b02e-7267-42d2-b1c5-82d16de9ecbb"; // charlie and lola
                DeepLinkingSingleton::getInstance()->setDeepLink(DeepLinkingSingleton::kPostContentDeeplinkStr + contentID);
                break;
            }
            case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_DOWN:
            {
                auto playlist = HQDataObjectStorage::getInstance()->getHQDataObjectForKey("GROUP HQ")->getHqCarousels().at(0);
                VideoPlaylistManager::getInstance()->setPlaylist(playlist);
                
                auto webViewSelector = WebViewSelector::create();
                auto item = playlist->getContentItems()[0];
                
                // https://media.azoomee.com/distribution/gb/5796a19b-7126-477d-ae19-aec2aa790e5a/video_stream.m3u8
                webViewSelector->loadWebView(item->getUri().c_str(), Orientation::Landscape);
                break;
            }
            case cocos2d::EventKeyboard::KeyCode::KEY_MENU:
            {
                // game
                const std::string& contentID = "82786572-e240-4235-a79e-e4861d456c50";
                DeepLinkingSingleton::getInstance()->setDeepLink(DeepLinkingSingleton::kPostContentDeeplinkStr + contentID);
                
                // TODO
//                cocos2d::Rect visibleArea;
//                visibleArea.origin = cocos2d::Director::getInstance()->getVisibleOrigin();
//                visibleArea.size = cocos2d::Director::getInstance()->getVisibleSize();
//                CCLOG("NavigationControl::visibleArea: %s", RectToString(visibleArea).c_str());
//
//                for(auto it : _navigationPoints)
//                {
//                    const cocos2d::Rect& navPoint = NodeScreenSpaceBoundingBox(it.first);
//                    //if(visibleArea.intersectsRect(navPoint))
//                    {
//                        CCLOG("NavigationControl::navigation: %s", RectToString(navPoint).c_str());
//                    }
//                }
                break;
            }
        }
    }
}

NS_AZOOMEE_END
