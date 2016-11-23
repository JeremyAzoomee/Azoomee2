#include "NavigationLayer.h"
#include "SimpleAudioEngine.h"

#include "BaseScene.h"

#include "InterSceneLoader.h"

USING_NS_CC;

int amountOfItems = 5;

Scene* NavigationLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = NavigationLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void NavigationLayer::turnOffAllMenuItems()
{
    for(int i = 0; i <= amountOfItems; i++)
    {
        this->getChildByTag(i)->getChildByName("on")->stopAllActions();
        this->getChildByTag(i)->getChildByName("on")->setOpacity(0);
    }
}

void NavigationLayer::delayedSetButtonOn(float dt)
{
    this->setButtonOn(0);
}

void NavigationLayer::setButtonOn(int i)
{
    this->getChildByTag(i)->getChildByName("on")->setOpacity(255);
}

void NavigationLayer::changeToScene(int target)
{
    std::vector<Vec2> menuPositions;
    
    menuPositions.push_back(Vec2(0,0));
    menuPositions.push_back(Vec2(-2732, 0));
    menuPositions.push_back(Vec2(2732, 0));
    menuPositions.push_back(Vec2(0, 0));
    menuPositions.push_back(Vec2(0, -2048));
    menuPositions.push_back(Vec2(0, 2048));
    
    this->getParent()->getChildByName("contentLayer")->stopAllActions();
    this->getParent()->getChildByName("contentLayer")->runAction(Sequence::create(DelayTime::create(0.5), EaseInOut::create(MoveTo::create(0, Vec2(menuPositions.at(target).x, menuPositions.at(target).y - 100)), 2), DelayTime::create(0.5), EaseInOut::create(MoveTo::create(0.5, Vec2(menuPositions.at(target).x, menuPositions.at(target).y)), 2), NULL));
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    //menupoints going up to line order
    for(int i = 0; i <= amountOfItems; i++)
    {
        auto menuItemImage = (Sprite *)this->getChildByTag(i);
        
        Vec2 travelPoint = circleMenuItemPositions.at(i);
        float delayTime = 0;
        
        if((target != 0)&&(target != 3))
        {
            delayTime = 0;
            travelPoint = horizontalMenuItemPositions.at(i);
        }
        
        menuItemImage->runAction(Sequence::create(DelayTime::create(delayTime), EaseInOut::create(MoveTo::create(0.5, travelPoint), 2), NULL));
        
    }
}

void NavigationLayer::startRotatingTheMenu()
{
    //The layer itself is going to be rotated.
    
    int repeat = 2;
    float rotateTime = 1;
    
    this->runAction(EaseOut::create(Repeat::create(RotateBy::create(rotateTime, 360), repeat), 10));
    this->schedule(schedule_selector(NavigationLayer::keepMenuPointsSideUp), 0, int(repeat * rotateTime * 60.0f), 0);
}

void NavigationLayer::keepMenuPointsSideUp(float dt)
{
    //When this function is scheduled, the holder layer is spinning. To keep the points always side up, we get the layer rotation, multiply it by -1, and set it to all menuItems.
    //It would possibly be nicer to stop calling this function when the rotation has finished, but it's not a huge overkill to have a few unnecessary calls depending on framerate.
    
    for(int i = 0; i <= amountOfItems; i++)
    {
        this->getChildByTag(i)->setRotation(this->getRotation() * -1);
    }
}

void NavigationLayer::addMenuItemPositionsAndColours()
{
    //Define menuitem positions (please note: previously these were calculated from an array filled in with several points of a circle calculated based on radius, using sin and cos, but less cpu to use raw data).
    circleMenuItemPositions.push_back(Point(1366, 1458));
    circleMenuItemPositions.push_back(Point(989, 1241));
    circleMenuItemPositions.push_back(Point(990, 805));
    circleMenuItemPositions.push_back(Point(1369, 589));
    circleMenuItemPositions.push_back(Point(1745, 810));
    circleMenuItemPositions.push_back(Point(1741, 1244));
    
    //Define menuitem positions when going to the secondary horizontal view
    horizontalMenuItemPositions.push_back(Point(775, 1675));
    horizontalMenuItemPositions.push_back(Point(1011, 1675));
    horizontalMenuItemPositions.push_back(Point(1248, 1675));
    horizontalMenuItemPositions.push_back(Point(1957, 1675));
    horizontalMenuItemPositions.push_back(Point(1721, 1675));
    horizontalMenuItemPositions.push_back(Point(1484, 1675));
    
    //Define menuitem outer ring colours
    
    menuItemColours.push_back(Vec4(0,0,0,0));
    menuItemColours.push_back(Vec4(248, 71, 89, 255));
    menuItemColours.push_back(Vec4(58, 188, 152, 255));
    menuItemColours.push_back(Vec4(0,0,0,0));
    menuItemColours.push_back(Vec4(246, 185, 66, 255));
    menuItemColours.push_back(Vec4(86, 177, 255, 255));
}

