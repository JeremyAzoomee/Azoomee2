#include "NavigationLayer.h"

#include "BaseScene.h"
#include "HQDataProvider.h"
#include "HQScene2.h"
#include "HQSceneArtsApp.h"

#include <AzoomeeCommon/Utils/SpecialCalendarEventManager.h>
#include <AzoomeeCommon/Data/Child/ChildDataStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "PreviewLoginSignupMessageBox.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "LoginLogicHandler.h"
#include "SettingsButton.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "SceneManagerScene.h"
#include "DeepLinkingSingleton.h"
#include "BackEndCaller.h"
#include "ChatNotificationsSingleton.h"
#include "DynamicNodeHandler.h"
#include "IAPFlowController.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "FlowDataSingleton.h"
#include <AzoomeeCommon/Utils/ActionBuilder.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN


int amountOfItems = 4;

cocos2d::Scene* NavigationLayer::createScene()
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
    }
    
    AudioMixer::getInstance()->playOomeeIdleSounds(true);
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Vec2(0,0);
    
    _navOffset = 0;
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        if(visibleSize.width < visibleSize.height)
        {
            _navOffset = 100;
        }
    }
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    const std::vector<std::string>& hqNames = ConfigStorage::getInstance()->getHqNames();
    int numItems = (int)hqNames.size();
    
    _hqButtonHolder = Node::create();
    _hqButtonHolder->setContentSize(Size(225.0f * numItems, 50));
    _hqButtonHolder->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height - 150 - _navOffset));
    _hqButtonHolder->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_hqButtonHolder);
    
    //The menupoint strcuture is the following: menuItemImage is a white circle, always visible. menuItemInactive is the inactive image, visible when the menupoint is off. menuItemActive is the image that is visible when the menu is active.
    
    for(int i = 0; i < numItems; i++)
    {
        const std::string& hqName = hqNames[i];
        auto menuItemHolder = addMenuItemHolder(hqName,(i + 0.5f)/numItems);
        addMenuItemCircle(hqName, menuItemHolder);
        if(hqName == ConfigStorage::kChatHQName)
        {
            addNotificationBadgeToChatIcon(menuItemHolder);
        }
        addMenuItemInactive(hqName, menuItemHolder);                                  //Inactive menuItem is visible, when another menuItem is the selected one. The menu works as a set of radio buttons.
        addMenuItemActive(hqName, menuItemHolder);                                    //Active menuItem is visible, when we are in the given menu
        
        if(SpecialCalendarEventManager::getInstance()->isXmasTime())
        {
            addXmasDecorationToMenuItem(hqName, menuItemHolder);
        }
        
        addListenerToMenuItem(menuItemHolder);
        
        if(!HQHistoryManager::getInstance()->noHistory())
        {
            runDisplayAnimationForMenuItem(menuItemHolder, true);
        }
        else
        {
            runDisplayAnimationForMenuItem(menuItemHolder, false);        //Animation for two items has to be handled separately, because opacity must not be in a parent-child relationship.
        }
    }
    
    _userTypeMessagingLayer = UserTypeMessagingLayer::create();
    _userTypeMessagingLayer->setContentSize(Size(visibleSize.width, 300));
    _userTypeMessagingLayer->setPosition(origin - Vec2(0,300));
    _userTypeMessagingLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    UserType userType = UserType::ANON;
    if(!ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
    {
        userType = UserType::LAPSED;
        if(ParentDataProvider::getInstance()->isPaidUser())
        {
            userType = UserType::PAID;
        }
    }
    _userTypeMessagingLayer->setUserType(userType);
    if(userType == UserType::PAID)
    {
        if(FlowDataSingleton::getInstance()->getDisplayUserPaidFlag())
        {
            FlowDataSingleton::getInstance()->setDisplayUserPaidFlag(false);
            _userTypeMessagingLayer->runAction(Sequence::create(MoveTo::create(1, origin), DelayTime::create(10), MoveTo::create(2, Vec2(0,-_userTypeMessagingLayer->getContentSize().height * 1.5f)), NULL));
        }
        else
        {
            _userTypeMessagingLayer->setVisible(false);
        }
    }
    else
    {
        _userTypeMessagingLayer->runAction(MoveTo::create(1, origin));
    }
    this->addChild(_userTypeMessagingLayer);
   
    
    
    if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        createTopObjects();
    }
    
    return true;
}

