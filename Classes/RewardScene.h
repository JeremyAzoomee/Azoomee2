//
//  RewardScene.h
//  azoomee2
//
//  Created by Macauley on 30/01/2019.
//

#ifndef RewardScene_h
#define RewardScene_h

#include <AzoomeeCommon/Tinizine.h>
#include <AzoomeeCommon/UI/Scene.h>
#include "RewardScreen.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class RewardScene : public RewardScreen, RewardScreenDelegate
{
	typedef RewardScreen Super;
private:
	std::vector<RewardScreen*> _screenSequence;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	CREATE_FUNC(RewardScene);
	
	void onAnimationComplete(const RewardItemRef& reward) override;
};

NS_AZ_END

#endif /* RewardScene_h */
