#include "NavigationLayer.h"

#include "BaseScene.h"
#include "HQDataProvider.h"
#include "HQScene.h"

#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include "PreviewLoginSignupMessageBox.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "LoginLogicHandler.h"
#include "SettingsButton.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "SceneManagerScene.h"
#include "DeepLinkingSingleton.h"
#include "BackEndCaller.h"

USING_NS_CC;
using namespace Azoomee;


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
    if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        ModalMessages::getInstance()->showMixpanelNotification();
        DeepLinkingSingleton::getInstance()->actionDeepLink();
    }
    
    AudioMixer::getInstance()->playOomeeIdleSounds(true);
    
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
        
        if(!HQHistoryManager::getInstance()->noHistory())
        {
            runDisplayAnimationForMenuItemQuick(menuItemImage, menuItemInactive);
        }
        else
        {
            runDisplayAnimationForMenuItem(menuItemImage, menuItemInactive);        //Animation for two items has to be handled separately, because opacity must not be in a parent-child relationship.
            this->scheduleOnce(schedule_selector(NavigationLayer::delayedSetButtonOn), 3.5);
        }
    }
    
    if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
        createSettingsButton();
    else
    {
        createPreviewLoginButton();
        createPreviewSignUpButton();
    }
    
    return true;
}

void NavigationLayer::startLoadingGroupHQ(std::string uri)
{
    HQHistoryManager::getInstance()->addHQToHistoryManager("GROUP HQ");
    
    this->getParent()->getChildByName("contentLayer")->stopAllActions();
    this->getParent()->getChildByName("contentLayer")->runAction(Sequence::create(EaseInOut::create(MoveTo::create(0.5, ConfigStorage::getInstance()->getTargetPositionForMove(6)), 2), DelayTime::create(0.5), NULL));
    
    moveMenuPointsToHorizontalStateInGroupHQ(0.5);
    turnOffAllMenuItems();
    addBackButtonToNavigation();
}

void NavigationLayer::changeToScene(int target, float duration)
{
    HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::getInstance()->getNameForMenuItem(target));
    
    cleanUpPreviousHQ();
    
    this->turnOffAllMenuItems();
    
    if(HQHistoryManager::getInstance()->getCurrentHQ() != "GROUP HQ")
    {
        this->turnOnMenuItem(target);
        removeBackButtonFromNavigation();
        
        Scene *runningScene = Director::getInstance()->getRunningScene();
        Node *baseLayer = runningScene->getChildByName("baseLayer");
        Node *contentLayer = baseLayer->getChildByName("contentLayer");
        HQScene *hqLayer = (HQScene *)contentLayer->getChildByName("GROUP HQ");
        
        hqLayer->removeAllChildren();
    }
    else
    {
        addBackButtonToNavigation();
    }
    
    this->getParent()->getChildByName("contentLayer")->stopAllActions();
    this->getParent()->getChildByName("contentLayer")->runAction(Sequence::create(EaseInOut::create(MoveTo::create(duration, ConfigStorage::getInstance()->getTargetPositionForMove(target)), 2), DelayTime::create(duration), NULL));
    
    
    switch (target) {
        case 0:
            moveMenuPointsToCircleState(duration);
            break;
        case 3:
            moveMenuPointsToCircleState(duration);
            break;
        case 6:
            moveMenuPointsToHorizontalStateInGroupHQ(duration);
            break;
            
        default:
            moveMenuPointsToHorizontalState(duration);
            break;
    }
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------
void NavigationLayer::loadArtsAppHQ()
{
    HQHistoryManager::getInstance()->addHQToHistoryManager("ARTS APP");
    
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
    settingsButton = SettingsButton::createSettingsButton(3.0f);
    settingsButton->setCenterPosition(Vec2(origin.x + visibleSize.width - settingsButton->getContentSize().width, origin.y + visibleSize.height - settingsButton->getContentSize().height));
    this->addChild(settingsButton);
}

void NavigationLayer::settingsButtonOffScreen()
{
    if(settingsButton)
        settingsButton->runAction(Sequence::create(EaseOut::create(MoveTo::create(1,Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height - settingsButton->getContentSize().height * 1.5)), 2), NULL));
}

void NavigationLayer::settingsButtonOnScreen()
{
    if(settingsButton)
        settingsButton->runAction(Sequence::create(EaseIn::create(MoveTo::create(1,Vec2(origin.x + visibleSize.width - settingsButton->getContentSize().width*1.5, origin.y + visibleSize.height - settingsButton->getContentSize().height * 1.5)), 2), NULL));
}

