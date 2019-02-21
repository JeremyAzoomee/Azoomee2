//
//  RewardDisplayHandler.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/02/2019.
//

#include "RewardDisplayHandler.h"
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
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
	if(reward->getType() == "COIN")
	{
		RewardScene* rewardScene = RewardScene::create();
		rewardScene->setDuration(18.0f);
		rewardScene->setRewardData(reward);
		rewardScene->setDeleagte(this);
		Director::getInstance()->setNotificationNode(rewardScene);
		_rewardDisplayRunning = true;
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

void RewardDisplayHandler::addRewardToQueue(const RewardItemRef& reward)
{
	if(reward->getStatus() == "PENDING") // only add pending rewards
	{
		if(std::find_if(_rewardQueue.begin(), _rewardQueue.end(), [reward](const RewardItemRef& r){return r->getId() == reward->getId();}) != _rewardQueue.end())
		{
			return; // reward already in queue
		}
		
		if(reward->getType() == "COIN")
		{
			auto pos = std::find_if(_rewardQueue.begin(), _rewardQueue.end(), [](const RewardItemRef& r){return r->getType() == "COIN";});
			if(pos != _rewardQueue.end())
			{
				pos->get()->mergeRewards(reward);
			}
			else
			{
				_rewardQueue.push_back(reward);
			}
		}
		else
		{
			_rewardQueue.push_back(reward);
		}
	}
}

void RewardDisplayHandler::showNextReward()
{
	if(!_rewardDisplayRunning && isRunningAnimationPossible() && _rewardQueue.size() > 0)
	{
		showReward(_rewardQueue.front());
		_rewardQueue.erase(_rewardQueue.begin());
	}
}

//Delegate functions
void RewardDisplayHandler::onRewardSuccess(const RewardItemRef& reward)
{
	addRewardToQueue(reward);
	showNextReward();
	
}
void RewardDisplayHandler::onAnimationComplete(const RewardItemRef& reward)
{
	const std::vector<std::string>& ids = splitStringToVector(reward->getId(), ";");
	
	for(const std::string& id : ids)
	{
		HttpRequestCreator* request = API::RedeemReward(id, this);
		request->execute();
	}
	
	if(_rewardQueue.size() > 0)
	{
		showReward(_rewardQueue.front());
		_rewardQueue.erase(_rewardQueue.begin());
	}
	else
	{
		_rewardDisplayRunning = false;
		Node* rewardLayer = Director::getInstance()->getNotificationNode();
		rewardLayer->setCascadeOpacityEnabled(true);
		rewardLayer->runAction(Sequence::createWithTwoActions(FadeOut::create(1), CallFunc::create([reward](){
			Director::getInstance()->setNotificationNode(nullptr);
		})));
	}
}

void RewardDisplayHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagGetPendingRewards)
	{
		rapidjson::Document data;
		data.Parse(body.c_str());
		if(data.HasParseError())
		{
			return;
		}
		
		if(data.IsArray() && data.Size() > 0)
		{
			for(int i = 0; i < data.Size(); i++)
			{
				const rapidjson::Value& rewardData = data[i];
				RewardItemRef reward = RewardItem::createWithJson(rewardData);
				addRewardToQueue(reward);
			}
			showNextReward();
		}
	}
	else if(requestTag == API::TagRedeemReward)
	{
		HttpRequestCreator* getInvReq = API::GetInventory(ChildDataProvider::getInstance()->getLoggedInChild()->getId(), this);
		getInvReq->execute();
	}
	else if(requestTag == API::TagGetInventory)
	{
		ChildDataParser::getInstance()->parseChildInventory(body);
	}
	
}
void RewardDisplayHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	//do nothing
}

NS_AZOOMEE_END
