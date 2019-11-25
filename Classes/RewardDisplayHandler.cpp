//
//  RewardDisplayHandler.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/02/2019.
//

#include "RewardDisplayHandler.h"
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/NotificationNodeDisplayManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
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

RewardDisplayHandler::~RewardDisplayHandler()
{
}

RewardDisplayHandler::RewardDisplayHandler()
{
}

void RewardDisplayHandler::showReward(const RewardItemRef& reward)
{
	AnalyticsSingleton::getInstance()->rewardAnimBeginEvent(abs(reward->getItemPrice()));
	if(reward->getType() == "COIN")
	{
		RewardScene* rewardScene = RewardScene::create();
		rewardScene->setDuration(8.0f);
		rewardScene->setRewardData(reward);
		rewardScene->setDeleagte(this);
		NotificationNodeDisplayManager::getInstance()->addRewardNode(rewardScene);
		_rewardDisplayRunning = true;
	}
}

bool RewardDisplayHandler::isRunningAnimationPossible()
{
	const auto& scene = Director::getInstance()->getRunningScene();
	if(!scene)
	{
		return false;
	}
	if(scene->getChildByName(ConfigStorage::kIosWebviewName) || scene->getChildByName(ConfigStorage::kAndroidWebviewName))
	{
		return false;
	}
	return true;
}

void RewardDisplayHandler::getPendingRewards()
{
	HttpRequestCreator* request = API::GetPendingRewards(ChildManager::getInstance()->getLoggedInChild()->getId(), this);
	request->execute();
}

void RewardDisplayHandler::addRewardToQueue(const RewardItemRef& reward)
{
	if(reward->getStatus() == "PENDING") // only add pending rewards
	{
		if(std::find_if(_rewardQueue.begin(), _rewardQueue.end(), [reward](const RewardItemRef& r){return r->getId().find(reward->getId()) != r->getId().npos;}) != _rewardQueue.end()) // id of reward may be multiple reward ids joined together, so look for target id within entire string
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
		const auto& firstItemIt = _rewardQueue.begin();
		showReward(*firstItemIt);
		_rewardQueue.erase(firstItemIt);
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
        HttpRequestCreator* request = API::RedeemReward(id,
                                                        std::bind(&RewardDisplayHandler::onHttpRequestSuccess, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                                                        std::bind(&RewardDisplayHandler::onHttpRequestFailed, this, std::placeholders::_1, std::placeholders::_2));
		request->execute();
	}
	AnalyticsSingleton::getInstance()->rewardRedeemedEvent(abs(reward->getItemPrice()));
	
	if(_rewardQueue.size() > 0)
	{
		const auto& firstItemIt = _rewardQueue.begin();
		showReward(*firstItemIt);
		_rewardQueue.erase(firstItemIt);
	}
	else
	{
		_rewardDisplayRunning = false;
		Node* rewardLayer = Director::getInstance()->getNotificationNode();
		rewardLayer->setCascadeOpacityEnabled(true);
		rewardLayer->runAction(Sequence::createWithTwoActions(FadeOut::create(1), CallFunc::create([reward](){
			NotificationNodeDisplayManager::getInstance()->clearRewardLayer();
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
        ChildManager::getInstance()->updateInventory();
	}
}
void RewardDisplayHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	//do nothing
}

NS_AZOOMEE_END
