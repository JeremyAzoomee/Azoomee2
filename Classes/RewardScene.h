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
#include "RewardScreen.h"

NS_AZOOMEE_BEGIN

class RewardScene : public RewardScreen, RewardScreenDelegate
{
	typedef RewardScreen Super;
private:
	std::vector<RewardScreen*> _screenSequence;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onSizeChanged() override;
	
	CREATE_FUNC(RewardScene);
	
	void onAnimationComplete(const RewardItemRef& reward) override;
};

NS_AZOOMEE_END

#endif /* RewardScene_h */
