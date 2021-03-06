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
#include <AzoomeeCommon/Data/Rewards/RewardManager.h>
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
	if(reward->getType() == "COIN")
	{
        AnalyticsSingleton::getInstance()->rewardAnimBeginEvent(abs(reward->getItemPrice()));
        
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
    return scene != nullptr && !scene->getChildByName(ConfigStorage::kIosWebviewName) && !scene->getChildByName(ConfigStorage::kAndroidWebviewName);
}

bool RewardDisplayHandler::showNextReward()
{
	if(!_rewardDisplayRunning && isRunningAnimationPossible())
	{
        return popAndShowNextReward();
	}
    return false;
}

bool RewardDisplayHandler::popAndShowNextReward()
{
    if(RewardManager::getInstance()->pendingRewardNotificationCount() > 0)
    {
        const RewardItemRef& reward = RewardManager::getInstance()->popPendingRewardNotification();
        if(reward != nullptr)
        {
            showReward(reward);
            return true;
        }
    }
    return false;
}

//Delegate functions

void RewardDisplayHandler::onAnimationComplete(const RewardItemRef& reward)
{
    bool rewardShown = popAndShowNextReward();
    
    // No more rewards so we can exit
    if(!rewardShown)
    {
        _rewardDisplayRunning = false;
        Node* rewardLayer = Director::getInstance()->getNotificationNode();
        rewardLayer->setCascadeOpacityEnabled(true);
        rewardLayer->runAction(Sequence::createWithTwoActions(FadeOut::create(1), CallFunc::create([reward](){
            NotificationNodeDisplayManager::getInstance()->clearRewardLayer();
        })));
    }
}

NS_AZOOMEE_END