void NavigationLayer::startLoadingGroupHQ(std::string uri)
{
    HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kGroupHQName);
    
    this->getParent()->getChildByName(ConfigStorage::kContentLayerName)->stopAllActions();
    this->getParent()->getChildByName(ConfigStorage::kContentLayerName)->runAction(Sequence::create(EaseInOut::create(MoveTo::create(0.5, ConfigStorage::getInstance()->getTargetPositionForMove(ConfigStorage::kGroupHQName)), 2), DelayTime::create(0.5), NULL));
    
    moveMenuPointsToHorizontalStateInGroupHQ(0.5);
    turnOffAllMenuItems();
    addBackButtonToNavigation();
}

void NavigationLayer::changeToScene(const std::string& hqName, float duration)
{
    
    if(hqName == "OOMEE_MAKER")
    {
        if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(OomeeMakerEntryPointScene));
        }
        return;
    }
    
    //CHECK IF THE ENTITLEMENT FOR THAT SPECIFIC HQ IS ENABLED
    
    const HQDataObjectRef &currentObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName);
    
    if(!currentObject->getHqEntitlement())
    {
        AnalyticsSingleton::getInstance()->registerCTASource("lockedHQ","",currentObject->getHqType());
        IAPEntryContext context = IAPEntryContext::DEFAULT;
        if(hqName == ConfigStorage::kChatHQName)
        {
            context = IAPEntryContext::LOCKED_CHAT;
        }
        DynamicNodeHandler::getInstance()->startIAPFlow(context);
        return;
    }
    
    //IF THE BUTTON IS CHAT, START IT
    
    if(hqName == ConfigStorage::kChatHQName)
    {
        this->hideNotificationBadge();
        if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
        {
            AnalyticsSingleton::getInstance()->navSelectionEvent("",hqName);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
        }
        return;
    }
    
    HQHistoryManager::getInstance()->addHQToHistoryManager(hqName);
    cleanUpPreviousHQ();
    
    this->startLoadingHQScene(hqName);
    this->turnOffAllMenuItems();
    
    if(HQHistoryManager::getInstance()->getCurrentHQ() != ConfigStorage::kGroupHQName)
    {
        this->turnOnMenuItem(hqName);
        removeBackButtonFromNavigation();
        
        cocos2d::Scene *runningScene = Director::getInstance()->getRunningScene();
        Node *contentLayer = runningScene->getChildByName(ConfigStorage::kContentLayerName);
        HQScene2 *hqLayer = (HQScene2 *)contentLayer->getChildByName(ConfigStorage::kGroupHQName);
        
        hqLayer->removeAllChildren();
    }
    else
    {
        addBackButtonToNavigation();
    }
    
    this->getParent()->getChildByName(ConfigStorage::kContentLayerName)->stopAllActions();
    this->getParent()->getChildByName(ConfigStorage::kContentLayerName)->runAction(Sequence::create(EaseInOut::create(MoveTo::create(duration, ConfigStorage::getInstance()->getTargetPositionForMove(hqName)), 2), DelayTime::create(duration), NULL));
    
    
    if(hqName == ConfigStorage::kGroupHQName)
    {
        moveMenuPointsToHorizontalStateInGroupHQ(duration);
    }
    else
    {
        moveMenuPointsToHorizontalState(duration);
    }
}

void NavigationLayer::onEnter()
{
    DeepLinkingSingleton::getInstance()->actionDeepLink();
    
    Node::onEnter();
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------
void NavigationLayer::loadArtsAppHQ()
{
    HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kArtAppHQName);
    
    cocos2d::Scene *runningScene = Director::getInstance()->getRunningScene();
    Node *contentLayer = runningScene->getChildByName(ConfigStorage::kContentLayerName);
    HQScene2 *hqLayer = (HQScene2 *)contentLayer->getChildByName(ConfigStorage::kArtAppHQName);
    
    hqLayer->startBuildingScrollView();
}

