//
//  AwesomeLayer.h
//  azoomee2
//
//  Created by Macauley on 05/02/2019.
//

#ifndef AwesomeLayer_h
#define AwesomeLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include "RewardScreen.h"

NS_AZOOMEE_BEGIN

class AwesomeLayer : public RewardScreen
{
	typedef RewardScreen Super;
private:
	static const std::vector<cocos2d::Color4F> kColours;
	float _nextParticleEmit = 0;
	
	void addAwsomeText();
	void addStars();
	
public:
	
	bool init() override;
	void onEnter() override;
	void update(float deltaT) override;
	
	CREATE_FUNC(AwesomeLayer);
};

NS_AZOOMEE_END

#endif /* AwesomeLayer_h */
