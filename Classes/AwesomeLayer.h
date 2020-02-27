//
//  AwesomeLayer.h
//  azoomee2
//
//  Created by Macauley on 05/02/2019.
//

#ifndef AwesomeLayer_h
#define AwesomeLayer_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include "RewardScreen.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class AwesomeLayer : public RewardScreen
{
	typedef RewardScreen Super;
private:
	static const std::vector<cocos2d::Color4F> kColours;
	float _nextParticleEmit = 0;
	
	cocos2d::LayerColor* _bgColour = nullptr;
	
	void addAwsomeText();
	void addStars();
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float deltaT) override;
	void onSizeChanged() override;
	
	CREATE_FUNC(AwesomeLayer);
};

NS_AZ_END

#endif /* AwesomeLayer_h */
