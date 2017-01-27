#include "NavigationLayer.h"
#include "SimpleAudioEngine.h"

#include "BaseScene.h"
#include "HQDataProvider.h"
#include "HQScene.h"

#include "ConfigStorage.h"
#include "ChildSelectorScene.h"
#include "ChildDataStorage.h"
#include "ChildDataProvider.h"
#include "ExitOrLogoutLayer.h"
#include "LoginScene.h"

USING_NS_CC;

int amountOfItems = 5;

Scene* NavigationLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = NavigationLayer::create();
    scene->addChild(layer);

    return scene;
}

bool NavigationLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    currentScene = 0;
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    //The menupoint strcuture is the following: menuItemImage is a white circle, always visible. menuItemInactive is the inactive image, visible when the menupoint is off. menuItemActive is the image that is visible when the menu is active.
    
    for(int i = 0; i <= amountOfItems; i++)
    {
        auto menuItemImage = addMenuItemImage(i);
        auto menuItemInactive = addMenuItemInactive(i, menuItemImage);          //Inactive menuItem is visible, when another menuItem is the selected one. The menu works as a set of radio buttons.
        addMenuItemActive(i, menuItemImage);                                    //Active menuItem is visible, when we are in the given menu
        addListenerToMenuItem(menuItemImage);
        runDisplayAnimationForMenuItem(menuItemImage, menuItemInactive);        //Animation for two items has to be handled separately, because opacity must not be in a parent-child relationship.
    }
    
    createSettingsButton();
    
    this->scheduleOnce(schedule_selector(NavigationLayer::delayedSetButtonOn), 3.5);
    
    return true;
}

void NavigationLayer::startLoadingGroupHQ(std::string uri)
{
    this->getParent()->getChildByName("contentLayer")->stopAllActions();
    this->getParent()->getChildByName("contentLayer")->runAction(Sequence::create(EaseInOut::create(MoveTo::create(0.5, ConfigStorage::getInstance()->getTargetPositionForMove(6)), 2), DelayTime::create(0.5), NULL));
    
    moveMenuPointsToHorizontalState();
    addBackButtonToNavigation();
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------
void NavigationLayer::loadArtsAppHQ()
{
    Scene *runningScene = Director::getInstance()->getRunningScene();
    Node *baseLayer = runningScene->getChildByName("baseLayer");
    Node *contentLayer = baseLayer->getChildByName("contentLayer");
    HQScene *hqLayer = (HQScene *)contentLayer->getChildByName("ARTS APP");
    
    hqLayer->startBuildingScrollViewBasedOnName();
}

void NavigationLayer::startLoadingHQScene(int categoryTag)
{
    if(categoryTag == 4)
    {
        auto funcCallAction = CallFunc::create([=](){
            this->loadArtsAppHQ();
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));

        return;
    }
    
    auto funcCallAction2 = CallFunc::create([=](){
        HQDataProvider::getInstance()->getDataForHQ(ConfigStorage::getInstance()->getNameForMenuItem(categoryTag));
    });
    this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction2, NULL));
}

void NavigationLayer::changeToScene(int target)
{
    currentScene = target;
    removeBackButtonFromNavigation();
    
    this->getParent()->getChildByName("contentLayer")->stopAllActions();
    this->getParent()->getChildByName("contentLayer")->runAction(Sequence::create(EaseInOut::create(MoveTo::create(0.5, ConfigStorage::getInstance()->getTargetPositionForMove(target)), 2), DelayTime::create(0.5), NULL));
    
    if((target != 0)&&(target != 3))
    {
        moveMenuPointsToHorizontalState();
    }
    else
    {
        moveMenuPointsToCircleState();
    }
}

Sprite* NavigationLayer::addMenuItemImage(int itemNumber)
{
    Color4B colour = ConfigStorage::getInstance()->getColourForMenuItem(itemNumber);
    Point position = ConfigStorage::getInstance()->getCirclePositionForMenuItem(itemNumber);
    
    auto menuItemImage = Sprite::create("res/navigation/outer_circle.png");
    menuItemImage->setTag(itemNumber);
    menuItemImage->setOpacity(0);
    menuItemImage->setTag(itemNumber);
    menuItemImage->setColor(Color3B(colour.r, colour.g, colour.b));
    menuItemImage->setPosition(position);
    this->addChild(menuItemImage);
    
    return menuItemImage;
}

Sprite* NavigationLayer::addMenuItemActive(int itemNumber, Node* toBeAddedTo)
{
    auto menuItemActive = Sprite::create(StringUtils::format("res/navigation/menu%d_on.png", itemNumber).c_str());
    menuItemActive->setName("on");
    menuItemActive->setOpacity(0);
    menuItemActive->setPosition(toBeAddedTo->getContentSize() / 2);
    toBeAddedTo->addChild(menuItemActive);
    
    return menuItemActive;
}