void NavigationLayer::startLoadingHQScene(const std::string& hqName)
{
    if(hqName == ConfigStorage::kArtAppHQName)
    {
        auto funcCallAction = CallFunc::create([=](){
            this->loadArtsAppHQ();
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));

        return;
    }
    
    auto funcCallAction2 = CallFunc::create([=](){
        HQDataProvider::getInstance()->getDataForHQ(hqName);
    });
    this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction2, NULL));
}

Sprite* NavigationLayer::addMenuItemHolder(const std::string& hqName, float pos)
{
    auto menuItemHolder = Sprite::create();
    menuItemHolder->setName(hqName);
    menuItemHolder->setCascadeOpacityEnabled(true);
    menuItemHolder->setOpacity(0);
    menuItemHolder->setNormalizedPosition(Vec2(pos,0.5));
    _hqButtonHolder->addChild(menuItemHolder);
    
    return menuItemHolder;
}

Sprite* NavigationLayer::addMenuItemCircle(const std::string& hqName, Node* toBeAddedTo)
{
    Color4B colour = ConfigStorage::getInstance()->getColourForMenuItem(hqName);
    
    auto menuItemCircle = Sprite::create("res/navigation/outer_circle.png");
    toBeAddedTo->setContentSize(menuItemCircle->getContentSize());
    menuItemCircle->setName("circle");
    menuItemCircle->setOpacity(255);
    menuItemCircle->setColor(Color3B(colour.r, colour.g, colour.b));
    menuItemCircle->setPosition(toBeAddedTo->getContentSize() / 2);
    toBeAddedTo->addChild(menuItemCircle);
    
    return menuItemCircle;
}

Sprite* NavigationLayer::addMenuItemActive(const std::string& hqName, Node* toBeAddedTo)
{
    auto menuItemActive = Sprite::create(StringUtils::format("res/navigation/menu_%s_on.png", convertStringToAssetSafeString(hqName).c_str()).c_str());
    menuItemActive->setName("on");
    menuItemActive->setOpacity(0);
    menuItemActive->setPosition(toBeAddedTo->getContentSize() / 2);
    toBeAddedTo->addChild(menuItemActive);
    
    return menuItemActive;
}

Sprite* NavigationLayer::addMenuItemInactive(const std::string& hqName, Node* toBeAddedTo)
{
    auto menuItemInactive = Sprite::create(StringUtils::format("res/navigation/menu_%s.png", convertStringToAssetSafeString(hqName).c_str()).c_str());
    menuItemInactive->setName("off");
    menuItemInactive->setPosition(toBeAddedTo->getContentSize() / 2);
    menuItemInactive->setOpacity(255);
    toBeAddedTo->addChild(menuItemInactive);
    
    return menuItemInactive;
}

void NavigationLayer::addXmasDecorationToMenuItem(const std::string& hqName, cocos2d::Node *toBeAddedTo)
{
    cocos2d::Sprite* xmasDecor = Sprite::create(StringUtils::format("res/xmasdecoration/snow%d.png", RandomHelper::random_int(0, 5)));
    xmasDecor->setPosition(Vec2(toBeAddedTo->getContentSize().width / 2, toBeAddedTo->getContentSize().height));
    toBeAddedTo->addChild(xmasDecor, DECORATION_ZORDER);
}

void NavigationLayer::addNotificationBadgeToChatIcon(cocos2d::Node* chatIcon)
{
    auto notificationBadge = Sprite::create("res/navigation/chatAlert.png");
    notificationBadge->setName("notification");
    notificationBadge->setPosition(chatIcon->getContentSize().width * 0.85, chatIcon->getContentSize().height * 0.85);
    notificationBadge->setScale(0.0);
    chatIcon->addChild(notificationBadge, 9);
    
    if(!ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        return; //not adding notifications in preview mode
    }
    
    ChatNotificationsSingleton::getInstance()->setNavigationLayer(this);
    ChatNotificationsSingleton::getInstance()->forceNotificationsUpdate();
}

