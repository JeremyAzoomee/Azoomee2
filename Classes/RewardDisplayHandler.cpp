//
//  RewardDisplayHandler.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/02/2019.
//

#include "RewardDisplayHandler.h"
#include <AzoomeeCommon/API/RewardCallbackHandler.h>

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

void showReward(const std::string& reward)
{
	
}

//Delegate functions
void onRewardSuccess(const std::string& responseData)
{
	// add data to queue, if nothing playing, call show reward
}
void onAnimationComplete()
{
	// remove reward layer, if other in the queue, play next item
}

NS_AZOOMEE_END
