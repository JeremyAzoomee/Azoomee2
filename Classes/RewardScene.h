//
//  RewardScene.h
//  azoomee2
//
//  Created by Macauley on 30/01/2019.
//

#ifndef RewardScene_h
#define RewardScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>

NS_AZOOMEE_BEGIN

class RewardScene : public Azoomee::Scene
{
	typedef Azoomee::Scene Super;
private:
	int _rewardValue;
	
public:
	static RewardScene* createWithValue(int rewardValue);
	
	bool init() override;
	void onEnter() override;
	
	void setRewardValue(int value);
	
	CREATE_FUNC(RewardScene);
};

NS_AZOOMEE_END

#endif /* RewardScene_h */