void NavigationLayer::showNotificationBadge()
{
    if(!_hqButtonHolder->getChildByName(ConfigStorage::kChatHQName)->getChildByName("notification"))
    {
        return;
    }
    
    _hqButtonHolder->getChildByName(ConfigStorage::kChatHQName)->getChildByName("notification")->stopAllActions();
    _hqButtonHolder->getChildByName(ConfigStorage::kChatHQName)->getChildByName("notification")->runAction(EaseElasticOut::create(ScaleTo::create(1.0, 1.0)));
}

void NavigationLayer::hideNotificationBadge()
{
    _hqButtonHolder->getChildByName(ConfigStorage::kChatHQName)->getChildByName("notification")->stopAllActions();
    _hqButtonHolder->getChildByName(ConfigStorage::kChatHQName)->getChildByName("notification")->setScale(0.0);
}

//------------------TOP LEVEL BUTTONS-------------------

void NavigationLayer::createTopObjects()
{
    settingsButton = SettingsButton::createSettingsButton(3.0f);
    const Size& settingsButtonSize = settingsButton->getContentSize();
    settingsButton->setPosition(origin.x + visibleSize.width, origin.y + visibleSize.height - settingsButtonSize.height * 1.25);
    this->addChild(settingsButton);

    returnToChildSelectorButton = ElectricDreamsButton::createChildSelectorButton();
    const Size& childSelectButtonSize = returnToChildSelectorButton->getContentSize();
    returnToChildSelectorButton->setPosition(Vec2(origin.x - childSelectButtonSize.width, origin.y + visibleSize.height - childSelectButtonSize.height*1.25));
    returnToChildSelectorButton->setDelegate(this);
    this->addChild(returnToChildSelectorButton);
    
    topObjectsOnScreen();
}

void NavigationLayer::topObjectsOffScreen()
{
    if(settingsButton)
    {
        const Size& settingsButtonSize = settingsButton->getContentSize();
        settingsButton->stopAllActions();
        settingsButton->runAction(Sequence::create(EaseOut::create(MoveTo::create(1,Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height - settingsButtonSize.height * 1.25)), 2), NULL));
    }
    
    if(returnToChildSelectorButton)
    {
        const Size& childSelectButtonSize = returnToChildSelectorButton->getContentSize();
        returnToChildSelectorButton->stopAllActions();
        returnToChildSelectorButton->runAction(Sequence::create(EaseOut::create(MoveTo::create(1,Vec2(origin.x - childSelectButtonSize.width, returnToChildSelectorButton->getPositionY())), 2), NULL));
    }
}

void NavigationLayer::topObjectsOnScreen()
{
    if(settingsButton)
    {
        const Size& settingsButtonSize = settingsButton->getContentSize();
        settingsButton->stopAllActions();
        settingsButton->runAction(Sequence::create(EaseIn::create(MoveTo::create(1,Vec2(origin.x + visibleSize.width - settingsButtonSize.width*1.25, origin.y + visibleSize.height - settingsButtonSize.height * 1.25)), 2), NULL));
    }
    
    if(returnToChildSelectorButton)
    {
        const Size& childSelectButtonSize = returnToChildSelectorButton->getContentSize();
        returnToChildSelectorButton->stopAllActions();
        returnToChildSelectorButton->runAction(Sequence::create(EaseIn::create(MoveTo::create(1,Vec2(origin.x + childSelectButtonSize.width*.25, visibleSize.height -  childSelectButtonSize.height * 1.25)), 2), NULL));
        
        
    }
}

//---------------LISTENERS------------------

void NavigationLayer::addListenerToMenuItem(cocos2d::Node *toBeAddedTo)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        if(Director::getInstance()->getRunningScene()->getChildByName(ConfigStorage::kContentLayerName)->getNumberOfRunningActions() > 0)
        {
            return false;
        }
        
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
            AnalyticsSingleton::getInstance()->navSelectionEvent("",target->getName());
            this->changeToScene(target->getName(), 0.5);
            
            return true;
        }
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), toBeAddedTo);
}

