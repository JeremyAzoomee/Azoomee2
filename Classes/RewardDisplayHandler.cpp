//
//  RewardDisplayHandler.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/02/2019.
//

#include "RewardDisplayHandler.h"
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "CoinCollectLayer.h"
#include "RewardScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<RewardDisplayHandler> sRewardDisplayHandlerSharedInstance;

RewardDisplayHandler* RewardDisplayHandler::getInstance()
{
	if (!sRewardDisplayHandlerSharedInstance.get())
	{
		sRewardDisplayHandlerSharedInstance.reset(new RewardDisplayHandler());
	}
	
	return sRewardDisplayHandlerSharedInstance.get();
}

RewardDisplayHandler::~RewardDisplayHandler(void)
{
	RewardCallbackHandler::getInstance()->setDelegate(nullptr);
}
RewardDisplayHandler::RewardDisplayHandler()
{
	RewardCallbackHandler::getInstance()->setDelegate(this);
}

void RewardDisplayHandler::showReward(const RewardItemRef& reward)
{
	if(reward->getItemPrice() < 0)
	{
		RewardScene* rewardScene = RewardScene::create();
		rewardScene->setDuration(8.0f);
		rewardScene->setRewardData(reward);
		rewardScene->setDeleagte(this);
		Director::getInstance()->setNotificationNode(rewardScene);
		//CoinCollectLayer* coinCollect = CoinCollectLayer::create();
		//coinCollect->setDuration(8.0f);
		//coinCollect->setRewardData(reward);
		//coinCollect->setOomeeFilepath(ChildDataProvider::getInstance()->getLoggedInChild()->getAvatar());
		//coinCollect->setDeleagte(this);
		//Director::getInstance()->setNotificationNode(coinCollect);
	}
}

bool RewardDisplayHandler::isRunningAnimationPossible()
{
	const auto& scene = Director::getInstance()->getRunningScene();
	if(!scene)
	{
		return false;
	}
	if(scene->getChildByName("iosWebView") || scene->getChildByName("androidWebView"))
	{
		return false;
	}
	return true;
}

void RewardDisplayHandler::getPendingRewards()
{
	HttpRequestCreator* request = API::GetPendingRewards(ChildDataProvider::getInstance()->getLoggedInChild()->getId(), this);
	request->execute();
}

//Delegate functions
void RewardDisplayHandler::onRewardSuccess(const RewardItemRef& reward)
{
	// add data to queue, if nothing playing, call show reward
	_rewardQueue.push(reward);
	if(!_rewardDisplayRunning)
	{
		if(isRunningAnimationPossible())
		{
			showReward(_rewardQueue.front());
			_rewardQueue.pop();
		}
	}
	
}
void RewardDisplayHandler::onAnimationComplete(const RewardItemRef& reward)
{
	HttpRequestCreator* request = API::RedeemReward(reward->getId(), nullptr);
	request->execute();
	
	if(_rewardQueue.size() > 0)
	{
		showReward(_rewardQueue.front());
		_rewardQueue.pop();
	}
	else
	{
		_rewardDisplayRunning = false;
		Node* rewardLayer = Director::getInstance()->getNotificationNode();
		rewardLayer->setCascadeOpacityEnabled(true);
		rewardLayer->runAction(Sequence::createWithTwoActions(FadeOut::create(1), CallFunc::create([reward](){
			Director::getInstance()->setNotificationNode(nullptr);
			ChildDataProvider::getInstance()->getLoggedInChild()->_coins -= reward->getItemPrice();
		})));
	}
}

void RewardDisplayHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	rapidjson::Document data;
	data.Parse(body.c_str());
	if(data.HasParseError())
	{
		return;
	}
	
	if(data.IsArray())
	{
		for(int i = 0; i < data.Size(); i++)
		{
			const rapidjson::Value& rewardData = data[i];
			RewardItemRef reward = RewardItem::createWithJson(rewardData);
			if(reward->getType() == "PENDING") // sanity check that parsing worked
			{
				onRewardSuccess(reward);
			}
		}
	}
	
}
void RewardDisplayHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	//do nothing
}

NS_AZOOMEE_END
