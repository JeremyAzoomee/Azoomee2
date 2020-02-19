//
//  CoinCollectLayer.h
//  azoomee2
//
//  Created by Macauley on 04/02/2019.
//

#ifndef CoinCollectLayer_h
#define CoinCollectLayer_h

#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include "RewardScreen.h"
#include <AzoomeeCommon/UI/RoundedRectSprite.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class CoinCollectLayer : public RewardScreen
{
	typedef RewardScreen Super;
private:
	int _rewardAmount;
	float _incPerSec;
	float _displayValue;
	cocos2d::Label* _valueLabel = nullptr;
	
	cocos2d::LayerColor* _bgColour = nullptr;
	RoundedRectSprite* _pattern = nullptr;
	
	cocos2d::Sprite* _plinth = nullptr;
	cocos2d::ParticleSystemQuad* _smoke = nullptr;
	
	cocos2d::Sprite* _mainCoin = nullptr;
	cocos2d::Sprite* _counterFrame = nullptr;
	
	float _nextParticleEmit = 0;
	
	std::string _oomeeFilepath;
	
	bool _animSkiped = false;
	int _plinthAudioId = -1;
	
	void addBackground();
	void addHeading();
	void addPlinth();
	void addCoinCounter();
	
	void skipAnimation();
	
	cocos2d::ParticleSystemQuad* createSparkleParticles(const cocos2d::Vec2& emissionArea);
	cocos2d::ParticleSystemQuad* createSmokeParticles(const cocos2d::Vec2& emissionArea);
	cocos2d::ParticleSystemQuad* createMeteorParticles();
	
	cocos2d::Sprite* createCoinWithDelay(float delay);
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float deltaT) override;
	void onSizeChanged() override;
	
	void setOomeeFilepath(const std::string& oomeeFilepath);
	
	CREATE_FUNC(CoinCollectLayer);
};

NS_AZ_END


#endif /* CoinCollectLayer_h */