void NavigationLayer::runDisplayAnimationForMenuItem(cocos2d::Node* node1, bool quick)
{
    Color4B colour = ConfigStorage::getInstance()->getColourForMenuItem(node1->getName());
    
    float randomDelay = 0;
    float blinkDelay = 0.0;
    
    if(!quick)
    {
        randomDelay = RandomHelper::random_real(0.2, 0.7) + 3;
        blinkDelay = 0.1;
    }
    
    node1->runAction(createBlinkEffect(randomDelay, blinkDelay));
}


void NavigationLayer::turnOffAllMenuItems()
{
    
    const std::vector<std::string>& hqNames = ConfigStorage::getInstance()->getHqNames();
    
    for(const std::string& hqName : hqNames)
    {
        _hqButtonHolder->getChildByName(hqName)->getChildByName("circle")->setOpacity(255);
        _hqButtonHolder->getChildByName(hqName)->getChildByName("on")->stopAllActions();
        _hqButtonHolder->getChildByName(hqName)->getChildByName("on")->setOpacity(0);
    }
}

void NavigationLayer::turnOnMenuItem(const std::string& hqName)
{
    _hqButtonHolder->getChildByName(hqName)->getChildByName("on")->runAction(Sequence::create(FadeTo::create(0, 255), DelayTime::create(0.1), FadeTo::create(0,0), DelayTime::create(0.1), FadeTo::create(0, 255), NULL));
    _hqButtonHolder->getChildByName(hqName)->getChildByName("circle")->setOpacity(0);
}

void NavigationLayer::delayedSetButtonOn(float dt)
{
    this->setButtonOn(ConfigStorage::kGameHQName);
}

void NavigationLayer::setButtonOn(const std::string& hqName)
{
    _hqButtonHolder->getChildByName(hqName)->getChildByName("on")->setOpacity(255);
    _hqButtonHolder->getChildByName(hqName)->getChildByName("circle")->setOpacity(0);
}

void NavigationLayer::moveMenuPointsToHorizontalState(float duration)
{
    AudioMixer::getInstance()->playOomeeIdleSounds(false);
    
    auto action = EaseInOut::create(MoveTo::create(duration, Vec2(visibleSize.width / 2.0f, visibleSize.height - 150 - _navOffset)), 2);
    action->setTag(1);
        
    _hqButtonHolder->runAction(action);
    
    topObjectsOnScreen();
}

void NavigationLayer::moveMenuPointsToHorizontalStateInGroupHQ(float duration)
{
    AudioMixer::getInstance()->playOomeeIdleSounds(false);
    
    _hqButtonHolder->stopAction(_hqButtonHolder->getActionByTag(1));
        
    auto action = EaseInOut::create(MoveTo::create(duration, Vec2(visibleSize.width / 2.0f, visibleSize.height + 150)), 2);
    action->setTag(1);
        
    _hqButtonHolder->runAction(action);
    
    topObjectsOffScreen();
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
        if(Director::getInstance()->getRunningScene()->getChildByName(ConfigStorage::kContentLayerName)->getNumberOfRunningActions() > 0)
        {
            return false;
        }
        
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("groupBackButton");
            AudioMixer::getInstance()->playEffect(BACK_BUTTON_AUDIO_EFFECT);
            cocos2d::Scene *runningScene = Director::getInstance()->getRunningScene();
            Node *contentLayer = runningScene->getChildByName(ConfigStorage::kContentLayerName);
            
            HQHistoryManager::getInstance()->getHistoryLog();
            
            if(HQHistoryManager::getInstance()->getPreviousHQ() != ConfigStorage::kHomeHQName)
            {
                
                HQScene2 *hqLayer2 = (HQScene2 *)contentLayer->getChildByName(HQHistoryManager::getInstance()->getPreviousHQ());
                
                auto funcCallAction = CallFunc::create([=](){
                    hqLayer2->rebuildScrollView();
                });
                
                this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
            }

            this->changeToScene(HQHistoryManager::getInstance()->getPreviousHQ(), 0.5);

            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), toBeAddedTo);
}


//-------------- DELEGATE FUNCTIONS ---------------

void NavigationLayer::buttonPressed(ElectricDreamsButton* button)
{

    if(button == returnToChildSelectorButton)
    {
        ChildDataParser::getInstance()->setChildLoggedIn(false);
        BackEndCaller::getInstance()->getAvailableChildren();
    }
}

