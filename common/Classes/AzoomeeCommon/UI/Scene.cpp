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
//    setPosition(cocos2d::Director::getInstance()->getVisibleOrigin());
//    setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
    
    _contentLayer = ui::Layout::create();
    _contentLayer->setLayoutType(ui::Layout::Type::ABSOLUTE);
//    _contentLayer->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
//    _contentLayer->setBackGroundColor(Style::Color::white);
    addChild(_contentLayer);
    
    // Offset the content by the visible origin
    _contentLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _contentLayer->setPositionType(ui::Widget::PositionType::PERCENT);
    _contentLayer->setPositionPercent(Vec2(0.5f, 0.5f));
    _contentLayer->setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
  
    return true;
}

void Scene::onEnter()
{
    _triggeringLayout = true;
    Super::onEnter();
    _triggeringLayout = false;
    
    
    // TODO: Get rid of this
    onSizeChanged();
}

#pragma mark - Layout

void Scene::updateSizeAndPosition()
{
    // Update scene content size to the size of the window.
    // This matches what cocos2d::Scene uses on creation, but it's not updated
    // automatically, so we do it here.
    const Size& sceneContentSize = Director::getInstance()->getWinSize();
    setContentSize(sceneContentSize);
    
    // Update the content layer so it sits inside and fills only the visible area of the screen
//    const Vec2& origin = cocos2d::Director::getInstance()->getVisibleOrigin();
//    _contentLayer->setPosition(origin * -1.0f);
    const Size& size = cocos2d::Director::getInstance()->getVisibleSize();
    _contentLayer->setContentSize(size);
    _contentLayer->updateSizeAndPosition(sceneContentSize);
    
    if(isRunning())
    {
        onSizeChanged();
    }
}

const cocos2d::Size& Scene::getContentSize() const
{
    if(_triggeringLayout)
    {
        return Super::getContentSize();
    }
    return _contentLayer->getContentSize();
}

void Scene::setContentSize(const cocos2d::Size& contentSize)
{
//    Size oldSize = getContentSize();
    Super::setContentSize(contentSize);
    
//    // Scene size changed, check for any Widgets
//    // These don't get resized automatically (because cocos2d::Scene is not a ui::class)
//    // So we do it here so it just works like magic
//    const cocos2d::Vector<cocos2d::Node*>& children = getChildren();
//    for(auto& child : children)
//    {
//        ui::Widget* widget = dynamic_cast<ui::Widget*>(child);
//        if(widget)
//        {
//            widget->updateSizeAndPosition();
//        }
//    }
    
//    if(_contentLayer)
//    {
//        if(oldSize.width != contentSize.width || oldSize.height != contentSize.height)
//        {
//            updateSizeAndPosition();
//            onSizeChanged();
//        }
//    }
}

void Scene::onSizeChanged()
{
    // Do nothing by default
}

void Scene::setLayoutType(cocos2d::ui::Layout::Type type)
{
    _contentLayer->setLayoutType(type);
}

cocos2d::ui::Layout::Type Scene::getLayoutType() const
{
    return _contentLayer->getLayoutType();
}

#pragma mark - Node override

void Scene::addChild(cocos2d::Node* child, int localZOrder, int tag)
{
    if(child == _contentLayer)
    {
        Super::addChild(child, localZOrder, tag);
        return;
    }
    _contentLayer->addChild(child, localZOrder, tag);
}

void Scene::addChild(cocos2d::Node* child, int localZOrder, const std::string& name)
{
    if(child == _contentLayer)
    {
        Super::addChild(child, localZOrder, name);
        return;
    }
    _contentLayer->addChild(child, localZOrder, name);
}

cocos2d::Node* Scene::getChildByTag(int tag) const
{
    return _contentLayer->getChildByTag(tag);
}

cocos2d::Node* Scene::getChildByName(const std::string& name) const
{
    return _contentLayer->getChildByName(name);
}

cocos2d::Vector<cocos2d::Node*>& Scene::getChildren()
{
    return _contentLayer->getChildren();
}

const cocos2d::Vector<cocos2d::Node*>& Scene::getChildren() const
{
    return _contentLayer->getChildren();
}

void Scene::removeChild(cocos2d::Node* child, bool cleanup)
{
    _contentLayer->removeChild(child, cleanup);
}

void Scene::removeAllChildrenWithCleanup(bool cleanup)
{
    _contentLayer->removeAllChildrenWithCleanup(cleanup);
}

void Scene::reorderChild(cocos2d::Node* child, int localZOrder)
{
    _contentLayer->reorderChild(child, localZOrder);
}

NS_AZOOMEE_END
