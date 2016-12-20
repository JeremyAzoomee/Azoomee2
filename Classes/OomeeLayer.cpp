#include "OomeeLayer.h"
#include "SimpleAudioEngine.h"
#include "extensions/cocos-ext.h"
#include "spine/spine.h"

USING_NS_CC;

using namespace spine;

Scene* OomeeLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = OomeeLayer::create();
    scene->addChild(layer);

    return scene;
}

bool OomeeLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto oomee = addOomeeToScreen();
    addListenerToOomee(oomee);
    
    return true;
}

spine::SkeletonAnimation* OomeeLayer::addOomeeToScreen()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    SkeletonAnimation *oomee = SkeletonAnimation::createWithFile("res/oomees/Pink.json", "res/oomees/Pink.atlas", 0.6f);
    oomee->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.38);
    oomee->setMix("Build_Dance_Wave", "Build_Float", 1);
    oomee->setAnimation(0, "Build_Simple_Wave", true);
    oomee->setScale(2);
    oomee->setOpacity(0);
    this->addChild(oomee);
    
    oomee->runAction(Sequence::create(DelayTime::create(8), FadeTo::create(0, 255), DelayTime::create(0.1), FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, 255), NULL));
    
    return oomee;
}

void OomeeLayer::addListenerToOomee(spine::SkeletonAnimation* toBeAddedTo)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch *touch, Event *event)
    {
        auto target = static_cast<SkeletonAnimation*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getBoundingBox().size;
        Rect rect = Rect(-s.width * 0.25, 0, s.width * 0.75, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            target->setAnimation(0, "Build_Glitch", false);
            target->addAnimation(0, "Build_Simple_Wave", true);
            
            return true;
        }
        
        return false;
        
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, toBeAddedTo);
}
