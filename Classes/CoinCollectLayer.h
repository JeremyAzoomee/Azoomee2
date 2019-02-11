//
//  CoinCollectLayer.h
//  azoomee2
//
//  Created by Macauley on 04/02/2019.
//

#ifndef CoinCollectLayer_h
#define CoinCollectLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include "RewardScreen.h"

NS_AZOOMEE_BEGIN

class CoinCollectLayer : public RewardScreen
{
	typedef RewardScreen Super;
private:
	int _rewardAmount;
	float _incPerSec;
	float _displayValue;
	cocos2d::Label* _valueLabel = nullptr;
	
	cocos2d::Sprite* _plinth = nullptr;
	
	float _nextParticleEmit = 0;
	
	std::string _oomeeFilepath;
	
	cocos2d::EventListenerTouchOneByOne* _passingTouchBlocker = nullptr;;
	
	void addBackground();
	void addHeading();
	void addPlinth();
	void addCoinCounter();
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float deltaT) override;
	
	void setOomeeFilepath(const std::string& oomeeFilepath);
	
	CREATE_FUNC(CoinCollectLayer);
};

NS_AZOOMEE_END


#endif /* CoinCollectLayer_h */