void NavigationLayer::createPreviewLoginButton()
{
    previewLoginButton = ElectricDreamsButton::createTextAsButton(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_IN_MULTILINE));
    previewLoginButton->setPosition(Vec2(origin.x+visibleSize.width + previewLoginButton->getContentSize().width + previewLoginButton->getContentSize().height/4, origin.y + visibleSize.height- previewLoginButton->getContentSize().height* 1.25));
    previewLoginButton->setDelegate(this);
    previewLoginButton->setMixPanelButtonName("PreviewLogin");
    this->addChild(previewLoginButton);
    
    previewLoginButton->runAction(Sequence::create(DelayTime::create(3), EaseOut::create(MoveTo::create(1, Vec2(origin.x+visibleSize.width - previewLoginButton->getContentSize().width - previewLoginButton->getContentSize().height/4, origin.y + visibleSize.height- previewLoginButton->getContentSize().height * 1.25)), 2), NULL));
}

void NavigationLayer::createPreviewSignUpButton()
{
    previewSignUpButton = ElectricDreamsButton::createTextAsButton(StringMgr::getInstance()->getStringForKey(BUTTON_SIGN_UP_MULTILINE));
    previewSignUpButton->setPosition(Vec2(origin.x - previewSignUpButton->getContentSize().width - previewSignUpButton->getContentSize().height/4, origin.y + visibleSize.height- previewSignUpButton->getContentSize().height * 1.25));
    previewSignUpButton->setDelegate(this);
    previewSignUpButton->setMixPanelButtonName("PreviewSignUp");
    this->addChild(previewSignUpButton);
    
    previewSignUpButton->runAction(Sequence::create(DelayTime::create(3), EaseOut::create(MoveTo::create(1, Vec2(origin.x + previewSignUpButton->getContentSize().height/4, origin.y + visibleSize.height- previewSignUpButton->getContentSize().height * 1.25)), 2), NULL));
}

