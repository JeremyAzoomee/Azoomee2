//
//  HQScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 29/01/2019.
//

#include "HQSceneDepreciated.h"
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Utils/SpecialCalendarEventManager.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectManager.h>
#include "FlowDataSingleton.h"
#include "ContentHistoryManager.h"
#include "RewardDisplayHandler.h"

#include "AgeGate.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string HQSceneDepreciated::kTutHandName = "tutHand";

bool HQSceneDepreciated::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	buildCoreUI();

	return true;
}
void HQSceneDepreciated::onEnter()
{
	_navLayer->setButtonOn(_hqCategory);
	_rewardRedeemedListener = EventListenerCustom::create(RewardDisplayHandler::kRewardRedeemedEventKey, [this](EventCustom* event){
		if(!_coinDisplay->isVisible())
		{
			_coinDisplay->setVisible(TutorialController::getInstance()->isTutorialCompleted(TutorialController::kFTUShopID) || ChildManager::getInstance()->getLoggedInChild()->getInventory()->getCoins() > 0);
		}
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_rewardRedeemedListener, this);
	
	TutorialController::getInstance()->registerDelegate(this);
	if(TutorialController::getInstance()->isTutorialActive())
	{
		onTutorialStateChanged(TutorialController::getInstance()->getCurrentState());
	}
	ContentHistoryManager::getInstance()->setReturnedFromContent(false);
	Super::onEnter();
}

void HQSceneDepreciated::onExit()
{
	TutorialController::getInstance()->unRegisterDelegate(this);
	_eventDispatcher->removeEventListener(_rewardRedeemedListener);
	_rewardRedeemedListener = nullptr;
	Super::onExit();
}

void HQSceneDepreciated::onSizeChanged()
{
	Super::onSizeChanged();

	const Size& visibleSize = this->getContentSize();
	_isPortrait = visibleSize.width < visibleSize.height;
	bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
	
	_coinDisplay->setAnchorPoint(Vec2(1.2,(isIphoneX && _isPortrait) ? 2.2f : 1.5f));
	_verticalScrollGradient->setScaleX(visibleSize.width / _verticalScrollGradient->getContentSize().width);

}

void HQSceneDepreciated::setHQCategory(const std::string &hqCategory)
{
	_hqCategory = hqCategory;
}

HQSceneType HQSceneDepreciated::getSceneType() const
{
	return _type;
}

void HQSceneDepreciated::buildCoreUI()
{
	const Size& visibleSize = this->getContentSize();
	_isPortrait = visibleSize.width < visibleSize.height;
	bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
	
	// add coin counter
	_coinDisplay = CoinDisplay::create();
	_coinDisplay->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	_coinDisplay->setAnchorPoint(Vec2(1.2,(isIphoneX && _isPortrait) ? 2.2f : 1.5f));
	_coinDisplay->setAnimate(true);
	this->addChild(_coinDisplay, 1);
	//show coin counter if they have coins or have completed the shop tutorial
	_coinDisplay->setVisible(TutorialController::getInstance()->isTutorialCompleted(TutorialController::kFTUShopID) || ChildManager::getInstance()->getLoggedInChild()->getInventory()->getCoins() > 0);
	
	_navLayer = NavigationLayer::create();
	_navLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_navLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	this->addChild(_navLayer);
	
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

void HQSceneDepreciated::addParticleElementsToBackground()
{
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

void HQSceneDepreciated::addXmasDecoration()
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

void HQSceneDepreciated::onTutorialStateChanged(const std::string& stateId)
{
	
}

NS_AZOOMEE_END
