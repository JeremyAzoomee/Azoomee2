//
//  RewardDisplayHandler.h
//  azoomee2
//
//  Created by Macauley on 08/02/2019.
//

#ifndef RewardDisplayHandler_h
#define RewardDisplayHandler_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include "RewardScreen.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class RewardDisplayHandler : public RewardScreenDelegate
{
private:
	bool _rewardDisplayRunning = false;
	
	RewardDisplayHandler();
	
    /// Show the next reward. Returns false if no reward was shown
    bool popAndShowNextReward();
	void showReward(const RewardItemRef& reward);
	
	bool isRunningAnimationPossible();
	
public:
	
	static RewardDisplayHandler* getInstance();
	virtual ~RewardDisplayHandler();
	
    /// Show the next reward. Returns false if no reward was shown
	bool showNextReward();
	
	//delagate functions
	void onAnimationComplete(const RewardItemRef& reward);
};

NS_AZ_END

#endif /* RewardDisplayHandler_h */
