#include "OomeeLayer.h"
#include "SimpleAudioEngine.h"
#include "extensions/cocos-ext.h"
#include "spine/spine.h"
#include "ConfigStorage.h"
#include "ChildDataProvider.h"
#include "ParentDataProvider.h"

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
    
    std::string oomeeUrl = ParentDataProvider::getInstance()->getValueFromOneAvailableChild(ChildDataProvider::getInstance()->getLoggedInChildNumber(), "avatar");
    displayedOomeeNumber = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
    
    auto oomee = addOomeeToScreen();
    addTouchListenerToOomee(oomee);
    addCompleteListenerToOomee(oomee);
    
    return true;
}

void OomeeLayer::setDisplayedOomee(int oomeeNumber)
{
    displayedOomeeNumber = oomeeNumber;
}

//---------------------------------------------------------All methods beyond this line are called internally only-----------------------------------------------

spine::SkeletonAnimation* OomeeLayer::addOomeeToScreen()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    std::string oomeeName = ConfigStorage::getInstance()->getNameForOomee(displayedOomeeNumber);
    std::string jsonFileName = StringUtils::format("res/oomees/%s.json", oomeeName.c_str());
    std::string atlasFileName = StringUtils::format("res/oomees/%s.atlas", oomeeName.c_str());
    
    SkeletonAnimation *oomee = SkeletonAnimation::createWithFile(jsonFileName, atlasFileName, 0.6f);
    oomee->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.38);
    oomee->setAnimation(0, ConfigStorage::getInstance()->getRandomIdForAnimationType("idle").c_str(), false);
    oomee->setScale(2);
    oomee->setOpacity(0);
    this->addChild(oomee);
    
    oomee->runAction(Sequence::create(DelayTime::create(8), FadeTo::create(0, 255), DelayTime::create(0.1), FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, 255), NULL));
    
    return oomee;
}

void OomeeLayer::addTouchListenerToOomee(spine::SkeletonAnimation* toBeAddedTo)
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
            target->setAnimation(0, ConfigStorage::getInstance()->getRandomIdForAnimationType("touch").c_str(), false);
            
            return true;
        }
        
        return false;
        
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, toBeAddedTo);
}

void OomeeLayer::addCompleteListenerToOomee(spine::SkeletonAnimation* toBeAddedTo)
{
    auto oomeeAnimationComplete = [=] (int trackIdx, int loopCount)
    {
        toBeAddedTo->addAnimation(0, ConfigStorage::getInstance()->getRandomIdForAnimationType("idle").c_str(), false);
    };
    
    toBeAddedTo->setCompleteListener(oomeeAnimationComplete);
}