Sprite* NavigationLayer::addMenuItemInactive(int itemNumber, Node* toBeAddedTo)
{
    auto menuItemInactive = Sprite::create(StringUtils::format("res/navigation/menu%d.png", itemNumber).c_str());
    menuItemInactive->setName("off");
    menuItemInactive->setPosition(toBeAddedTo->getContentSize() / 2);
    menuItemInactive->setOpacity(0);
    toBeAddedTo->addChild(menuItemInactive);
    
    return menuItemInactive;
}

void NavigationLayer::createSettingsButton()
{
    auto settingsButton = ElectricDreamsButton::createSettingsButton(3.0f);
    settingsButton->setCenterPosition(Vec2(origin.x + visibleSize.width - settingsButton->getContentSize().width, origin.y + visibleSize.height - settingsButton->getContentSize().height));
    this->addChild(settingsButton);
}

void NavigationLayer::addListenerToMenuItem(cocos2d::Node *toBeAddedTo)
{
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
            if(target->getTag() == 3)
            {
                if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
                {
                    //Child Selection Button Pressed.
                    //Logout Child
                    ChildDataStorage::getInstance()->childLoggedIn = false;
                    auto childSelectorScene = ChildSelectorScene::createScene(0);
                    Director::getInstance()->replaceScene(childSelectorScene);
                }
                else
                {
                    auto loginScene = LoginScene::createScene(0);
                    Director::getInstance()->replaceScene(loginScene);
                }
            }
            else
            {
                this->startLoadingHQScene(target->getTag());
                this->turnOffAllMenuItems();
                target->getChildByName("on")->runAction(Sequence::create(FadeTo::create(0, 255), DelayTime::create(0.1), FadeTo::create(0,0), DelayTime::create(0.1), FadeTo::create(0, 255), NULL));
                this->changeToScene(target->getTag());
            }
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), toBeAddedTo);
}

void NavigationLayer::runDisplayAnimationForMenuItem(cocos2d::Node* node1, cocos2d::Node* node2)
{
    Color4B colour = ConfigStorage::getInstance()->getColourForMenuItem(node1->getTag());
    
    float randomDelay = RandomHelper::random_real(0.2, 0.7);
    node1->runAction(Sequence::create(DelayTime::create(3.0f + randomDelay), FadeTo::create(0, colour.a), DelayTime::create(0.1), FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, colour.a), NULL));
    node2->runAction(Sequence::create(DelayTime::create(3.0f + randomDelay), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
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

void NavigationLayer::moveMenuPointsToCircleState()
{
    for(int i = 0; i <= amountOfItems; i++)
    {
        auto menuItemImage = (Sprite *)this->getChildByTag(i);
        Point targetPosition = ConfigStorage::getInstance()->getCirclePositionForMenuItem(i);
        float delayTime = 0;
        
        menuItemImage->stopAction(menuItemImage->getActionByTag(1));
        
        auto sequence = Sequence::create(DelayTime::create(delayTime), EaseInOut::create(MoveTo::create(0.5, targetPosition), 2), NULL);
        sequence->setTag(1);
        
        menuItemImage->runAction(sequence);
    }
}

void NavigationLayer::moveMenuPointsToHorizontalState()
{
    for(int i = 0; i <= amountOfItems; i++)
    {
        auto menuItemImage = (Sprite *)this->getChildByTag(i);
        Point targetPosition = ConfigStorage::getInstance()->getHorizontalPositionForMenuItem(i);
        float delayTime = 0;
        
        menuItemImage->stopAction(menuItemImage->getActionByTag(1));
        
        auto sequence = Sequence::create(DelayTime::create(delayTime), EaseInOut::create(MoveTo::create(0.5, targetPosition), 2), NULL);
        sequence->setTag(1);
        
        menuItemImage->runAction(sequence);
    }
}

void NavigationLayer::addBackButtonToNavigation()
{
    auto backButtonImage = Sprite::create("res/hqscene/back_btn.png");
    backButtonImage->setPosition(250, 1650);
    backButtonImage->setOpacity(0);
    backButtonImage->setName("backButton");
    this->addChild(backButtonImage);
    
    backButtonImage->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    addListenerToBackButton(backButtonImage);
}

void NavigationLayer::removeBackButtonFromNavigation()
{
    this->removeChild(this->getChildByName("backButton"));
}

void NavigationLayer::addListenerToBackButton(Node* toBeAddedTo)
{
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
            Scene *runningScene = Director::getInstance()->getRunningScene();
            Node *baseLayer = runningScene->getChildByName("baseLayer");
            Node *contentLayer = baseLayer->getChildByName("contentLayer");
            HQScene *hqLayer = (HQScene *)contentLayer->getChildByName("GROUP HQ");
            
            hqLayer->removeAllChildren();
            
            this->changeToScene(currentScene);
            
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), toBeAddedTo);
}
