#include "NavigationLayer.h"

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
#include <AzoomeeCommon/Utils/ActionBuilder.h>
#include "FlowDataSingleton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN


int amountOfItems = 4;

bool NavigationLayer::init()
{
    if ( !Super::init() )
    {
        return false;
    }
    
    const std::vector<std::string>& hqNames = ConfigStorage::getInstance()->getHqNames();
    int numItems = (int)hqNames.size();
	
    this->setContentSize(Size(225.0f * numItems, 300));
    
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
        
        if(!HQHistoryManager::getInstance()->noHistory())
        {
            runDisplayAnimationForMenuItem(menuItemHolder, true);
        }
        else
        {
            runDisplayAnimationForMenuItem(menuItemHolder, false);        //Animation for two items has to be handled separately, because opacity must not be in a parent-child relationship.
        }
    }
    return true;
}

void NavigationLayer::changeToScene(const std::string& hqName, float duration)
{
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
#ifndef ALLOW_UNPAID_SIGNUP
		DynamicNodeHandler::getInstance()->startIAPFlow(context);
#else
		DynamicNodeHandler::getInstance()->startSignupFlow();
#endif
        return;
    }
    
    //IF THE BUTTON IS CHAT, START IT
    
    if(hqName == ConfigStorage::kChatHQName)
    {
		if(!ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChatEntryPointScene));
		}
		return;
    }
    
    HQHistoryManager::getInstance()->addHQToHistoryManager(hqName);
	Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
	
}

void NavigationLayer::onEnter()
{
    DeepLinkingSingleton::getInstance()->actionDeepLink();
    
    Super::onEnter();
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------

ui::Button* NavigationLayer::addMenuItemHolder(const std::string& hqName, float pos)
{
	const Color4B& colour = ConfigStorage::getInstance()->getColourForMenuItem(hqName);
	
	auto menuItemHolder = ui::Button::create("res/navigation/outer_circle.png");
    menuItemHolder->setName(hqName);
    menuItemHolder->setCascadeOpacityEnabled(true);
    menuItemHolder->setOpacity(0);
    menuItemHolder->setNormalizedPosition(Vec2(pos,0.5));
	menuItemHolder->setColor(Color3B(colour.r, colour.g, colour.b));
	menuItemHolder->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ui::Button* button = dynamic_cast<ui::Button*>(pSender);
			if(button && button->getChildByName("on")->getOpacity() < 255)
			{
				AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
				AnalyticsSingleton::getInstance()->navSelectionEvent("",button->getName());
				this->changeToScene(button->getName(), 0.5);
			}
		}
	});
    this->addChild(menuItemHolder);
    
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
	
	if(!ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
    {
        return; //not adding notifications in preview mode
    }
    
    ChatNotificationsSingleton::getInstance()->setNavigationLayer(this);
    ChatNotificationsSingleton::getInstance()->forceNotificationsUpdate();
}

void NavigationLayer::showNotificationBadge()
{
    if(!this->getChildByName(ConfigStorage::kChatHQName)->getChildByName("notification"))
    {
        return;
    }
    
    this->getChildByName(ConfigStorage::kChatHQName)->getChildByName("notification")->stopAllActions();
    this->getChildByName(ConfigStorage::kChatHQName)->getChildByName("notification")->runAction(EaseElasticOut::create(ScaleTo::create(1.0, 1.0)));
}

void NavigationLayer::hideNotificationBadge()
{
    this->getChildByName(ConfigStorage::kChatHQName)->getChildByName("notification")->stopAllActions();
    this->getChildByName(ConfigStorage::kChatHQName)->getChildByName("notification")->setScale(0.0);
}

//---------------LISTENERS------------------

void NavigationLayer::runDisplayAnimationForMenuItem(cocos2d::Node* node1, bool quick)
{
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
		auto button = this->getChildByName(hqName);
		if(button)
		{
        	button->getChildByName("circle")->setOpacity(255);
        	button->getChildByName("on")->stopAllActions();
        	button->getChildByName("on")->setOpacity(0);
		}
    }
}

void NavigationLayer::turnOnMenuItem(const std::string& hqName)
{
	auto button = this->getChildByName(hqName);
	if(button)
	{
    	button->getChildByName("on")->runAction(Sequence::create(FadeTo::create(0, 255), DelayTime::create(0.1), FadeTo::create(0,0), DelayTime::create(0.1), FadeTo::create(0, 255), NULL));
    	button->getChildByName("circle")->setOpacity(0);
	}
}

void NavigationLayer::delayedSetButtonOn(float dt)
{
    this->setButtonOn(ConfigStorage::kGameHQName);
}

void NavigationLayer::setButtonOn(const std::string& hqName)
{
	auto button = this->getChildByName(hqName);
	if(button)
	{
    	button->getChildByName("on")->setOpacity(255);
    	button->getChildByName("circle")->setOpacity(0);
	}
}

void NavigationLayer::onExit()
{
    ChatNotificationsSingleton::getInstance()->setNavigationLayer(NULL);
    Super::onExit();
}

NS_AZOOMEE_END
