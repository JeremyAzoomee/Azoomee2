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
#include "CoinChestLayer.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool RewardScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_screenSequence = std::vector<RewardScreen*>();
	
	return true;
}

void RewardScene::onEnter()
{
	this->setScale(1);
	
	CoinChestLayer* coinChestLayer = CoinChestLayer::create();
	coinChestLayer->setDuration(2.5f);
	coinChestLayer->setRewardData(_rewardData);
	coinChestLayer->setDeleagte(this);
	coinChestLayer->retain();
	_screenSequence.push_back(coinChestLayer);
	
	AwesomeLayer* awesomeLayer = AwesomeLayer::create();
	awesomeLayer->setDuration(2.5f);
	awesomeLayer->setRewardData(_rewardData);
	awesomeLayer->setDeleagte(this);
	awesomeLayer->retain();
	_screenSequence.push_back(awesomeLayer);
	
	CoinCollectLayer* coinCollect = CoinCollectLayer::create();
	coinCollect->setDuration(4.0f);
	coinCollect->setRewardData(_rewardData);
	coinCollect->setOomeeFilepath(ChildDataProvider::getInstance()->getLoggedInChild()->getAvatar());
	coinCollect->setDeleagte(this);
	this->addChild(coinCollect);
	
	Node::onEnter(); // skip RewardScreen onEnter which schedules callback
}

void RewardScene::onAnimationComplete(const RewardItemRef& reward)
{
	removeAllChildren();
	if(_screenSequence.size() > 0)
	{
		auto nextLayer = _screenSequence.back();
		this->addChild(nextLayer);
		nextLayer->release();
		_screenSequence.pop_back();
	}
	else
	{
		if(this->_delegate)
		{
			this->_delegate->onAnimationComplete(_rewardData);
		}
	}
}

NS_AZOOMEE_END