// on "init" you need to initialize your instance
bool NavigationLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    addMenuItemPositionsAndColours();
    
    
    //The menupoint strcuture is the following: menuItemImage is a white circle, always visible. menuItemInactive is the inactive image, visible when the menupoint is off. menuItemActive is the image that is visible when the menu is active.
    
    
    for(int i = 0; i <= amountOfItems; i++)
    {
        auto menuItemImage = Sprite::create("res/navigation/outer_circle.png");
        menuItemImage->setTag(i);
        menuItemImage->setOpacity(0);
        menuItemImage->setColor(Color3B(menuItemColours.at(i).x, menuItemColours.at(i).y, menuItemColours.at(i).z));
        menuItemImage->setPosition(circleMenuItemPositions.at(i));
        
        //adding inactive image as a child of the circle - easier to handle functions this way
        
        auto menuItemInactive = Sprite::create(StringUtils::format("res/navigation/menu%d.png", i).c_str());
        menuItemInactive->setName("off");
        menuItemInactive->setPosition(menuItemImage->getContentSize() / 2);
        menuItemInactive->setOpacity(0);
        menuItemImage->addChild(menuItemInactive);
        
        //add the on version of the sprite as a child of the off version
        
        auto menuItemActive = Sprite::create(StringUtils::format("res/navigation/menu%d_on.png", i).c_str());
        menuItemActive->setName("on");
        menuItemActive->setOpacity(0);
        menuItemActive->setPosition(menuItemImage->getContentSize() / 2);
        menuItemImage->addChild(menuItemActive);
        
        this->addChild(menuItemImage);
        
        //Cocos "feature": setting opacity for a layer with children does not affect the children's opacity. There is a setting that can be used to do so, but trying to use this really as a feature.
        
        float randomDelay = RandomHelper::random_real(0.2, 0.7);
        menuItemImage->runAction(Sequence::create(DelayTime::create(3.0f + randomDelay), FadeTo::create(0, menuItemColours.at(i).w), DelayTime::create(0.1), FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, menuItemColours.at(i).w), NULL));
        menuItemInactive->runAction(Sequence::create(DelayTime::create(3.0f + randomDelay), FadeTo::create(0, 255), DelayTime::create(0.1), FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, 255), NULL));
        
        //Adding the listener to the button, that is basically about changing the scene position on the screen. Later to be implemented much further.
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
        {
            auto target = static_cast<Sprite*>(event->getCurrentTarget());
            
            Point locationInNode = target->convertToNodeSpace(touch->getLocation());
            Size s = target->getContentSize();
            Rect rect = Rect(0,0,s.width, s.height);
            
            if(rect.containsPoint(locationInNode))
            {
                //Turn off all menuitems first
                NavigationLayer * myClass = (NavigationLayer *)target->getParent();
                myClass->turnOffAllMenuItems();
                
                //After all items are off, we turn on the one that was pressed.
                target->getChildByName("on")->runAction(Sequence::create(FadeTo::create(0, 255), DelayTime::create(0.1), FadeTo::create(0,0), DelayTime::create(0.1), FadeTo::create(0, 255), NULL));
                
                //And we change the main scene position depending on the pressed button.
                myClass->changeToScene(target->getTag());
                
                //New addition: while changing hide the scene with the loader scene
                InterSceneLoader *interSceneLoader = (InterSceneLoader *)this->getParent()->getChildByName("interSceneLoader");
                interSceneLoader->startLoading();
                
                return true;
            }
          
            return false;
        };
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), menuItemImage);
        
    }
    
    this->scheduleOnce(schedule_selector(NavigationLayer::delayedSetButtonOn), 3.5);
    this->startRotatingTheMenu();
    
    return true;
}
