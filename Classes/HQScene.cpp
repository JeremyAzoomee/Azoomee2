//
//  HQScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 29/01/2019.
//

#include "HQScene.h"
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"

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
	_homeButton = ui::Button::create("res/navigation/back_button.png");
	_homeButton->setAnchorPoint(Vec2(-0.25, 1.25));
	_homeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_homeButton->addTouchEventListener([&](Ref* pSender, ui::Button::TouchEventType eType){
		if(eType == ui::Button::TouchEventType::ENDED)
		{
			HQHistoryManager::getInstance()->popHQ();
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
		}
	});
	this->addChild(_homeButton,1);
	
	// add coin counter
	
	
}

// Delegate Functions

void HQScene::onTutorialStateChanged(const std::string& stateId)
{
	
}

NS_AZOOMEE_END
