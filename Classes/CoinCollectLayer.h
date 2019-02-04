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

NS_AZOOMEE_BEGIN

class CoinCollectLayer : public cocos2d::Node
{
	typedef cocos2d::Node Super;
private:
	int _rewardAmount;
	int _incPerSec;
	int _displayValue;
	
	float _animDuration;
	
	float _nextParticleEmit = 0;
	
	std::string _oomeeFilepath;
	
	void addBackground();
	void addHeading();
	void addPlinth();
	void addCoinCounter();
	
public:
	
	bool init() override;
	void onEnter() override;
	void update(float deltaT) override;
	
	void setRewardAmount(int rewardAmount);
	void setAnimDuration(float secs);
	void setOomeeFilepath(const std::string& oomeeFilepath);
	
	CREATE_FUNC(CoinCollectLayer);
};

NS_AZOOMEE_END


#endif /* CoinCollectLayer_h */
