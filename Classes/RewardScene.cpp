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
	
	_passingTouchBlocker->setSwallowTouches(false);
	
	return true;
}

void RewardScene::onEnter()
{
	CoinChestLayer* coinChestLayer = CoinChestLayer::create();
	coinChestLayer->setDuration(_duration * 3.0f/7.0f);
	coinChestLayer->setRewardData(_rewardData);
	coinChestLayer->setDeleagte(this);
	coinChestLayer->retain();
	_screenSequence.push_back(coinChestLayer);
	
	AwesomeLayer* awesomeLayer = AwesomeLayer::create();
	awesomeLayer->setDuration(_duration * 2.0f/7.0f);
	awesomeLayer->setRewardData(_rewardData);
	awesomeLayer->setDeleagte(this);
	awesomeLayer->retain();
	_screenSequence.push_back(awesomeLayer);
	
	CoinCollectLayer* coinCollect = CoinCollectLayer::create();
	coinCollect->setDuration(_duration * 2.0f/7.0f);
	coinCollect->setRewardData(_rewardData);
	coinCollect->setOomeeFilepath(ChildDataProvider::getInstance()->getLoggedInChild()->getAvatar());
	coinCollect->setDeleagte(this);
	coinCollect->retain();
	_screenSequence.push_back(coinCollect);
	
	auto nextLayer = _screenSequence.back();
	this->addChild(nextLayer);
	nextLayer->release();
	
	Super::onEnter();
	stopActionByTag(kAutoCallbackActionTag); // stop auto callback action set in Super::onEnter()
}

void RewardScene::onExit()
{
	Super::onExit();
}

void RewardScene::onSizeChanged()
{
	Super::onSizeChanged();
	for(RewardScreen* screen : _screenSequence)
	{
		screen->onSizeChanged();
	}
}

void RewardScene::onAnimationComplete(const RewardItemRef& reward)
{
	removeAllChildren();
	_screenSequence.pop_back();
	if(_screenSequence.size() > 0)
	{
		auto nextLayer = _screenSequence.back();
		this->addChild(nextLayer);
		nextLayer->release();
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
