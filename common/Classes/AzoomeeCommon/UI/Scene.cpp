#include "Scene.h"


using namespace cocos2d;



NS_AZOOMEE_BEGIN

bool Scene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    // Set the size of the scene to be the visible size, and move to origin
    // This way the content inside doesn't need to care about visible size, it can just fill the scene
    setPosition(cocos2d::Director::getInstance()->getVisibleOrigin());
    setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
  
    _initialised = true;
    return true;
}

void Scene::onEnter()
{
    Super::onEnter();
    
    // Since onSizeChanged won't have been called before the scene was entered
    // we call it here for convenience.
    onSizeChanged();
}

void Scene::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
    
    // Scene size changed, check for any Widgets
    // These don't get resized automatically (because cocos2d::Scene is not a ui::class)
    // So we do it here so it just works like magic
    const cocos2d::Vector<cocos2d::Node*>& children = getChildren();
    for(auto& child : children)
    {
        ui::Widget* widget = dynamic_cast<ui::Widget*>(child);
        if(widget)
        {
            widget->updateSizeAndPosition();
        }
    }
    
    if(_initialised)
    {
        onSizeChanged();
    }
}

void Scene::onSizeChanged()
{
    // Do nothing by default
}

NS_AZOOMEE_END