void NavigationLayer::addListenerToMenuItem(cocos2d::Node *toBeAddedTo)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        if(Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("contentLayer")->getNumberOfRunningActions() > 0) return false;
        
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            if(target->getTag() == 3)
            {
                AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
                if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
                {
                    //Child Selection Button Pressed.
                    //Logout Child
                    ChildDataParser::getInstance()->setChildLoggedIn(false);
                    BackEndCaller::getInstance()->getAvailableChildren();
                }
                else
                {
                    PreviewLoginSignupMessageBox::create();
                }
            }
            else
            {
                AnalyticsSingleton::getInstance()->navSelectionEvent("",target->getTag());
                AudioMixer::getInstance()->playEffect(HQ_HUB_SELECTED_AUDIO_EFFECT);
                this->startLoadingHQScene(target->getTag());
                this->turnOffAllMenuItems();
                this->turnOnMenuItem(target->getTag());
                this->changeToScene(target->getTag(), 0.5);
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

void NavigationLayer::runDisplayAnimationForMenuItemQuick(cocos2d::Node* node1, cocos2d::Node* node2)
{
    Color4B colour = ConfigStorage::getInstance()->getColourForMenuItem(node1->getTag());
    
    node1->runAction(Sequence::create(DelayTime::create(0), FadeTo::create(0, colour.a), DelayTime::create(0), FadeTo::create(0, 0), DelayTime::create(0), FadeTo::create(0, colour.a), NULL));
    node2->runAction(Sequence::create(DelayTime::create(0), FadeIn::create(0), DelayTime::create(0), FadeOut::create(0), DelayTime::create(0), FadeIn::create(0), NULL));
}


void NavigationLayer::turnOffAllMenuItems()
{
    for(int i = 0; i <= amountOfItems; i++)
    {
        this->getChildByTag(i)->getChildByName("on")->stopAllActions();
        this->getChildByTag(i)->getChildByName("on")->setOpacity(0);
    }
}

void NavigationLayer::turnOnMenuItem(int tagNumber)
{
    this->getChildByTag(tagNumber)->getChildByName("on")->runAction(Sequence::create(FadeTo::create(0, 255), DelayTime::create(0.1), FadeTo::create(0,0), DelayTime::create(0.1), FadeTo::create(0, 255), NULL));
}

void NavigationLayer::delayedSetButtonOn(float dt)
{
    this->setButtonOn(3);
}

void NavigationLayer::setButtonOn(int i)
{
    this->getChildByTag(i)->getChildByName("on")->setOpacity(255);
}

void NavigationLayer::moveMenuPointsToCircleState(float duration)
{
    AudioMixer::getInstance()->playOomeeIdleSounds(true);
    for(int i = 0; i <= amountOfItems; i++)
    {
        auto menuItemImage = (Sprite *)this->getChildByTag(i);
        Point targetPosition = ConfigStorage::getInstance()->getCirclePositionForMenuItem(i);
        
        menuItemImage->stopAction(menuItemImage->getActionByTag(1));
        
        auto action = EaseInOut::create(MoveTo::create(duration, targetPosition), 2);
        action->setTag(1);
        
        menuItemImage->runAction(action);
    }
    settingsButtonOnScreen();
}

void NavigationLayer::moveMenuPointsToHorizontalState(float duration)
{
    AudioMixer::getInstance()->playOomeeIdleSounds(false);
    for(int i = 0; i <= amountOfItems; i++)
    {
        auto menuItemImage = (Sprite *)this->getChildByTag(i);
        Point targetPosition = ConfigStorage::getInstance()->getHorizontalPositionForMenuItem(i);
        
        menuItemImage->stopAction(menuItemImage->getActionByTag(1));
        
        auto action = EaseInOut::create(MoveTo::create(duration, targetPosition), 2);
        action->setTag(1);
        
        menuItemImage->runAction(action);
    }
    settingsButtonOffScreen();
}

void NavigationLayer::moveMenuPointsToHorizontalStateInGroupHQ(float duration)
{
    AudioMixer::getInstance()->playOomeeIdleSounds(false);
    for(int i = 0; i <= amountOfItems; i++)
    {
        auto menuItemImage = (Sprite *)this->getChildByTag(i);
        Point targetPosition = ConfigStorage::getInstance()->getHorizontalPositionForMenuItemInGroupHQ(i);
        
        menuItemImage->stopAction(menuItemImage->getActionByTag(1));
        
        auto action = EaseInOut::create(MoveTo::create(duration, targetPosition), 2);
        action->setTag(1);
        
        menuItemImage->runAction(action);
    }
    settingsButtonOffScreen();
}

void NavigationLayer::addBackButtonToNavigation()
{
    auto backButtonImage = Sprite::create("res/hqscene/back_btn.png");
    backButtonImage->setPosition(origin.x +backButtonImage->getContentSize().width*.7, origin.y + visibleSize.height - backButtonImage->getContentSize().height*.7);
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
        if(Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("contentLayer")->getNumberOfRunningActions() > 0) return false;
        
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("groupBackButton");
            AudioMixer::getInstance()->playEffect(BACK_BUTTON_AUDIO_EFFECT);
            Scene *runningScene = Director::getInstance()->getRunningScene();
            Node *baseLayer = runningScene->getChildByName("baseLayer");
            Node *contentLayer = baseLayer->getChildByName("contentLayer");
            
            HQHistoryManager::getInstance()->getHistoryLog();
            
            if(HQHistoryManager::getInstance()->getPreviousHQ() != "HOME")
            {
                
                HQScene *hqLayer2 = (HQScene *)contentLayer->getChildByName(HQHistoryManager::getInstance()->getPreviousHQ());
                
                auto funcCallAction = CallFunc::create([=](){
                    hqLayer2->startBuildingScrollViewBasedOnName();
                });
                
                this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
            }
            
            this->changeToScene(ConfigStorage::getInstance()->getTagNumberForMenuName(HQHistoryManager::getInstance()->getPreviousHQ()), 0.5);

            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), toBeAddedTo);
}

//-------------- DELEGATE FUNCTIONS ---------------

void NavigationLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == previewLoginButton)
    {
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
    else if(button == previewSignUpButton)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Onboarding));
    }
}

void NavigationLayer::cleanUpPreviousHQ()
{
    CCLOG("previous hq is: %s", HQHistoryManager::getInstance()->getPreviousHQ().c_str());
    std::string previousHqName = HQHistoryManager::getInstance()->getPreviousHQ();
    if(previousHqName != "HOME")
    {
        HQScene* lastHQLayer = (HQScene *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("contentLayer")->getChildByName(previousHqName);
        
        auto funcCallAction = CallFunc::create([=](){
            lastHQLayer->removeAllChildrenWithCleanup(true);
            Director::getInstance()->purgeCachedData();
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.0), funcCallAction, NULL));
        
    }
}
