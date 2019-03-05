//
//  HQScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 29/01/2019.
//

#include "HQScene.h"
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
#include "CoinDisplay.h"

#include "DynamicNodeHandler.h"

#include <AzoomeeCommon/Utils/SpecialCalendarEventManager.h>

#include "ContentHistoryManager.h"
#include "RewardDisplayHandler.h"

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
	//RewardDisplayHandler::getInstance()->getPendingRewards();
	TutorialController::getInstance()->registerDelegate(this);
	if(TutorialController::getInstance()->isTutorialActive())
	{
		onTutorialStateChanged(TutorialController::getInstance()->getCurrentState());
	}
	_navLayer->setButtonOn(_hqCategory);
	Super::onEnter();
}

void HQScene::onExit()
{
	TutorialController::getInstance()->unRegisterDelegate(this);
	Super::onExit();
}

void HQScene::onSizeChanged()
{
	Super::onSizeChanged();
	
	DynamicNodeHandler::getInstance()->rebuildCurrentCTA();

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
	_settingsButton = SettingsButton::create();
	_settingsButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_settingsButton->setAnchorPoint(Vec2(-0.25,1.25));
	this->addChild(_settingsButton,1);
	
	// add coin counter
	CoinDisplay* coinDisplay = CoinDisplay::create();
	coinDisplay->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	coinDisplay->setAnchorPoint(Vec2(1.2,1.5));
	this->addChild(coinDisplay, 1);
	
	_navLayer = NavigationLayer::create();
	_navLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_navLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	this->addChild(_navLayer, 1);
	
	addParticleElementsToBackground();
	
	if(SpecialCalendarEventManager::getInstance()->isXmasTime())
	{
		addXmasDecoration();
	}

	
	ContentHistoryManager::getInstance()->setReturnedFromContent(false);
	/*
	const std::string& fakeData = "{\"id\": \"id\",\"userId\":  \"99999999-7848-46ce-b7d3-9999999999\",\"item\": {\"id\": \"ID1\",\"name\": \"test\",\"uri\": \"test\",\"type\": \"COIN\"},\"itemPrice\":" + StringUtils::format("%d",-RandomHelper::random_int(100, 500)) + " ,\"description\": \"Played Yeti\",\"status\": \"PENDING\"}";

	rapidjson::Document data;
	data.Parse(fakeData.c_str());
	
	RewardItemRef reward = RewardItem::createWithJson(data);
	RewardDisplayHandler::getInstance()->onRewardSuccess(reward);
	 */
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
