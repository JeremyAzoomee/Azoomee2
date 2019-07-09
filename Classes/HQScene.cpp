//
//  HQScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 29/01/2019.
//

#include "HQScene.h"
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"

#include "DynamicNodeHandler.h"

#include <AzoomeeCommon/Utils/SpecialCalendarEventManager.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>

#include "FlowDataSingleton.h"
#include "ContentHistoryManager.h"
#include "RewardDisplayHandler.h"

#include "AgeGate.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool HQScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	buildCoreUI();

	return true;
}
void HQScene::onEnter()
{
	TutorialController::getInstance()->registerDelegate(this);
	if(TutorialController::getInstance()->isTutorialActive())
	{
		onTutorialStateChanged(TutorialController::getInstance()->getCurrentState());
	}
	_navLayer->setButtonOn(_hqCategory);
	
	_rewardRedeemedListener = EventListenerCustom::create(RewardDisplayHandler::kRewardRedeemedEventKey, [this](EventCustom* event){
		if(!_coinDisplay->isVisible())
		{
			_coinDisplay->setVisible(TutorialController::getInstance()->isTutorialCompleted(TutorialController::kFTUShopID) || ChildManager::getInstance()->getLoggedInChild()->getInventory()->getCoins() > 0);
		}
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_rewardRedeemedListener, this);
	
	Super::onEnter();
}

void HQScene::onExit()
{
	TutorialController::getInstance()->unRegisterDelegate(this);
	_eventDispatcher->removeEventListener(_rewardRedeemedListener);
	_rewardRedeemedListener = nullptr;
	Super::onExit();
}

void HQScene::onSizeChanged()
{
	Super::onSizeChanged();

	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
	
	_messagingLayer->setContentSize(Size(visibleSize.width, 350));
	
	_messagingLayer->repositionElements();
	
	_coinDisplay->setAnchorPoint(Vec2(1.2,(isIphoneX && isPortrait) ? 2.2f : 1.5f));
	_verticalScrollGradient->setScaleX(visibleSize.width / _verticalScrollGradient->getContentSize().width);
	
	DynamicNodeHandler::getInstance()->rebuildCurrentCTA();
	
	AgeGate* ageGate = dynamic_cast<AgeGate*>(getChildByName(AgeGate::kAgeGateLayerName));
	if(ageGate)
	{
		ageGate->onSizeChanged();
	}

}

void HQScene::setHQCategory(const std::string &hqCategory)
{
	_hqCategory = hqCategory;
}

HQSceneType HQScene::getSceneType() const
{
	return _type;
}

void HQScene::buildCoreUI()
{
	const Size& visibleSize = this->getContentSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
	
	// add coin counter
	_coinDisplay = CoinDisplay::create();
	_coinDisplay->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	_coinDisplay->setAnchorPoint(Vec2(1.2,(isIphoneX && isPortrait) ? 2.2f : 1.5f));
	_coinDisplay->setAnimate(true);
	this->addChild(_coinDisplay, 1);
	//show coin counter if they have coins or have completed the shop tutorial
	_coinDisplay->setVisible(TutorialController::getInstance()->isTutorialCompleted(TutorialController::kFTUShopID) || ChildManager::getInstance()->getLoggedInChild()->getInventory()->getCoins() > 0);
	
	_messagingLayer = UserTypeMessagingLayer::create();
	_messagingLayer->setContentSize(Size(visibleSize.width, 350));
	_messagingLayer->setPosition(-Vec2(0,350));
	_messagingLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	UserBillingType userType = UserBillingType::ANON;
	if(!ParentManager::getInstance()->isLoggedInParentAnonymous())
	{
		userType = UserBillingType::LAPSED;
		if(ParentManager::getInstance()->isPaidUser())
		{
			userType = UserBillingType::PAID;
		}
	}
	_messagingLayer->setUserType(userType);
	if(userType == UserBillingType::PAID)
	{
		_showingMessagingLayer = false;
		_messagingLayer->setOpacity(0);
	}
	else
	{
		if(HQHistoryManager::getInstance()->getHistorySize() == 1)
		{
			_messagingLayer->runAction(MoveTo::create(1, Vec2(0,0)));
		}
		else
		{
			_messagingLayer->setPosition(Vec2(0,0));
		}
	}
	this->addChild(_messagingLayer,1);
	
	_navLayer = NavigationLayer::create();
	_navLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
	_navLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_messagingLayer->addChild(_navLayer);
	
	addParticleElementsToBackground();
	
	_verticalScrollGradient = Sprite::create("res/decoration/TopNavGrad.png");
	_verticalScrollGradient->setAnchorPoint(Vec2(0.5, 0.9));
	_verticalScrollGradient->setScaleX(visibleSize.width / _verticalScrollGradient->getContentSize().width);
	_verticalScrollGradient->setColor(Color3B::BLACK);
	_verticalScrollGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
	_verticalScrollGradient->setRotation(180);
	_navLayer->addChild(_verticalScrollGradient);
	
	if(SpecialCalendarEventManager::getInstance()->isXmasTime())
	{
		addXmasDecoration();
	}
	
	if(HQHistoryManager::getInstance()->getHistorySize() == 1 || ContentHistoryManager::getInstance()->getReturnedFromContent())
	{
		AudioMixer::getInstance()->playBackgroundMusic(HQ_BACKGROUND_MUSIC);
	}
	ContentHistoryManager::getInstance()->setReturnedFromContent(false);
}

void HQScene::addParticleElementsToBackground()
{
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	auto myParticle = ParticleMeteor::create();
	
	if(SpecialCalendarEventManager::getInstance()->isXmasTime())
	{
		myParticle->setColor(cocos2d::Color3B::WHITE);
		myParticle->setStartColor(cocos2d::Color4F::WHITE);
		myParticle->setEndColor(cocos2d::Color4F::WHITE);
		myParticle->setLife(20.0f);
	}
	
	myParticle->setSpeed(30);
	myParticle->setGravity(Vec2(0, -20));
	myParticle->setScale(1);
	myParticle->setPosVar(Vec2(2732, 5192));
	this->addChild(myParticle, 0);
}

void HQScene::addXmasDecoration()
{
	Sprite* snow1 = Sprite::create("res/xmasdecoration/snowPileLeft.png");
	snow1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	snow1->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
	snow1->setScale(0);
	this->addChild(snow1, DECORATION_ZORDER);
	snow1->runAction(Sequence::create(DelayTime::create(0.3f), EaseOut::create(ScaleTo::create(2.0f, 1.0f), 2.0f), NULL));
	
	Sprite *snow2 = Sprite::create("res/xmasdecoration/snowPileRight.png");
	snow2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	snow2->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
	snow2->setScale(0);
	this->addChild(snow2, DECORATION_ZORDER);
	snow2->runAction(Sequence::create(DelayTime::create(0.5f), EaseOut::create(ScaleTo::create(2.0f, 1.0f), 2.0f), NULL));
}



// Delegate Functions

void HQScene::onTutorialStateChanged(const std::string& stateId)
{
	
}

NS_AZOOMEE_END
