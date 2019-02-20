//
//  RewardDisplayHandler.h
//  azoomee2
//
//  Created by Macauley on 08/02/2019.
//

#ifndef RewardDisplayHandler_h
#define RewardDisplayHandler_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/API/RewardCallbackHandler.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include "RewardScreen.h"

NS_AZOOMEE_BEGIN

class RewardDisplayHandler : public RewardCallbackDelegate, RewardScreenDelegate, HttpRequestCreatorResponseDelegate
{
private:
	bool _rewardDisplayRunning = false;
	
	std::vector<RewardItemRef> _rewardQueue;
	
	RewardDisplayHandler();
	
	void showReward(const RewardItemRef& reward);
	
	bool isRunningAnimationPossible();
	
public:
	static RewardDisplayHandler* getInstance();
	virtual ~RewardDisplayHandler();
	
	void getPendingRewards();
	
	//delagate functions
	void onRewardSuccess(const RewardItemRef& responseData);
	void onAnimationComplete(const RewardItemRef& reward);
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
	void onHttpRequestFailed(const std::string& requestTag, long errorCode);
};

NS_AZOOMEE_END

#endif /* RewardDisplayHandler_h */
