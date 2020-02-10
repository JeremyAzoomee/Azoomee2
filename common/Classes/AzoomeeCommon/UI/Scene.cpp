#include "Scene.h"
#include "ModalWebview.h"

using namespace cocos2d;



NS_AZOOMEE_BEGIN

bool Scene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _contentLayer = ui::Layout::create();
    _contentLayer->setLayoutType(ui::Layout::Type::ABSOLUTE);
//    _contentLayer->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
//    _contentLayer->setBackGroundColor(Colours::Color_3B::white);
    addChild(_contentLayer);
    
    // Offset the content by the visible origin
    _contentLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _contentLayer->setPositionType(ui::Widget::PositionType::PERCENT);
    _contentLayer->setPositionPercent(Vec2(0.5f, 0.5f));
    const Size& sceneSize = Director::getInstance()->getWinSize();
    _contentLayer->setPosition(sceneSize * 0.5f);
    _contentLayer->setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
  
    return true;
}

void Scene::onEnter()
{
    _triggeringLayout = true;
    Super::onEnter();
    _triggeringLayout = false;
    
    // Trigger initial size change event to make sure elements are positioned correctly on enter
    onSizeChanged();
}

#pragma mark - Layout

void Scene::screenSizeDidChange()
{
    // Update scene content size to the size of the window.
    // This matches what cocos2d::Scene uses on creation, but it's not updated
    // automatically, so we do it here.
    const Size& sceneSize = Director::getInstance()->getWinSize();
    setContentSize(sceneSize);
    
    // Update the content layer so it sits inside and fills only the visible area of the screen
    const Size& contentSize = cocos2d::Director::getInstance()->getVisibleSize();
    if(_contentLayer)
    {
        _contentLayer->setContentSize(contentSize);
        _contentLayer->setPosition(sceneSize * 0.5f);
    }
    
    if(isRunning())
    {
        onSizeChanged();
    }
}

void Scene::screenSizeWillChange(float duration)
{
    // For now, don't animate any change until we can get it working nicely
    // TODO: Fix screenSizeWillChange animation
    screenSizeDidChange();
    return;
    
//    const Size& sceneSize = Director::getInstance()->getWinSize();
//    setContentSize(sceneSize);
//    
//    // Animate the change to the content
//    // Copy these values rather than keep ref - they will change in runAction
////    const Size sceneSizeStart = Super::getContentSize();
////    const Size sceneSizeTarget = Director::getInstance()->getWinSize();
//    const Size contentSizeStart = _contentLayer->getContentSize();
//    const Size contentSizeTarget = cocos2d::Director::getInstance()->getVisibleSize();
//    
//    runAction(ActionFloat::create(duration, 0.0f, 1.0f, [=](float dt) {
////        Size sceneSizeNext = sceneSizeStart + ((sceneSizeTarget - sceneSizeStart) * dt);
////        setContentSize(sceneSizeNext);
//        _contentLayer->setContentSize(contentSizeStart + ((contentSizeTarget - contentSizeStart) * dt));
//        _contentLayer->setPosition(sceneSize * 0.5f);
//    }));
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
    Super::setContentSize(contentSize);
}

void Scene::onSizeChanged()
{
    // Do nothing for the in herited scene by default
    // However, incase the scene has a MessageBox overlay, then trigger the
    // MessageBox onSizeChanged, if the orientation has changed.
	
	if(_contentLayer->getContentSize().width != Director::getInstance()->getVisibleSize().width || _contentLayer->getContentSize().height != Director::getInstance()->getVisibleSize().height)
	{
		screenSizeDidChange();
	}
	else
	{
		return;
	}
    
    ModalWebview* modalWebview = dynamic_cast<ModalWebview*>(this->getChildByName("ModalWebview"));
    if(modalWebview)
    {
        modalWebview->onSizeChanged();
    }
    
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
