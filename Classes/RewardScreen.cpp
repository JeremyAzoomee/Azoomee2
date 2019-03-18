//
//  RewardScreen.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/02/2019.
//

#include "RewardScreen.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool RewardScreen::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	this->setContentSize(Director::getInstance()->getVisibleSize());
	
	return true;
}
void RewardScreen::onEnter()
{
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(_duration), CallFunc::create([this](){
		if(this->_delegate)
		{
			this->_delegate->onAnimationComplete(_rewardData);
		}
	})));
	
	Super::onEnter();
}
void RewardScreen::onExit()
{
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
