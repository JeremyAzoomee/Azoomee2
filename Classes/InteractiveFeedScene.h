//
//  InteractiveFeedScene.h
//  azoomee2
//
//  Created by Macauley on 11/12/2018.
//

#ifndef InteractiveFeedScene_h
#define InteractiveFeedScene_h

#include <AzoomeeCommon/UI/Scene.h>

NS_AZOOMEE_BEGIN

class InteractiveFeedScene : public Azoomee::Scene
{
	typedef Azoomee::Scene Super;
private:
	cocos2d::ParticleSystem* _touchTrail = nullptr;
	bool _touchStarted = false;
	
public:
	
	virtual bool init();
	virtual void onEnter();

	CREATE_FUNC(InteractiveFeedScene);
};

NS_AZOOMEE_END

#endif /* InteractiveFeedScene_h */
