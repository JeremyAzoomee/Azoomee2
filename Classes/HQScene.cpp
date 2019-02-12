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
#include "SettingsButton.h"

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
	/*_homeButton = ui::Button::create("res/navigation/back_button.png");
	_homeButton->setAnchorPoint(Vec2(-0.25, 1.25));
	_homeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_homeButton->addTouchEventListener([&](Ref* pSender, ui::Button::TouchEventType eType){
		if(eType == ui::Button::TouchEventType::ENDED)
		{
			HQHistoryManager::getInstance()->popHQ();
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
		}
	});
	this->addChild(_homeButton,1);*/
	
	SettingsButton* settingsButton = SettingsButton::create();
	const Size& settingsButtonSize = settingsButton->getContentSize();
	settingsButton->setPosition(settingsButtonSize.width * 0.25f, this->getContentSize().height - settingsButtonSize.height * 1.25f);
	this->addChild(settingsButton);
	
	// add coin counter
	CoinDisplay* coinDisplay = CoinDisplay::create();
	coinDisplay->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	coinDisplay->setAnchorPoint(Vec2(1.2,1.5));
	this->addChild(coinDisplay);
	
	_navLayer = NavigationLayer::create();
	_navLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_navLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	this->addChild(_navLayer);
	
	if(ContentHistoryManager::getInstance()->getReturnedFromContent())
	{
		const std::string& fakeData = "{\"id\": \"id\",\"userId\":  \"99999999-7848-46ce-b7d3-9999999999\",\"item\": {\"id\": \"ID1\",\"name\": \"test\",\"uri\": \"test\",\"type\": \"COIN\"},\"itemPrice\":" + StringUtils::format("%d",-RandomHelper::random_int(100, 500)) + " ,\"description\": \"Played Yeti\",\"status\": \"PENDING\"}";
		
		rapidjson::Document data;
		data.Parse(fakeData.c_str());
		
		RewardItemRef reward = RewardItem::createWithJson(data);
		RewardDisplayHandler::getInstance()->onRewardSuccess(reward);
	}
}

// Delegate Functions

void HQScene::onTutorialStateChanged(const std::string& stateId)
{
	
}

NS_AZOOMEE_END
