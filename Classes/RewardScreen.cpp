//
//  RewardScreen.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/02/2019.
//

#include "RewardScreen.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const int RewardScreen::kAutoCallbackActionTag = 9999;

bool RewardScreen::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	this->setContentSize(Director::getInstance()->getVisibleSize());
	
	_passingTouchBlocker = EventListenerTouchOneByOne::create();
	_passingTouchBlocker->setSwallowTouches(true);
	_passingTouchBlocker->onTouchBegan = [](Touch* touch, Event* event){ return true; };
	_passingTouchBlocker->onTouchEnded = [this](Touch* touch, Event* event){
		// Do your stuff here
	};
	_passingTouchBlocker->retain();
	return true;
}
void RewardScreen::onEnter()
{
	_eventDispatcher->addEventListenerWithFixedPriority(_passingTouchBlocker, -1); // less than zero
	_passingTouchBlocker->release();
	Action* action = Sequence::createWithTwoActions(DelayTime::create(_duration), CallFunc::create([this](){
		if(this->_delegate)
		{
			this->_delegate->onAnimationComplete(_rewardData);
		}
	}));
	action->setTag(kAutoCallbackActionTag);
	this->runAction(action);
	
	Super::onEnter();
}
void RewardScreen::onExit()
{
	_eventDispatcher->removeEventListener(_passingTouchBlocker);
	_passingTouchBlocker = nullptr;
	Super::onExit();
}
void RewardScreen::update(float deltaT)
{
	Super::update(deltaT);
}
void RewardScreen::onSizeChanged()
{
	this->setContentSize(Director::getInstance()->getVisibleSize());
}

void RewardScreen::setDeleagte(RewardScreenDelegate* delegate)
{
	_delegate = delegate;
}
void RewardScreen::setDuration(float duration)
{
	_duration = duration;
}

void RewardScreen::setRewardData(const RewardItemRef &reward)
{
	_rewardData = reward;
}

NS_AZOOMEE_END
