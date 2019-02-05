//
//  RewardScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 30/01/2019.
//

#include "RewardScene.h"
#include "SceneManagerScene.h"
#include "CoinCollectLayer.h"
#include "AwesomeLayer.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

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
	int reward = RandomHelper::random_int(200, 1000);
	CoinCollectLayer* coinCollect = CoinCollectLayer::create();
	coinCollect->setAnimDuration(reward / 100);
	coinCollect->setOomeeFilepath(ChildDataProvider::getInstance()->getLoggedInChild()->getAvatar());
	coinCollect->setRewardAmount(reward);
	this->addChild(coinCollect);
	
	this->runAction(Sequence::create(DelayTime::create(15),CallFunc::create([this, coinCollect](){
		this->removeChild(coinCollect);
		AwesomeLayer* awesomeLayer = AwesomeLayer::create();
		this->addChild(awesomeLayer);
	}),DelayTime::create(10),CallFunc::create([](){
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
	}),NULL));
	
	Super::onEnter();
}

void RewardScene::setRewardValue(int value)
{
	_rewardValue = value;
}

NS_AZOOMEE_END