void NavigationLayer::cleanUpPreviousHQ()
{
    cocos2d::log("previous hq is: %s", HQHistoryManager::getInstance()->getPreviousHQ().c_str());
    const std::string& previousHqName = HQHistoryManager::getInstance()->getPreviousHQ();
    if(previousHqName != ConfigStorage::kHomeHQName)
    {
        HQScene2* lastHQLayer = (HQScene2 *)Director::getInstance()->getRunningScene()->getChildByName(ConfigStorage::kContentLayerName)->getChildByName(previousHqName);
        
        auto funcCallAction = CallFunc::create([=](){
            lastHQLayer->removeAllChildrenWithCleanup(true);
            Director::getInstance()->purgeCachedData();
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.0), funcCallAction, NULL));
        
    }
}

void NavigationLayer::onExit()
{
    ChatNotificationsSingleton::getInstance()->setNavigationLayer(NULL);
    Node::onExit();
}

void NavigationLayer::repositionElements()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Vec2(0,0);
    
    _userTypeMessagingLayer->setContentSize(Size(visibleSize.width, 300));
    _userTypeMessagingLayer->setPositionX(origin.x);
    
    _userTypeMessagingLayer->repositionElements();
    
    _navOffset = 0;
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        if(visibleSize.width < visibleSize.height)
        {
           _navOffset = 100;
        }
    }
    
    if(_hqButtonHolder)
    {
        if(HQHistoryManager::getInstance()->getCurrentHQ() == ConfigStorage::kGroupHQName)
        {
            _hqButtonHolder->setPosition(visibleSize.width / 2.0f, visibleSize.height + 150);
        }
        else
        {
            _hqButtonHolder->setPosition(visibleSize.width / 2.0f, visibleSize.height - 150 - _navOffset);
        }
    }
    
    if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        if(HQHistoryManager::getInstance()->getCurrentHQ() == ConfigStorage::kGroupHQName)
        {
            if(settingsButton)
            {
                const Size& settingsButtonSize = settingsButton->getContentSize();
                settingsButton->stopAllActions();
                settingsButton->setPosition(Vec2(origin.x + visibleSize.width + settingsButtonSize.width*1.25, origin.y + visibleSize.height - settingsButtonSize.height * 1.25));
            }
            
            if(returnToChildSelectorButton)
            {
                const Size& childSelectButtonSize = returnToChildSelectorButton->getContentSize();
                returnToChildSelectorButton->stopAllActions();
                returnToChildSelectorButton->setPosition(Vec2(origin.x - childSelectButtonSize.width*1.25, visibleSize.height - childSelectButtonSize.height * 1.25));
            }
            
            auto backButton = this->getChildByName("backButton");
            if(backButton)
            {
                const Size& backButtonSize = backButton->getContentSize();
                backButton->setPosition(origin.x + backButtonSize.width*.7, origin.y + visibleSize.height - backButtonSize.height*.7);
            }
            
        }
        else
        {
            if(settingsButton)
            {
                const Size& settingsButtonSize = settingsButton->getContentSize();
                settingsButton->stopAllActions();
                settingsButton->setPosition(Vec2(origin.x + visibleSize.width - settingsButtonSize.width*1.25, origin.y + visibleSize.height - settingsButtonSize.height * 1.25));
            }
            
            if(returnToChildSelectorButton)
            {
                const Size& childSelectButtonSize = returnToChildSelectorButton->getContentSize();
                returnToChildSelectorButton->stopAllActions();
                returnToChildSelectorButton->setPosition(Vec2(origin.x + childSelectButtonSize.width*.25, visibleSize.height -  childSelectButtonSize.height * 1.25));
            }
        }
    }
    else
    {
        if(HQHistoryManager::getInstance()->getCurrentHQ() == ConfigStorage::kGroupHQName)
        {
            auto backButton = this->getChildByName("backButton");
            if(backButton)
            {
                const Size& backButtonSize = backButton->getContentSize();
                backButton->setPosition(origin.x + backButtonSize.width*.7, origin.y + visibleSize.height - backButtonSize.height*.7);
            }
        }

    }
    
}

NS_AZOOMEE_END
