//
//  RewardScreen.h
//  azoomee2
//
//  Created by Macauley on 08/02/2019.
//

#ifndef RewardScreen_h
#define RewardScreen_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "RewardItem.h"
#include <TinizineCommon/UI/NotificationNode.h>
#include <TinizineCommon/UI/ResizeNode.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

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

NS_AZ_END

#endif /* RewardScreen_h */
