//
//  RewardScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 30/01/2019.
//

#include "RewardScene.h"
#include "SceneManagerScene.h"
#include "CoinCollectLayer.h"
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
	CoinCollectLayer* coinCollect = CoinCollectLayer::create();
	coinCollect->setAnimDuration(8.0f);
	coinCollect->setOomeeFilepath(ChildDataProvider::getInstance()->getLoggedInChild()->getAvatar());
	coinCollect->setRewardAmount(RandomHelper::random_int(100, 1000));
	this->addChild(coinCollect);
	
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(15),CallFunc::create([](){
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
	})));
	
	Super::onEnter();
}

void RewardScene::setRewardValue(int value)
{
	_rewardValue = value;
}

NS_AZOOMEE_END
