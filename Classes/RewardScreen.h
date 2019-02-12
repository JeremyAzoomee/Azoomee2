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

NS_AZOOMEE_BEGIN

class RewardScreenDelegate
{
public:
	virtual void onAnimationComplete(const RewardItemRef& reward) = 0;
};

// layers that extend RewardScreen will be used to diaplay reward animations on the notifications node, set by Director::getInstance()->setNotificationNode(Node);
// notification nodes exists above all normal scene layers, so isnt impacted by scene transitions, allowing it to work with the async nature of the rewards mechanic.
// Notification nodes are rendered outside of the normal pipeline, and thus dont have the normal matrix stack applied, this class scales the node to work like normal.
class RewardScreen : public cocos2d::Node
{
	typedef cocos2d::Node Super;
private:
	RewardScreenDelegate* _delegate = nullptr;
	
protected:
	float _duration = 0;
	RewardItemRef _rewardData = nullptr;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float deltaT) override;
	
	void setDeleagte(RewardScreenDelegate* delegate);
	void setDuration(float duration);
	void setRewardData(const RewardItemRef& reward);
	
	CREATE_FUNC(RewardScreen);
};

NS_AZOOMEE_END

#endif /* RewardScreen_h */
