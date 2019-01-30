//
//  RewardScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 30/01/2019.
//

#include "RewardScene.h"
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

RewardScene* RewardScene::createWithValue(int rewardValue)
{
	RewardScene* scene = RewardScene::create();
	scene->setRewardValue(rewardValue);
	return scene;
}

bool RewardScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void RewardScene::onEnter()
{
	ui::Scale9Sprite* wires = ui::Scale9Sprite::create("res/rewards/wires.png");
	wires->setContentSize(this->getContentSize());
	wires->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	this->addChild(wires);
	
	Sprite* lights = Sprite::create("res/rewards/wow_bg.png");
	lights->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	lights->runAction(RepeatForever::create(Sequence::createWithTwoActions(RotateBy::create(1, 20), RotateBy::create(1, -20))));
	this->addChild(lights);
	
	Sprite* coins = Sprite::create("res/rewards/chest_of_coins.png");
	coins->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	this->addChild(coins);
	
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(10),CallFunc::create([](){
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
	})));
	
	Super::onEnter();
}

void RewardScene::setRewardValue(int value)
{
	_rewardValue = value;
}

NS_AZOOMEE_END
