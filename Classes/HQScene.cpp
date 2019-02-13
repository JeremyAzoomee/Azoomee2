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
	const Size& settingsButtonSize = _settingsButton->getContentSize();
	_settingsButton->setPosition(settingsButtonSize.width * 0.25f, this->getContentSize().height - settingsButtonSize.height * 1.25f);
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
	
	if(ContentHistoryManager::getInstance()->getReturnedFromContent())
	{
		const std::string& fakeData = "{\"id\": \"id\",\"userId\":  \"99999999-7848-46ce-b7d3-9999999999\",\"item\": {\"id\": \"ID1\",\"name\": \"test\",\"uri\": \"test\",\"type\": \"COIN\"},\"itemPrice\":" + StringUtils::format("%d",-RandomHelper::random_int(100, 500)) + " ,\"description\": \"Played Yeti\",\"status\": \"PENDING\"}";
		
		rapidjson::Document data;
		data.Parse(fakeData.c_str());
		
		RewardItemRef reward = RewardItem::createWithJson(data);
		RewardDisplayHandler::getInstance()->onRewardSuccess(reward);
		ContentHistoryManager::getInstance()->setReturnedFromContent(false);
	}
}

// Delegate Functions

void HQScene::onTutorialStateChanged(const std::string& stateId)
{
	
}

NS_AZOOMEE_END
