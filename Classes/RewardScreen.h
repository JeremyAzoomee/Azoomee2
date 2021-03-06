//
//  RewardScreen.h
//  azoomee2
//
//  Created by Macauley on 08/02/2019.
//

#ifndef RewardScreen_h
#define RewardScreen_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/Rewards/RewardItem.h>
#include <AzoomeeCommon/UI/NotificationNode.h>
#include <AzoomeeCommon/UI/ResizeNode.h>

NS_AZOOMEE_BEGIN

class RewardScreenDelegate
{
public:
	virtual void onAnimationComplete(const RewardItemRef& reward) = 0;
};

class RewardScreen : public ResizeNode
{
	typedef ResizeNode Super;
protected:
	static const int kAutoCallbackActionTag;
	
	RewardScreenDelegate* _delegate = nullptr;
	
	float _duration = 0;
	RewardItemRef _rewardData = nullptr;
	
	cocos2d::EventListenerTouchOneByOne* _passingTouchBlocker = nullptr;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float deltaT) override;
	virtual void onSizeChanged() override;
	
	void setDeleagte(RewardScreenDelegate* delegate);
	void setDuration(float duration);
	void setRewardData(const RewardItemRef& reward);
	
	CREATE_FUNC(RewardScreen);
};

NS_AZOOMEE_END

#endif /* RewardScreen_h */
