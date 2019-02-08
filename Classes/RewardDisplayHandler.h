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
#include "RewardScreen.h"

NS_AZOOMEE_BEGIN

class RewardDisplayHandler : public RewardCallbackDelegate, RewardScreenDelegate
{
private:
	bool _rewardDisplayRunning = false;
	
	std::queue<std::string> _rewardQueue;
	
	RewardDisplayHandler();
	
	void showReward(const std::string& reward);
	
	
public:
	static RewardDisplayHandler* getInstance();
	virtual ~RewardDisplayHandler();
	
	//delagate functions
	void onRewardSuccess(const std::string& responseData);
	void onAnimationComplete();
};

NS_AZOOMEE_END

#endif /* RewardDisplayHandler_h */
